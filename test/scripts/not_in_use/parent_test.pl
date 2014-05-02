#!/usr/bin/perl

#$0 represents the name of this process
#$$ represents the pid of this process
print "I'm the parent ($0): $$\n";

my $child_pid = fork();

#$! contains error information
defined $child_pid or die "can't fork: $!\n";

#unless means if(! ... )
unless ($child_pid) { #this indicates the child process
	print "I'm child ($0): $$\n";
	exec $^X, './child-child.pl';
}

