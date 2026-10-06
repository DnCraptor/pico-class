#!/usr/bin/env python3
# fixcart.py - writes the cartridge checksum into a Gamate image made by
# cc65. The Gamate BIOS adds up the bytes at 7000-7FFF (file offset 0x1000,
# 0x1000 bytes) and starts the cartridge only if the sum matches the first
# two bytes of the header. Same as util/gamate/gamate-fixcart.c of cc65.
import sys
for fn in sys.argv[1:]:
    with open(fn, 'r+b') as f:
        data = bytearray(f.read())
        n = sum(data[0x1000:0x2000]) & 0xFFFF
        data[0] = n & 0xFF
        data[1] = n >> 8
        f.seek(0)
        f.write(data)
