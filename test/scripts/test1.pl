#!/usr/bin/perl

#fork a child
my $pid1 = fork();

#handle the case in which the fork fails
if(!defined($pid1)) {
	die "Fork 1 failed";
} 

#here in the parent we'll fork a second child and wait for both to finish
elsif ($pid1) { 

	#fork a second child and handle the case in which it fails
	$pid2 = fork();
	if(!defined($pid2)) {
		die "Fork 2 failed.";
	}

	#this is the second child, in which we will time the performance test
	#on a given number of tickets
	elsif(!$pid2) {
		exec('time nice -n -20 ./performance_test > /dev/tty 2> output2');

		#die if the exec fails
		die "Exec 2 failed";
	}

	#wait for both children to finish
	waitpid($pid1, 0);
	waitpid($pid2, 0);

	#now the arithmetic can occur
	#get output from the time command
	my @return1 = `cat output1`;
	my @return2 = `cat output2`;
	
	#parse the output on the spaces
	my @times1 = split(/ /, $return1[0]);
	my @times2 = split(/ /, $return2[0]);

	#get the time
	$time1 = $times1[6];
	$time2 = $times2[6];

	#compute and print the ratio
	print "If these both of these equally-ticketed processes ran in the same amount of time, this figure, the ratio of their runtimes, should be very close to one: ";
	print $time1/$time2;
	print "\n";
} 

#this is the first child
else {
	#execute the timed test
	exec('time nice -n 80 ./performance_test > /dev/tty 2> output1');
	
	#handle the case in which the exec fails
	die "Exec 1 failed.";
}

