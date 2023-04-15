#!/bin/bash

function seq {
	N=$1
	echo -n "N = $N"
	/usr/bin/time -f "time: %E" -o seq.txt ./sec	$N >&1  2>1
	cat seq.txt
}

N1=9400
N2=219500
delta=21010

for(( N = N1; N <= N2; N+=delta ))
do
	seq $N
done
