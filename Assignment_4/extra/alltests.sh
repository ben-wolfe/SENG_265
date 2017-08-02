#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"
TEST_DIR=$DIR/test_files
ORIGINAL_DIR=$DIR/original_files

IN_PREFIX="t"
IN_SUFFIX="ph1"

OUT_PREFIX="t"
OUT_SUFFIX="ph2"

DIFF=/usr/bin/diff
RM=/bin/rm

TESTS=('01' '02' '03' '04' '05' '06' '07' '08' '09' '10' '11' '12' '13' '14' '15' '16' '17' '18' '19' '20');

for T in "${TESTS[@]}"
do
    echo "Encoding test $T"
	 ./dll_phase2 \
	 	--infile ${ORIGINAL_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} \
		--outfile ${TEST_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
		--encode
	
	./sll_phase2 \
		--infile ${ORIGINAL_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} \
		--outfile ${TEST_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
		--encode
	echo
done

for T in "${TESTS[@]}"
do
    echo "Decoding test $T"
	 ./dll_phase2 \
	 	--infile ${ORIGINAL_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
		--outfile ${TEST_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} \
		--decode

	./sll_phase2 \
		--infile ${ORIGINAL_DIR}/${OUT_PREFIX}${T}.${OUT_SUFFIX} \
		--outfile ${TEST_DIR}/${IN_PREFIX}${T}.${IN_SUFFIX} \
		--decode
	echo
done
