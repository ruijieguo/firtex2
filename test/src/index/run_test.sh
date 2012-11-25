#!/usr/bin/env bash
for i in {1..1000}
do
    rm ./log.txt -rf
    ./index_test
    if [ $? -eq 1 ]
	then 
	echo "error"
	exit
    fi
done