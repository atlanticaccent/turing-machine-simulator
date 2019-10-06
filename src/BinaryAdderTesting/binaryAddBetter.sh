#!/bin/bash

MAXLOOPS=2000
COUNTER=0
LEFT="1"
RIGHT="0"
RESULT=""
FILE=$1

rm ${FILE}.txt

for ((i=0;i<MAXLOOPS;i++))
do
    let "COUNTER++"
    echo -n "${LEFT}#${RIGHT}#${RESULT}1" | tee > ${FILE}.tape
    cat ${FILE}.tape
    echo " ${COUNTER}"
    OUTPUT=$(../runtm binadd.tm ${FILE}.tape | grep '^[^Ba]')
    WRITE=$(printf "%s %s\n" "${COUNTER}" "${OUTPUT}")
    echo "${WRITE}" >> ${FILE}.txt
    LEFT+="0"
    if [ $i -eq 0 ]; then
        RIGHT+="1"
        RESULT+="1"
    else
        RIGHT=$(printf "0%s" ${RIGHT})
        RESULT+="0"
    fi
done