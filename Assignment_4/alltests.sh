#!/bin/bash

# set -x

ZASTRE_DIR=/home/zastre/seng265/tests
BEN_DIR=/home/wolfeben/SENG_265/mygit/a4/my_tests
IN_PREFIX="t"
IN_SUFFIX="ph1"
OUT_PREFIX="t"
OUT_SUFFIX="ph2"

DIFF=/usr/bin/diff
RM=/bin/rm


TESTS=('01' '02' '03' '04' '05' '06' '07' '08' '09' '10' '11' '12' '13' '14' '15' '16' '17' '18' '19' '20');


for T in "${TESTS[@]}"
do
    ./phase2 \
         --infile $ZASTRE_DIR/${IN_PREFIX}${T}.${IN_SUFFIX} \
        --outfile $BEN_DIR/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
        --encode
    $DIFF $BEN_DIR/${OUT_PREFIX}${T}.${OUT_SUFFIX} $ZASTRE_DIR/${OUT_PREFIX}${T}.${OUT_SUFFIX}
    if [ $? -eq 0 ]
    then
        echo "$T encode: passed"
    else
        echo "$T encode: FAILED"
    fi
done


for T in "${TESTS[@]}"
do
    ./phase2 \
         --infile $ZASTRE_DIR/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
        --outfile $BEN_DIR/${IN_PREFIX}${T}.${IN_SUFFIX} \
       --decode
    $DIFF $BEN_DIR/${IN_PREFIX}${T}.${IN_SUFFIX} $ZASTRE_DIR/${IN_PREFIX}${T}.${IN_SUFFIX}
    if [ $? -eq 0 ]
    then
        echo "$T decode: passed"
    else
        echo "$T decode: FAILED"
    fi
done
