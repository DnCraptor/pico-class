#!/bin/sh
# check.sh - draws every graphics mode of VIDEO.PAS in DOSBox-X (an S3 SVGA
# card with VESA BIOS) without a window and puts the pictures side by side
# into modes.png. A test build (tpc /DDUMP) goes through all modes by
# itself and saves the video memory of each to DUMPn.BIN; dump2png.py
# decodes them. RUN.LOG gets what the program printed, with the drawing
# times. Needs dosbox-x, python3 with Pillow and NumPy.
set -e
cd "$(dirname "$0")"
CARD=$(cd ../../sdcard/freedos/EDU && pwd)
WORK=$(mktemp -d)
cp "$CARD/VIDEO/VIDEO.PAS" "$WORK/VIDEOD.PAS"
cat > "$WORK/run.conf" <<EOT
[sdl]
output=surface
[dosbox]
quit warning=false
machine=svga_s3
[cpu]
cycles=fixed 30000
[autoexec]
mount c "$WORK"
mount t "$CARD/TP55"
c:
t:\tpc /Tt:\ /DDUMP videod.pas > build.log
videod > run.log
exit
EOT
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy dosbox-x -conf "$WORK/run.conf" -silent -nogui -fastlaunch >/dev/null 2>&1 || true
python3 dump2png.py "$WORK" modes.png
iconv -f CP866 -t UTF-8 "$WORK/RUN.LOG" | grep -B1 "нарисована" | grep -v -- "--"
rm -rf "$WORK"
