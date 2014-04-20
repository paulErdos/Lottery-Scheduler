# Team Name: Team++
# Project 2: Kernel Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# Note: These rules remove the files at COPYDIR, replacing 
#       them with your own. Be careful!

# make        -> Copies all kernel files to COPYDIR
# make proc_c -> Copies proc.c to COPYDIR
# make proc_h -> Copies proc.h to COPYDIR

COPYDIR = /usr/src/kernel

all:
	make proc_c
	make proc_h

proc_c:
	cp proc.c $(COPYDIR)

proc_h:
	cp proc.h $(COPYDIR)