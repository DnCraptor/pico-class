#!/bin/sh
# mkspeech.sh - makes SPEECH.RAW for SOUNDS.EXE: a short Russian phrase spoken
# by the RHVoice synthesizer (voice Aleksandr), as 8000 Hz, 8-bit unsigned
# mono samples (what a DAC takes).
# Needs RHVoice with the Russian voices and sox (Debian/Ubuntu packages
# rhvoice, rhvoice-russian, sox); made with RHVoice 1.8.0 and SoX 14.4.
set -e
cd "$(dirname "$0")"
OUT=../../sdcard/freedos/EDU/SOUNDS/SPEECH.RAW
echo "Привет! Раз, два, три." | RHVoice-test -p aleksandr -r 80 -o speech.wav
# trim the silence at both ends, cut the rumble, even out the loudness
# (quiet sounds up, loud ones down) and fill the 8 bits; -D: no dither,
# so the result is the same every time
sox -D speech.wav -r 8000 -c 1 -b 8 -e unsigned-integer -t raw "$OUT" \
  silence 1 0.02 0.5% reverse silence 1 0.02 0.5% reverse \
  highpass 120 compand 0.003,0.12 6:-70,-70,-50,-30,-20,-9,0,-5 -3 norm -1
rm -f speech.wav
