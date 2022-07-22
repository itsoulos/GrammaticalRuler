#!/bin/sh
BASEPATH=~/Desktop/ERGASIES/GrammaticalRuler/
PROGRAM=$BASEPATH/GrammaticalRuler
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
GENOMES=500
GENERATIONS=500
DATAFILE=$1
rm -f $1.out
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 1 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 1"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 2 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 2"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 3 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 3"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 4 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 4"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 5 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 5"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 6 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 6"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 7 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 7"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 8 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 8"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 9 -l 100 -g 50 -d 20 >> $1.out
 echo "$1 finish .... 9"
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 10 -l 100 -g 50 -d 20 >> $1.out
echo -n "Test Error: "
cat $1.out | awk '{ sum += $2; n++ } END { if (n > 0) print sum / n; }'
echo -n "Class Error: "
cat $1.out | awk '{ sum += $4; n++ } END { if (n > 0) print sum / n; }'
