#!/bin/sh
# build.sh - builds the Gamate examples with cc65 (https://cc65.github.io,
# package "cc65" in Debian/Ubuntu). The cartridges go to ../../sdcard/GAMATE;
# pico-gamate lists "*.bin". fixcart.py writes the checksum the Gamate BIOS
# checks before it starts a cartridge.
set -e
cd "$(dirname "$0")"
python3 mkgfx.py
mkdir -p ../../sdcard/GAMATE
for p in hello sprite catch; do
	cl65 -t gamate -O -o ../../sdcard/GAMATE/$p.bin $p.c
	python3 fixcart.py ../../sdcard/GAMATE/$p.bin
done
rm -f *.o
