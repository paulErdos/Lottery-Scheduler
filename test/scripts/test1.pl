#!/usr/bin/perl

#fork two children
my $pid1 = fork();

if(!defined($pid1)) {
	die "Fork 1 failed";
} 

#here in the parent we'll fork the second child and wait for both to 
#finish
elsif ($pid1) { 

	$pid2 = fork();
	if(!defined($pid2)) {
		die "Fork 2 failed.";
	}

	#this is parent 2
	#the waitpid may need to go here
	elsif($pid2) {
	}

	else {
		exec('time nice -n -20 ./performance_test > /dev/tty 2> output2');

		die "Exec 2 failed";
	}


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

	print $time1/$time2;

	
	print "This is the parent.\n";
} 

else { #the child should execute first
	
	exec('time nice -n 20 ./performance_test > /dev/tty 2> output1');
	
	die "Exec 1 failed.";
}

