#!/usr/bin/perl

$pid1 = open $fhOut, "| time ./performance_test &",
#> /dev/tty 2> output1&",
			or die "death has occurred.";
#my $pid2 = open $fhOut, "| ./performance_test > /dev/tty 2> output2&",
#			or die "death has occurred.";

#$pid1 = $pid1 + 1;
#$pid2 = $pid2 + 1;

print "pid1 is $pid1\n";
#print "pid2 is $pid2\n";

print "before waitpid\n";

waitpid($pid1,0);
#waitpid($pid2,0);

print "after waitpid\n";

#print $$;
