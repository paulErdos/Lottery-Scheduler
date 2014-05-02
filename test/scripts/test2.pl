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

	#this is the second child, in which we will time the performance 
	#test on a given number of tickets
	elsif(!$pid2) {
#		exec('time nice -n 80 ./performance_test > /dev/tty 2> output2');
		$begin_time2 = `date +%s`;
		@nothing = `nice -n 80 ./performance_test > /dev/tty`;
		$end_time2 = `date +%s`;
		$time_to_completion2 = $end_time2 - $begin_time2;

		@nothing = `echo $time_to_completion2 > output2`;	
		print "2\n";
		exit();
#		#die if the exec fails
#		die "Exec 2 failed";
	}

	#wait for both children to finish
	waitpid($pid1, 0);
	waitpid($pid2, 0);

	wait();
	wait();

	#now the arithmetic can occur
	#get output from the time command
	$time1 = `cat output1`;
	$time2 = `cat output2`;


		
	#parse the output on the spaces
#	@times1 = split(/ /, $return1[0]);
#	@times2 = split(/ /, $return2[0]);
#
#	#get the time
#	$time1 = $times1[6];
#	$time2 = $times2[6];

	#compute and print the ratio
	print "\n\n\nThis has been a test of two identical CPU-intensive\n";
	print "tasks, each with 30 tickets.\n";
	print "If both of these equally-ticketed processes completed\n";
	print "in the same amount of time, this figure, the ratio of\n";
	print "their runtimes, should be very close to one: \n\n";
	print $time1/$time2;
	print "\n\n";
} 

#this is the first child
else {
	$begin_time1 = `date +%s`;
	@nothing = `nice -n -19 ./performance_test > /dev/tty`;
	$end_time1 = `date +%s`;
	$time_to_completion1 = $end_time1 - $begin_time1;
	
	@nothing = `echo $time_to_completion1 > output1`;

	print "1\n";
	exit();

#	#execute the timed test
#	exec('time nice -n 30 ./performance_test > /dev/tty 2> output1');
#	
#	#handle the case in which the exec fails
#	die "Exec 1 failed.";
}

