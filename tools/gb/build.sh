#!/bin/sh
# build.sh - builds the Game Boy examples with GBDK-2020 (https://github.com/gbdk-2020/gbdk-2020).
# GBDK is the unpacked GBDK-2020 release (gbdk-linux64.tar.gz); the ROMs go
# to ../../sdcard/GB. Extensions are lowercase: pico-gameboy lists "*.gb".
set -e
GBDK=${GBDK:-$HOME/gbdk}
cd "$(dirname "$0")"
python3 mksprites.py
mkdir -p ../../sdcard/GB
for p in hello sprite catch; do
	# -Wm-yn: the game title in the cartridge header
	"$GBDK/bin/lcc" -Wm-yn"$(echo $p | tr a-z A-Z)" -o ../../sdcard/GB/$p.gb $p.c
done
rm -f ../../sdcard/GB/*.ihx ../../sdcard/GB/*.map ../../sdcard/GB/*.noi ../../sdcard/GB/*.o
