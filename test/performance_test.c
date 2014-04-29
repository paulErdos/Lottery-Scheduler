/*
Team Name: Team++
Project 2: performance_test.c
CMPS 111: Spring 2014

CREATED (4-19-2014)

Currently runs in MINIX 3 in ~10.41 seconds as specified.
*/

#include <stdio.h>
#include <unistd.h>

#define MAX_INT 2147483648   /* Max integer size on MINIX 3. */
#define LOOP_CHECK 100000    /* Check to exit loop and check to print status every LOOP_CHECK (100000). */
#define MAX_CHECK 50000      /* Max number of times we can check to exit or check to print. */
#define DIVISOR_CHECK 100  /* Helps decide when to print. */

/*
main(int argc, char *argv[]): The entry point.
*/
int main(int argc, char *argv[]) {
    int loops = 0;          /* Increments every while iteration. Gets reset after every LOOP_CHECK. */
    int value = 1;          /* The value of the arithmetic instruction. */
    int checks = 0;         /* Total number of times the we have checked to exit and checked to print. */
    int inst_count = 0;     /* Number of arithmetic calculations we preform. */
    int overflow_count = 0; /* Count of how many integer overflows would have happened. */

    while (1) {
        value = (2*value) - 1; /* An arithmetic calculation every loop. */

        if (inst_count == MAX_INT) { /* If our instruction count overflows. */
            inst_count = 1; /* Set instruction count to 1. */
            ++overflow_count; /* Increment number of overflows. */
        }
        else {
            ++inst_count; /* Else simply increment instruction count. */
        }

        ++loops; /* Increment total number of loops. */

        if (loops == LOOP_CHECK) { /* Every LOOP_CHECK many loops we check to exit and check to print. */
            loops = 0; /* Reset total number of loops so we can check again in the future if we need too. */

            if (checks == MAX_CHECK) {
                break; /* Break loop if we have checked the max number of times. */
            }

            if (checks % DIVISOR_CHECK == 0) { /* Print status every time checks is an even multiple of DIVISOR_CHECK. */
                printf("\nProcess ID: %d\nWork Done: ((%d * 2147483648) + %d)) computations\n", getpid(), overflow_count, inst_count);
            }

            ++checks; /* Increment total number of checks. */
        }
    }

    printf("\nProcess ID: %d\nWork Done: ((%d * 2147483648) + %d)) computations\n", getpid(), overflow_count, inst_count);

    return 0;
}