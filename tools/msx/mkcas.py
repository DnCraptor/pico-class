#!/usr/bin/env python3
"""Pack MSX BASIC programs (plain text) into .CAS tape images.

Each NAME.bas becomes NAME.CAS holding one ASCII-format file, the same
thing SAVE"CAS:NAME",A writes on a real MSX. Load it with RUN"CAS:".

Usage: python3 mkcas.py OUTDIR FILE.bas [FILE.bas ...]
"""
import os
import sys

CAS_HEADER = bytes([0x1F, 0xA6, 0xDE, 0xBA, 0xCC, 0x13, 0x7D, 0x74])
ASCII_ID = bytes([0xEA] * 10)
EOF_MARK = 0x1A


def make_cas(name, text):
    data = text.replace("\r\n", "\n").rstrip("\n").replace("\n", "\r\n") + "\r\n"
    data = data.encode("ascii")
    tape_name = name.upper()[:6].ljust(6).encode("ascii")
    out = bytearray(CAS_HEADER + ASCII_ID + tape_name)
    # Data blocks of 256 bytes; the last one is padded with EOF marks.
    # If the text ends exactly on a block boundary, one more block of
    # EOF marks follows, so the loader always finds the end.
    pos = 0
    while True:
        chunk = data[pos:pos + 256]
        pos += 256
        out += CAS_HEADER + chunk + bytes([EOF_MARK] * (256 - len(chunk)))
        if len(chunk) < 256:
            break
    return bytes(out)


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    outdir = sys.argv[1]
    for src in sys.argv[2:]:
        name = os.path.splitext(os.path.basename(src))[0]
        with open(src, encoding="ascii") as f:
            cas = make_cas(name, f.read())
        dst = os.path.join(outdir, name.upper() + ".CAS")
        with open(dst, "wb") as f:
            f.write(cas)
        print(f"{dst}: {len(cas)} bytes")


if __name__ == "__main__":
    main()
