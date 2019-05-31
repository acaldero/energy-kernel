#!/bin/sh
set -x

OPTS="-O0 -O1 -O2 -O3 -Ofast -g"

for OPT in $OPTS; do

	clang++ -I. ${OPT} -o Memoization_uthash.o      -c Memoization_uthash.cc
	clang++ -I. ${OPT} -o compute-vs-find.o         -c compute-vs-find.cc        -DOPT=${OPT}
	clang++ -I. ${OPT} -o compute-vs-find-vector.o  -c compute-vs-find-vector.cc -DOPT=${OPT}
	clang++ -I. ${OPT} -o test${OPT}        compute-vs-find.o        Memoization_uthash.o -lm -lz
	clang++ -I. ${OPT} -o test-vector${OPT} compute-vs-find-vector.o Memoization_uthash.o -lm -lz

done

