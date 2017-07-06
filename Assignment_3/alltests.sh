#!/bin/bash

# set -x

TESTDIR=/Users/Ben/OneDrive/GitHub/SENG_265/Assignment_3/zastre_tests
IN_PREFIX="t"
IN_SUFFIX="ph1"
OUT_PREFIX="t"
OUT_SUFFIX="ph2"

DIFF=/usr/bin/diff
RM=/bin/rm


TESTS=('01' '02' '03' '04' '05' '06' '07' '08' '09' '10' '11' '12' '13' '14' '15' '16' '17' '18' '19' '20');


for T in "${TESTS[@]}"
do
    ./phase2.py \
         --infile $TESTDIR/${IN_PREFIX}${T}.${IN_SUFFIX} \
        --outfile /Users/Ben/OneDrive/GitHub/SENG_265/Assignment_3/my_tests/${OUT_PREFIX}${T}_ben.${OUT_SUFFIX} \
        --encode
    $DIFF /Users/Ben/OneDrive/GitHub/SENG_265/Assignment_3/my_tests/${OUT_PREFIX}${T}_ben.${OUT_SUFFIX} $TESTDIR/${OUT_PREFIX}${T}.${OUT_SUFFIX}
    if [ $? -eq 0 ]
    then
        echo "$TESTNUM encode: passed"
    else
        echo "$TESTNUM encode: FAILED"
    fi
    #	$RM /tmp/$$
done


for T in "${TESTS[@]}"
do
    ./phase2.py \
         --infile $TESTDIR/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
        --outfile /Users/Ben/OneDrive/GitHub/SENG_265/Assignment_3/my_tests/${IN_PREFIX}${T}_ben.${IN_SUFFIX} \
        --decode
    $DIFF /Users/Ben/OneDrive/GitHub/SENG_265/Assignment_3/my_tests/${IN_PREFIX}${T}_ben.${IN_SUFFIX} $TESTDIR/${IN_PREFIX}${T}.${IN_SUFFIX}
    if [ $? -eq 0 ]
    then
        echo "$TESTNUM decode: passed"
    else
        echo "$TESTNUM decode: FAILED"
    fi
    # $RM /tmp/$$
done
