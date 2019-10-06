#!/bin/bash

MAXLOOPS=2000
COUNTER=0
INPUT="1"
FILE=$1

rm ${FILE}.txt

for ((i=0;i<MAXLOOPS;i++))
do
    let "COUNTER++"
    echo "${COUNTER}"
    echo -n "${INPUT}#${INPUT}#0${INPUT}" | tee > ${FILE}.tape
    OUTPUT=$(../runtm binadd.tm ${FILE}.tape | grep '^[^0a]')
    WRITE=$(printf "%s %s\n" "${COUNTER}" "${OUTPUT}")
    echo "${WRITE}" >> ${FILE}.txt
    INPUT+="1"
done