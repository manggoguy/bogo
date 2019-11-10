#!/bin/bash
BOGOLIB=/home/mok/바탕화면/llvm/bogo/build/lib

for INPUT_PROGRAM in $@
do
    ORIG_BC=$INPUT_PROGRAM".bc"
    TRANS_BC=$INPUT_PROGRAM"_out.bc"
    PROG_S=$INPUT_PROGRAM".s"
    PROG_OUT=$INPUT_PROGRAM".o"
    clang -emit-llvm -c $INPUT_PROGRAM -o $ORIG_BC
    opt -load=$BOGOLIB/libllmpx_me.so $ORIG_BC --llmpx -o $TRANS_BC
    llc $TRANS_BC -o $PROG_S
    clang $PROG_S -o $PROG_OUT $BOGOLIB/libmpxrt.a $BOGOLIB/libTx.a -lstdc++ -lm -ldl -lpthread
done
