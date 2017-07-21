#!bin/bash

pcm=$1
start=$2

size=$[ ($start+100)*16000 ]
head -c $size $pcm > tmp
tail -c 1600000 tmp > ${pcm%.pcm}_$start.pcm

rm tmp
