# Team Name: Team++
# Project 2: Main Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# make              	   -> Builds the kernel. Be careful!
# make copy-<type>-all     -> Copies ALL Minix source files of the given type
#                      		  to their proper place on the system.
#                      	      Removes old files. Be careful (use sparingly)!
# make copy-<type>-<name>  -> Copies <name> file to the Minix system for scheduler <type> (use this more often).

# <type> can either be static or dynamic.

all:
	cd /usr/src/tools; make install

copy-static-all:
	cd static/usr/src/include/minix; make
	cd static/usr/src/servers/sched; make
	cd static/usr/src/kernel; make
	cd static/usr/src/servers/pm; make

copy-dynamic-all:
	cd dynamic/usr/src/include/minix; make
	cd dynamic/usr/src/servers/sched; make
	cd dynamic/usr/src/kernel; make
	cd dynamic/usr/src/servers/pm; make

copy-static-config:
	cd static/usr/src/include/minix; make

copy-static-schedproc:
	cd static/usr/src/servers/sched; make schedproc

copy-static-schedule:
	cd static/usr/src/servers/sched; make schedule

copy-static-pm-schedule:
	cd static/usr/src/servers/pm; make

copy-static-main:
	cd static/usr/src/kernel; make

copy-dynamic-config:
	cd dynamic/usr/src/include/minix; make

copy-dynamic-schedproc:
	cd dynamic/usr/src/servers/sched; make schedproc

copy-dynamic-schedule:
	cd dynamic/usr/src/servers/sched; make schedule

copy-dynamic-pm-schedule:
	cd dynamic/usr/src/servers/pm; make

copy-dynamic-main:
	cd dynamic/usr/src/kernel; make