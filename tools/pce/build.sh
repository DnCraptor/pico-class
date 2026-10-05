#!/bin/sh
# build.sh - builds the PCE examples with HuC (https://github.com/pce-devel/huc).
# HUC is the HuC checkout with built tools (make in its root); the ROMs go
# to ../../sdcard/PCE. Extensions are lowercase: pico-pce lists only "*.pce".
set -e
HUC=${HUC:-$HOME/huc}
export PCE_INCLUDE="$HUC/include/huc"
export PCE_PCEAS="$HUC/bin/pceas"
cd "$(dirname "$0")"
python3 mksprites.py
mkdir -p ../../sdcard/PCE
for p in hello sprite catch; do
	"$HUC/bin/huc" -fno-recursive -msmall $p.c
	cp $p.pce ../../sdcard/PCE/$p.pce
	rm -f $p.s $p.lst $p.sym $p.pce
done
