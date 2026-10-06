#!/bin/sh
# build.sh - builds WonderSnake (Tomasz Slanina, GPL-3.0) for pico-wonderswan
# from its original source with free tools, and puts the cartridge into
# ../../sdcard/WS/wondersnake.wsc.
#
#   WONDERSNAKE - a clone of https://github.com/tslanina/Retro-WonderSwanColor-Wondersnake
#                 (checked with 56309a6)
#   JWASM_BIN   - the JWasm assembler (https://github.com/Baron-von-Riedesel/JWasm,
#                 built with "make -f GccUnix.mak"; checked with 7f6f32e).
#                 Not called JWASM: JWasm reads options from an environment
#                 variable of that name.
#
# The original is built with Borland Turbo Assembler and Turbo Link. Two
# changes let JWasm assemble it, made in a temporary copy:
#   - the TASM directive JUMPS is commented out: JWasm extends conditional
#     jumps that are out of range by itself;
#   - "levels\" in the include paths becomes "levels/".
# com2ws.c from the same repository turns ws.com into a 4 Mbit cartridge.
set -e
cd "$(dirname "$0")"
: "${WONDERSNAKE:?set WONDERSNAKE to the WonderSnake source directory}"
: "${JWASM_BIN:?set JWASM_BIN to the jwasm executable}"
OUT="$(pwd)/../../sdcard/WS"
TMP=$(mktemp -d)
cp -r "$WONDERSNAKE"/. "$TMP"
cd "$TMP"
sed -i -e 's/^JUMPS/;JUMPS/' -e 's#levels\\#levels/#' ws.asm
"$JWASM_BIN" -bin -Fo=ws.com ws.asm
cc -O2 -o c2w com2ws/com2ws.c
mkdir -p "$OUT"
./c2w ws.com "$OUT/wondersnake.wsc"
cd /
rm -rf "$TMP"
