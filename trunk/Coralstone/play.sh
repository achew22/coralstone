#!/bin/sh
# This file is written by Andrew Allen
# And adheres to the GPL license
# I don't care to put it all in here but it should be distributed
# With the project
#
# By using this file you promise that you wont bring suit against me
# Also -- I am great

i=$(echo `svn up` |awk '{print substr($0, 0 , 2)}')
if [ $i != "At" ]; then
	#Project isnt up to date
	echo "Your project isn't up to date, right now it's 
recompiling";
	make
fi
./autorpg
