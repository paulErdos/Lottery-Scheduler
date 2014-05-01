Team Name: Team++
Project 2: README.txt
CMPS 111: Spring 2014

CREATED (4-19-2014)

Build Instructions for Static Scheduler:
1. In the top-level directory type 'make copy-static-all' to copy
   our source files to the Minix system. Note that this overwrites the
   original Minix files.
2. In the same directory type 'make' to build and install
   the Minix kernel.
3. Type shutdown.
4. Type exit to reboot the newly built system.

Build Instructions for Dynamic Scheduler:
1. In the top-level directory type 'make copy-dynamic-all' to copy
   our source files to the Minix system. Note that this overwrites the
   original Minix files.
2. In the same directory type 'make' to build and install
   the Minix kernel.
3. Type shutdown.
4. Type exit to reboot the newly built system.

Quick Testing the Static Scheduler:
1. In the test directory type 'make' to build both the tests.

Quick Testing the Dynamic Scheduler:
1. In the test directory type 'make' to build both the tests.

Directory Structure:
- Makefile
- README.txt
- test
    - io_test.c
    - performance_test.c
    - input_test.txt
    - Makefile
- static
    - usr
        - src
            - include
                - minix
                    - config.h
                    - Makefile
            - kernel
                - main.c
                - Makefile
            - servers
                - pm
                    - schedule.c
                    - utility.c
                    - Makefile
                - sched
                    - schedule.c
                    - schedproc.h
                    - Makefile
- dynamic
    - usr
        - src
            - include
                - minix
                    - config.h
                    - Makefile
            - kernel
                - main.c
                - Makefile
            - servers
                - pm
                    - schedule.c
                    - utility.c
                    - Makefile
                - sched
                    - schedule.c
                    - schedproc.h
                    - Makefile