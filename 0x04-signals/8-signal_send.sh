#!/usr/bin/bash
if [ "$#" -ne 1 ]; then
	echo "Usage: ${0} <pid>"
	exit
fi
kill -3 $1;
