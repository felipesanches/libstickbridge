#!/bin/bash

rm -rf ./build
mkdir ./build

echo ------- yosys
yosys -p 'synth_ice40 -top top -blif ./build/example.blif' example.v | grep -iE 'Warning|Error' || exit

echo ------- arachne-pnr
# arachne-pnr -d 8k -P ct256 -o ./build/example.asc -p example.pcf ./build/example.blif 2> /dev/null || exit
arachne-pnr -d 1k -o ./build/example.asc -p example.pcf ./build/example.blif 2> /dev/null || exit



echo ------- icepack
icepack ./build/example.asc ./build/example.bin || exit

# echo ------- icetime
# icetime ./build/example.asc -d hx8k || exit

if [ "x$1" = x"-prog" ]; then
  echo ------- iceprog
  iceprog build/example.bin
fi

