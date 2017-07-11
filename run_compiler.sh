#!/bin/bash
input=in_asm.txt
output=a.out
if [ $# -ge 1 ]; then
    input=$1;
fi
if [ $# -ge 2 ]; then
    output=$2;
fi

make
./etapa6 $input asm.s
gcc asm.s -o $output