#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"
TEST_DIR=$DIR/test_files
ORIGINAL_DIR=$DIR/original_files

IN_PREFIX="t"
IN_SUFFIX="ph1"

OUT_PREFIX="t"
OUT_SUFFIX="ph2"

DIFF=/usr/bin/diff

TESTS=('01' '02' '03' '04' '05' '06' '07' '08' '09' '10' \
		 '11' '12' '13' '14' '15' '16' '17' '18' '19' '20');

for T in "${TESTS[@]}"
do
    ./phase2.py \
	 	--infile ${ORIGINAL_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} \
		--outfile ${TEST_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
		--encode

    $DIFF ${TEST_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} ${ORIGINAL_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX}

    if [ $? -eq 0 ]
    then
        echo "$TESTNUM encode: passed"
    else
        echo "$TESTNUM encode: FAILED"
    fi
done


for T in "${TESTS[@]}"
do
    ./phase2.py \
	 	--infile ${ORIGINAL_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
		--outfile ${TEST_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} \
		--decode
    
	 $DIFF ${TEST_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} ${ORIGINAL_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX}
    
	 if [ $? -eq 0 ]
    then
        echo "$TESTNUM decode: passed"
    else
        echo "$TESTNUM decode: FAILED"
    fi
done
