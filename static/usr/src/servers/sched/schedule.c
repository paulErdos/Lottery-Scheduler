/* This file contains the scheduling policy for SCHED
 *
 * The entry points are:
 *   do_noquantum:        Called on behalf of process' that run out of quantum
 *   do_start_scheduling  Request to start scheduling a proc
 *   do_stop_scheduling   Request to stop scheduling a proc
 *   do_nice          Request to change the nice level on a proc
 *   init_scheduling      Called from main.c to set up/prepare scheduling
 */
#include "sched.h"
#include "schedproc.h"
#include <assert.h>
#include <minix/com.h>
#include <machine/archtypes.h>
#include "kernel/proc.h" /* for queue constants */

PRIVATE timer_t sched_timer;
PRIVATE unsigned balance_timeout;

/* CHANGED START (4-21-2014) */

#define MIN_TICKETS 1   /* Minimum number of tickets a process can have. */
#define MAX_TICKETS 100 /* Maximum number of tickets a process can have. */

/* CHANGED END (4-21-2014) */

#define BALANCE_TIMEOUT 5 /* how often to balance queues in seconds */

FORWARD _PROTOTYPE( int schedule_process, (struct schedproc * rmp)  );
FORWARD _PROTOTYPE( void balance_queues, (struct timer *tp)     );

#define DEFAULT_USER_TIME_SLICE 200

/*===========================================================================*
 *              do_noquantum                     *
 *===========================================================================*/

PUBLIC int do_noquantum(message *m_ptr)
{
    register struct schedproc *rmp;
    int rv, proc_nr_n;

    if (sched_isokendpt(m_ptr->m_source, &proc_nr_n) != OK) {
        printf("SCHED: WARNING: got an invalid endpoint in OOQ msg %u.\n",
        m_ptr->m_source);
        return EBADEPT;
    }

    rmp = &schedproc[proc_nr_n];

    /* CHANGED START (4-21-2014) */

    if (check_if_user(rmp)) { /* If we have a user process that is out of quantum. */
        rmp->priority = LOSER_Q; /* Put it in the loser queue. */
        super_lotto(); /* Play the lottery. */
    }
    else if (rmp->priority < (MAX_USER_Q - 1)) { /* Kernel process. If this priority is less than the max priority for the user minus 1. */
        rmp->priority += 1; /* Lower priority. */
    }

    /* CHANGED END (4-21-2014) */

    if ((rv = schedule_process(rmp)) != OK) {
        return rv;
    }
    
    return OK;
}

/*===========================================================================*
 *              do_stop_scheduling               *
 *===========================================================================*/
PUBLIC int do_stop_scheduling(message *m_ptr)
{
    register struct schedproc *rmp;
    int rv, proc_nr_n;

    /* check who can send you requests */
    if (!accept_message(m_ptr))
        return EPERM;

    if (sched_isokendpt(m_ptr->SCHEDULING_ENDPOINT, &proc_nr_n) != OK) {
        printf("SCHED: WARNING: got an invalid endpoint in OOQ msg "
        "%ld\n", m_ptr->SCHEDULING_ENDPOINT);
        return EBADEPT;
    }

    rmp = &schedproc[proc_nr_n];
    rmp->flags = 0; /*&= ~IN_USE;*/

    return OK;
}

/*===========================================================================*
 *              do_start_scheduling              *
 *===========================================================================*/
