#!/bin/bash

allfiles=`find . -name *.cpp `
INC="-I ../include -I ../poly/include -I ../util/include -I ../sdp/include -I ../psd/include"
LIB="-L ../lib  -lsdp -lpsd -lpoly -lutil  -lgtest -lpthread"

for ff in  $allfiles
do
    bname=${ff##*/}
    bbname=${bname%.cpp}
    echo $bbname

    g++ -std=c++11 $ff $(echo $INC)  $(echo $LIB)   -o $bbname
    ./$bbname

done
