#! /bin/bash
for ((i = 0; i < $1; i++))
do 
  od -A n -t d -N 1 /dev/random >> tmp
done
