#!/bin/sh
set -x

OPTS="-O0 -O1 -O2 -O3 -Ofast"
O_FMT="tex"
O_R_TIMES="./results/results-times-i5.tex"
O_R_VALUES="./results/results-values-i5.tex"

##
#echo "test"
#for OPT in $OPTS; do
#	./test${OPT}        
#done

#
echo "test-vector"

NELTOS=20
./test-vector-O0 ${NELTOS} --${O_FMT}-header-times   > ${O_R_TIMES}
for OPT in $OPTS; do
  ./test-vector${OPT} ${NELTOS} --${O_FMT}-times    >> ${O_R_TIMES}
done
./test-vector-O0 ${NELTOS} --${O_FMT}-footer-times  >> ${O_R_TIMES}

NELTOS=17
./test-vector-O0 ${NELTOS} --${O_FMT}-header-values  > ${O_R_VALUES}
for OPT in $OPTS; do
  ./test-vector${OPT} ${NELTOS} --${O_FMT}-values   >> ${O_R_VALUES}
done
./test-vector-O0 ${NELTOS} --${O_FMT}-footer-values >> ${O_R_VALUES}

