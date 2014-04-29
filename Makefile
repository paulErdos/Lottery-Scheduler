# Team Name: Team++
# Project 2: Main Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# make              -> Builds the kernel. Be careful!
# make copy-all     -> Copies ALL Minix source files 
#                      to their proper place on the system.
#                      Removes old files. Be careful (use sparingly)!
# make copy-<name>  -> Copies <name> file to the Minix system (use this more often).

all:
	cd /usr/src/tools; make install

copy-all:
	cd usr/src/include/minix; make
	cd usr/src/servers/sched; make
	cd usr/src/kernel; make
	cd usr/src/servers/pm; make

copy-config:
	cd usr/src/include/minix; make

copy-schedproc:
	cd usr/src/servers/sched; make schedproc

copy-schedule:
	cd usr/src/servers/sched; make schedule

copy-pm-schedule:
	cd usr/src/servers/pm; make

copy-main:
	cd usr/src/kernel; make