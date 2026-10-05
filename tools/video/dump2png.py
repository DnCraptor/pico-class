#!/usr/bin/env python3
"""dump2png.py - turns the video memory dumps DUMP2.BIN..DUMP9.BIN written by
a test build of VIDEO.PAS (tpc /DDUMP) into one picture: every graphics mode
side by side, scaled to the same size. Usage: dump2png.py DUMPDIR OUT.PNG
Needs Pillow and NumPy."""
import sys
import numpy as np
from PIL import Image, ImageDraw

D = sys.argv[1].rstrip('/') + '/'
EGA = [(0, 0, 0), (0, 0, 170), (0, 170, 0), (0, 170, 170), (170, 0, 0), (170, 0, 170),
       (170, 85, 0), (170, 170, 170), (85, 85, 85), (85, 85, 255), (85, 255, 85),
       (85, 255, 255), (255, 85, 85), (255, 85, 255), (255, 255, 85), (255, 255, 255)]
CGA = [(0, 0, 0), (0, 170, 170), (170, 0, 170), (170, 170, 170)]   # palette 1


def raw(k):
    return np.frombuffer(open(D + 'DUMP%s.BIN' % k, 'rb').read(), np.uint8)


def cga(k, bpp, w, h):
    d = raw(k)
    out = np.zeros((h, w), int)
    for y in range(h):
        o = (y & 1) * 0x2000 + (y >> 1) * 80
        bits = np.unpackbits(d[o:o + 80])
        out[y] = bits[0::2] * 2 + bits[1::2] if bpp == 2 else bits
    return out


def planar(k, w, h):
    d = raw(k)
    n = w // 8 * h
    c = np.zeros(w * h, int)
    for p in range(4):
        c |= np.unpackbits(d[p * n:(p + 1) * n]).astype(int) << p
    return np.array(EGA)[c.reshape(h, w)]


PAL = np.zeros((256, 3), int)
PAL[:16] = EGA
i = 16
for r in range(6):
    for g in range(6):
        for b in range(6):
            PAL[i] = (r * 63 // 5 * 4, g * 63 // 5 * 4, b * 63 // 5 * 4)
            i += 1
for k in range(24):
    PAL[232 + k] = (k * 63 // 23 * 4,) * 3

mono = np.array([(0, 0, 0), (255, 255, 255)])
pics = [
    ('CGA 320x200, 4', np.array(CGA)[cga('2', 2, 320, 200)]),
    ('CGA 640x200, 2', mono[cga('3', 1, 640, 200)]),
    ('EGA 320x200, 16', planar('4', 320, 200)),
    ('EGA 640x350, 16', planar('5', 640, 350)),
    ('MCGA 640x480, 2', mono[np.unpackbits(raw('6')).reshape(480, 640)]),
    ('VGA 640x480, 16', planar('7', 640, 480)),
    ('320x200, 256', PAL[raw('8').reshape(200, 320)]),
    ('SVGA 640x400, 256', PAL[raw('9').reshape(400, 640)]),
]

TW, TH, CAP = 320, 240, 16
sheet = Image.new('RGB', (TW * 4, (TH + CAP) * 2), (255, 255, 255))
draw = ImageDraw.Draw(sheet)
for n, (name, a) in enumerate(pics):
    im = Image.fromarray(a.astype(np.uint8)).resize((TW, TH), Image.BOX)
    x, y = (n % 4) * TW, (n // 4) * (TH + CAP)
    sheet.paste(im, (x, y + CAP))
    draw.text((x + 4, y + 2), name, fill=(0, 0, 0))
sheet.save(sys.argv[2], optimize=True)
