# Team Name: Team++
# Project 2: Scheduler Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# Note: These rules remove the files at COPYDIR, replacing 
#       them with your own. Be careful!

# make            -> Copies all sched files to COPYDIR
# make schedproc  -> Copies schedproc.h to COPYDIR
# make schedule   -> Copies schedule.c to COPYDIR

COPYDIR = /usr/src/servers/sched

all:
	make schedproc
	make schedule

schedproc:
	cp schedproc.h $(COPYDIR)

schedule:
	cp schedule.c $(COPYDIR)