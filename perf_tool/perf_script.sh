#!/bin/bash

if [ $1 = "lock" ] ;then
	perf lock record dd if=/dev/zero of=/dev/null bs=1M count=1024
fi

if [ $1 = "stat" ] && [ $2 -eq 1 ] ;then
	perf stat -e cycles,instructions,branch-misses,bus-cycles,cache-misses,cache-references sleep 1
fi

if [ $1 = "stat" ] && [ $2 -eq 2 ] ;then
	perf stat -e context-switches,cpu-clock sleep 10
fi
