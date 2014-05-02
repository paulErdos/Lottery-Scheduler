#!usr/bin/perl

#fork a child
$pid = fork();

if(!defined($pid)) {
	die "Fork failed.";
} elsif(!$pid) { #child code
	#start both processes
	my $nothing = `time ./performance_test > /dev/tty 2> output1 &
			time ./performance_test > /dev/tty 2> output2`;
} else { #parent code

	waitpid($pid, 0);
	
	my @return1 = `cat output1`;
	my @return2 = `cat output2`;

	#$return[0] is the output of time
	@times1 = split(/ /, $return1[0]);
	@times2 = split(/ /, $return2[0]);

	#$times[6] is the actual amount of time
	$a=$times1[6];
	$b=$times2[6];

	print "The ratio of the time to completion of both processes is:\n";
	print $b/$a;
	print "\n";
}
