# Team Name: Team++
# Project 2: Scheduler Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# Note: These rules remove the files at COPYDIR, replacing 
#       them with your own. Be careful!

# make            -> Copies all sched files to COPYDIR
# make main       -> Copies main.c to COPYDIR
# make proto      -> Copies proto.h to COPYDIR
# make sched      -> Copies sched.h to COPYDIR
# make schedproc  -> Copies schedproc.h to COPYDIR
# make schedule   -> Copies schedule.c to COPYDIR
# make utility    -> Copies utility.c to COPYDIR

COPYDIR = /usr/src/servers/sched

all:
	make main
	make proto
	make sched
	make schedproc
	make schedule
	make utility

main:
	cp main.c $(COPYDIR)

proto:
	cp proto.h $(COPYDIR)

sched:
	cp sched.h $(COPYDIR)

schedproc:
	cp schedproc.h $(COPYDIR)

schedule:
	cp schedule.c $(COPYDIR)

utility:
	cp utility.c $(COPYDIR)