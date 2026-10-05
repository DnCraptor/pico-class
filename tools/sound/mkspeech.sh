#!/bin/sh
# mkspeech.sh - makes SPEECH.RAW for SOUNDS.EXE: a Russian sentence spoken by
# eSpeak NG, as 8000 Hz, 8-bit unsigned mono samples (what a DAC takes).
# Needs espeak-ng and sox (Debian/Ubuntu packages of the same names);
# made with eSpeak NG 1.51 and SoX 14.4.
set -e
cd "$(dirname "$0")"
OUT=../../sdcard/freedos/EDU/SOUNDS/SPEECH.RAW
espeak-ng -v ru -s 150 -w speech.wav "Привет! Это цифровой звук."
# -D: no dither, so the result is the same every time
sox -D speech.wav -r 8000 -c 1 -b 8 -e unsigned-integer -t raw "$OUT" vol 0.9
rm -f speech.wav
