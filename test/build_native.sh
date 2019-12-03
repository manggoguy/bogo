#!/bin/bash
BOGOLIB=/home/mok/project/bogo/build/lib

for INPUT_PROGRAM in $@
do
    ORIG_BC=$INPUT_PROGRAM".bc"
    TRANS_BC=$INPUT_PROGRAM"_out.bc"
    PROG_S=$INPUT_PROGRAM".s"
    PROG_OUT=$INPUT_PROGRAM".o"
    clang -emit-llvm -c $INPUT_PROGRAM -o $ORIG_BC -mrtm
    opt -load=$BOGOLIB/libllmpx.so $ORIG_BC --llmpx -o $TRANS_BC
    llc $TRANS_BC -o $PROG_S
    clang $PROG_S -o $PROG_OUT $BOGOLIB/libmpx.a $BOGOLIB/liblock.a -mrtm -lstdc++ -lm -ldl -lpthread
done
