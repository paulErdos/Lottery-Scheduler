# Team Name: Team++
# Project 2: Test Makefile
# CMPS 111: Spring 2014

# CREATED (4-19-2014)

# make                  -> Builds both tests.
# make performance_test -> Builds the performance test.
# make io_test          -> Builds the IO test.

all:
	make performance_test
	make io_test

performance_test : performance_test.o
	cc -o performance_test performance_test.o

performance_test.o : performance_test.c
	cc -c performance_test.c

io_test : io_test.o
	cc -o io_test io_test.o

io_test.o : io_test.c
	cc -c io_test.c

clean :
	rm -f io_test performance_test io_test.o performance_test.o