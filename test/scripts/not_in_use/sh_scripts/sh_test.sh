#!/bin/sh
echo hello world

time ./performance_test 2> output

#the first word is the time to completion
i=0
for word in $(cat output)
do
	i=$word
	break	
done

echo $i
