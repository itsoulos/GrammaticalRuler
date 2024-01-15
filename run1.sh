#!/bin/sh
export LC_ALL=en
BASEPATH=~/Desktop/ERGASIES/GrammaticalRuler/
PROGRAM=$BASEPATH/GrammaticalRuler
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
GENOMES=500
GENERATIONS=2000
DATAFILE=$1
rm -f $1.out
INSTANCES=10
for i in $(seq 1 $INSTANCES)
do
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -t $DATAPATH/$DATAFILE.test  -n $GENERATIONS -r $i -l 100 -g 50 -d 20 >> $1.out
done
 grep "Testerror" $1.out > xx
 mv xx $1.out
echo -n "Test Error: "
cat $1.out | awk '{ sum += $2; n++ } END { if (n > 0) print sum / n; }'
echo -n "Class Error: "
cat $1.out | awk '{ sum += $4; n++ } END { if (n > 0) print sum / n; }'
