#!/usr/bin/perl
my @thing = `ls > /dev/tty`;

foreach (@thing) {
	print $_;
}
print "done\n";
