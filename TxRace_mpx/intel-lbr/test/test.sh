#!/bin/bash

./test |grep "0x"> output.log 2>&1

while read line; do
	fromaddr=`echo $line | cut -d'-' -f1`
	toaddr=`echo $line | cut -d'>' -f2`
	echo "FROM:$fromaddr"
	addr2line -e ./test -f -a $fromaddr
	echo "TO:$toaddr"
	addr2line -e ./test -f -a $toaddr
	echo "--------"
done<output.log

