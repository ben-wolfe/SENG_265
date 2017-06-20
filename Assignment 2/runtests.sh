#!/bin/bash

# usage: ./runtests.sh

# This assumes the executable is in the current directory.

# set -x 
TEST_DIR=/home/zastre/seng265/tests
ASSIGNDIR=a1
IN_PREFIX="t"
IN_SUFFIX="txt"
OUT_PREFIX="t"
OUT_SUFFIX="ph1"

DIFF=/usr/bin/diff
EX=./phase1
RM=/bin/rm

STUDENT=$1

# cd $STUDENT/$ASSIGNDIR

echo "forward tests..."
for f in 01 02 03 04 05 06 07 08 09 10
do
    $EX --infile $TEST_DIR/${IN_PREFIX}${f}.${IN_SUFFIX} \
        --outfile /tmp/$$ \
        --forward
    $DIFF /tmp/$$ $TEST_DIR/${OUT_PREFIX}${f}.${OUT_SUFFIX}
    if [ !?* ]
    then
        echo "$f forward: passed"
    else
        echo "$f forward: FAILED"
    fi
    $RM /tmp/$$ 
done

echo
echo "backward tests..."
for f in 01 02 03 04 05 06 07 08 09 10
do
    $EX --infile $TEST_DIR/${OUT_PREFIX}${f}.${OUT_SUFFIX} \
        --outfile /tmp/$$ \
        --backward
    $DIFF /tmp/$$ $TEST_DIR/${IN_PREFIX}${f}.${IN_SUFFIX}
    if [ !?* ]
    then
        echo "$f backward: passed"
    else
        echo "$f backward: FAILED"
    fi
    $RM /tmp/$$ 
done
