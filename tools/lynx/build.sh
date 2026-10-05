#!/bin/sh
# build.sh - builds the Atari Lynx examples with cc65 (https://cc65.github.io,
# package "cc65" in Debian/Ubuntu). The cartridges go to ../../sdcard/LYNX.
# Extensions are lowercase: pico-lynx lists "*.lnx".
set -e
cd "$(dirname "$0")"
python3 mksprites.py
mkdir -p ../../sdcard/LYNX
for p in hello sprite catch; do
	cl65 -t lynx -O -o ../../sdcard/LYNX/$p.lnx $p.c
done
rm -f *.o
