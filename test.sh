#!/bin/bash

program=$1
files="${2}/*.in"

rm -f ${2}/"*.myout"
rm -f ${2}/"*.myerr"

for f in $files; do
	myoutput=$(echo "$f" | sed "s/.in$/.myout/");
    myerror=$(echo "$f" | sed "s/.in$/.myerr/");
	echo "$myoutput"
	echo "$myerror"
	${program} < $f 1> $myoutput 2>myerror
done