PUBLIC int do_start_scheduling(message *m_ptr)
{
    register struct schedproc *rmp;
    int rv, proc_nr_n, parent_nr_n;
    
    /* we can handle two kinds of messages here */
    assert(m_ptr->m_type == SCHEDULING_START || 
        m_ptr->m_type == SCHEDULING_INHERIT);

    /* check who can send you requests */
    if (!accept_message(m_ptr))
        return EPERM;

    /* Resolve endpoint to proc slot. */
    if ((rv = sched_isemtyendpt(m_ptr->SCHEDULING_ENDPOINT, &proc_nr_n))
            != OK) {
        return rv;
    }
    rmp = &schedproc[proc_nr_n];

    /* Populate process slot */
    rmp->endpoint     = m_ptr->SCHEDULING_ENDPOINT;
    rmp->parent       = m_ptr->SCHEDULING_PARENT;
    rmp->max_priority = (unsigned) m_ptr->SCHEDULING_MAXPRIO;
    if (rmp->max_priority >= NR_SCHED_QUEUES) {
        return EINVAL;
    }
    
    switch (m_ptr->m_type) {

    case SCHEDULING_START:
        /* We have a special case here for system processes, for which
         * quanum and priority are set explicitly rather than inherited 
         * from the parent */
        rmp->priority   = rmp->max_priority;
        rmp->time_slice = (unsigned) m_ptr->SCHEDULING_QUANTUM;
        /* We have a system process! */
        break;
        
    case SCHEDULING_INHERIT:
        /* Inherit current priority and time slice from parent. Since there
         * is currently only one scheduler scheduling the whole system, this
         * value is local and we assert that the parent endpoint is valid */
        if ((rv = sched_isokendpt(m_ptr->SCHEDULING_PARENT,
                &parent_nr_n)) != OK)
            return rv;

        rmp->time_slice = schedproc[parent_nr_n].time_slice;

        /* CHANGED START (4-21-2014) */

        rmp->ticket_count = 20; /* We have a user process! Give it 20 tickets. */
        rmp->priority = LOSER_Q; /* Put it in the loser queue until it wins. */

        /* CHANGED END (4-21-2014) */
        
        break;
        
    default: 
        /* not reachable */
        assert(0);
    }

    /* Take over scheduling the process. The kernel reply message populates
     * the processes current priority and its time slice */
    if ((rv = sys_schedctl(0, rmp->endpoint, 0, 0)) != OK) {
        printf("Sched: Error taking over scheduling for %d, kernel said %d\n",
            rmp->endpoint, rv);
        return rv;
    }
    rmp->flags = IN_USE;

    /* Schedule the process, giving it some quantum */
    if ((rv = schedule_process(rmp)) != OK) {
        printf("Sched: Error while scheduling process, kernel replied %d\n",
            rv);
        return rv;
    }

    /* Mark ourselves as the new scheduler.
     * By default, processes are scheduled by the parents scheduler. In case
     * this scheduler would want to delegate scheduling to another
     * scheduler, it could do so and then write the endpoint of that
     * scheduler into SCHEDULING_SCHEDULER
     */

    m_ptr->SCHEDULING_SCHEDULER = SCHED_PROC_NR;

    return OK;
}

/*===========================================================================*
 *              do_nice                      *
 *===========================================================================*/
PUBLIC int do_nice(message *m_ptr)
{
    struct schedproc *rmp;
    int proc_nr_n;

    /* CHANGED START (4-21-2014) */

    int tickets_passed; /* How many tickets were passed to this function. */
    int new_ticket_total; /* How many tickets will this process have after nice(). */

    /* CHANGED END (4-21-2014) */

    /* check who can send you requests */
    if (!accept_message(m_ptr))
        return EPERM;

    if (sched_isokendpt(m_ptr->SCHEDULING_ENDPOINT, &proc_nr_n) != OK) {
        printf("SCHED: WARNING: got an invalid endpoint in OOQ msg "
        "%ld\n", m_ptr->SCHEDULING_ENDPOINT);
        return EBADEPT;
    }

    rmp = &schedproc[proc_nr_n];

    /* CHANGED START (4-21-2014) */

    tickets_passed = m_ptr->SCHEDULING_MAXPRIO; /* Get how many tickets passed to the nice() command. */

    new_ticket_total = (rmp->ticket_count + tickets_passed); /* Total tickets after added to tickets passed from nice(). */

    if (new_ticket_total <= MAX_TICKETS && new_ticket_total >= MIN_TICKETS) {
        rmp->ticket_count = new_ticket_total; /* Allot those tickets to this process. */
    }
    else { /* Else print error. */
        printf("error - cannot give this process less that 1 ticket, or more than 100 tickets.\n");
    }

    return schedule_process(rmp);

    /* CHANGED END (4-21-2014) */
}

