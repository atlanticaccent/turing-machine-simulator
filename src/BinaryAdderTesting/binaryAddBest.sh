#!/bin/bash

MAXLOOPS=2000
COUNTER=0
RESULT=""
FILE=$1

rm ${FILE}.txt

for ((i=0;i<MAXLOOPS;i++))
do
    let "COUNTER++"
    RESULT+="0"
    echo -n "##${RESULT}" | tee > ${FILE}.tape
    cat ${FILE}.tape
    echo " ${COUNTER}"
    OUTPUT=$(../runtm binadd.tm ${FILE}.tape | grep '^[^Ba]')
    WRITE=$(printf "%s %s\n" "${COUNTER}" "${OUTPUT}")
    echo "${WRITE}" >> ${FILE}.txt
done