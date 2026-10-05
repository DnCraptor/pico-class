#!/bin/sh
# build.sh - builds the Mega Drive examples with SGDK (https://github.com/Stephane-D/SGDK).
# GDK is the SGDK checkout with libmd.a and the host tools built for Linux
# (cmake + make in its root, with PREFIX set to the m68k compiler);
# PREFIX is the m68k gcc prefix (m68k-elf- or the distribution's
# m68k-linux-gnu-). The ROMs go to ../../sdcard/MD.
set -e
GDK=${GDK:-$HOME/SGDK}
PREFIX=${PREFIX:-m68k-linux-gnu-}
cd "$(dirname "$0")"
HERE=$(pwd)
python3 mksprites.py
mkdir -p ../../sdcard/MD
TMP=$(mktemp -d)
for p in hello sprite catch; do
	mkdir -p "$TMP/$p/src" "$TMP/$p/inc" "$TMP/$p/res"
	cp $p.c "$TMP/$p/src/main.c"
	cp beep.h sprite_gfx.h catch_gfx.h "$TMP/$p/inc/"
	(cd "$TMP/$p" && PATH="$GDK/bin:$PATH" make -s -f "$GDK/makefile.gen" GDK="$GDK" \
		PREFIX="$PREFIX" CC="${PREFIX}gcc -Wl,--build-id=none" release >/dev/null)
	cp "$TMP/$p/out/rom.bin" "$HERE/../../sdcard/MD/$p.bin"
done
rm -rf "$TMP"
