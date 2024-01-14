#!/bin/sh
export LC_ALL=en
BASEPATH=~/Desktop/ERGASIES/GrammaticalRuler/
PROGRAM=$BASEPATH/GrammaticalRuler
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
GENOMES=500
GENERATIONS=500
DATAFILE=$1
rm -f $1.out
 $PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -n 2000 -r 1 -l 100 -g 50 -d 20 >> $1.out