/*===========================================================================*
 *              schedule_process                 *
 *===========================================================================*/
PRIVATE int schedule_process(struct schedproc * rmp)
{
    int rv;

    if ((rv = sys_schedule(rmp->endpoint, rmp->priority,
            rmp->time_slice)) != OK) {
        printf("SCHED: An error occurred when trying to schedule %d: %d\n",
        rmp->endpoint, rv);
    }

    return rv;
}


/*===========================================================================*
 *              start_scheduling                 *
 *===========================================================================*/
PUBLIC void init_scheduling(void)
{
    balance_timeout = BALANCE_TIMEOUT * sys_hz();
    init_timer(&sched_timer);
    set_timer(&sched_timer, balance_timeout, balance_queues, 0);
}

/*===========================================================================*
 *              balance_queues                   *
 *===========================================================================*/

/* This function in called every 100 ticks to rebalance the queues. The current
 * scheduler bumps processes down one priority when ever they run out of
 * quantum. This function will find all proccesses that have been bumped down,
 * and pulls them back up. This default policy will soon be changed.
 */
PRIVATE void balance_queues(struct timer *tp)
{
    struct schedproc *rmp;
    int proc_nr;

    for (proc_nr = 0, rmp = schedproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
        if (rmp->flags & IN_USE) {

            /* CHANGED START (4-24-2014) */

            if ( (rmp->priority > rmp->max_priority) && (!check_if_user(rmp)) ) {
                rmp->priority -= 1; /* Increase priority (default behavior). */
            }

            schedule_process(rmp); /* Reschedule all in use processes to make sure the changes occur. */

            /* CHANGED END (4-24-2014) */
        }
    }

    set_timer(&sched_timer, balance_timeout, balance_queues, 0);
}

/* CHANGED START (4-23-2014) */

/*===========================================================================*
 *              super_lotto                  *
 *===========================================================================*/
PRIVATE void super_lotto(void)
{
    struct schedproc *rmp = 0, *winner = 0; /* The process. */
    int proc_nr = 0;
    int total_tickets = 0; /* The total number of tickets in all the processes. */
    int winning_ticket = 0; /* Holds winning ticket location. */

    /* Loop through each process, and count up all the tickets. */
    for (proc_nr = 0, rmp = schedproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
        if ( (rmp->flags & IN_USE) && check_if_user(rmp) ) { /* If we have an in-use user process. */
            total_tickets += rmp->ticket_count; /* Tally the total number of tickets. */
        }
    }

    winning_ticket = rand() % total_tickets + 1; /* Pick a winning ticket. */

    /* Loop through each process to see who has the winning ticket. */
    for (proc_nr = 0, rmp = schedproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
        if ( (rmp->flags & IN_USE) && check_if_user(rmp) ) { /* If we have an in-use, user process. */
            if (winning_ticket >= 0) {
                winning_ticket -= rmp->ticket_count; /* Range check to see if winning ticket is in this processes range. */
            }

            if (!winner && winning_ticket < 0) { /* If the winning ticket is in the range. */
                rmp->priority = WINNER_Q; /* Put the process in the winning queue. */
                winner = rmp; /* Make note of the winner. */
            }
            else {
                rmp->priority = LOSER_Q; /* If the process loses, put it in the loser queue. */
            }
        }
    }

    if (winner) {
        schedule_process(winner); /* Reschedule to promote the winner. */
    }
}

/*===========================================================================*
 *              check_if_user                *
 *===========================================================================*/
PRIVATE int check_if_user(struct schedproc *rmp)
{
    if (rmp->priority >= WINNER_Q && rmp->priority <= LOSER_Q) {
        return 1; /* Return true if we have a user process. */
    }

    return 0; /* False otherwise. */
}

 /* CHANGED END (4-23-2014) */