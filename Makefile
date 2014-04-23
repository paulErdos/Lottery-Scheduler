# Team Name: Team++
# Project 2: Main Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# make              -> Builds the kernel. Be careful!
# make copy-all     -> Copies ALL Minix source files 
#                      to their proper place on the system.
#                      Removes old files. Be careful!
# make copy-<name>  -> Copies <name> file to the Minix system.

all:
	cd /usr/src/tools; make; make install; shutdown

copy-all:
	cd usr/include/minix; make
	cd usr/src/servers/sched; make

copy-config:
	cd usr/include/minix; make

copy-schedproc:
	cd usr/src/servers/sched; make schedproc

copy-schedule:
	cd usr/src/servers/sched; make schedule