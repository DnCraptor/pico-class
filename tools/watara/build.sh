#!/bin/sh
# build.sh - builds the Watara Supervision examples with cc65
# (https://cc65.github.io, package "cc65" in Debian/Ubuntu). The cartridges
# go to ../../sdcard/WATARA; pico-watara lists "*.sv" and "*.bin".
set -e
cd "$(dirname "$0")"
python3 mkgfx.py
mkdir -p ../../sdcard/WATARA
for p in hello sprite catch; do
	cl65 -t supervision -O -o ../../sdcard/WATARA/$p.sv $p.c
done
rm -f *.o
