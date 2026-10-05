#!/bin/sh
# build.sh - compiles SOUNDS.PAS with Turbo Pascal 5.5 (from the card,
# /freedos/EDU/TP55) inside DOSBox-X, without a window. On a class machine
# the same is just "tpc sounds" in \FREEDOS\EDU\SOUNDS: TP55 is in the PATH.
set -e
cd "$(dirname "$0")"
CARD=$(cd ../../sdcard/freedos/EDU && pwd)
CONF=$(mktemp)
cat > "$CONF" <<EOT
[sdl]
output=surface
[dosbox]
quit warning=false
[autoexec]
mount c "$CARD/SOUNDS"
mount t "$CARD/TP55"
c:
t:\tpc /Tt:\ sounds.pas > build.log
exit
EOT
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy dosbox-x -conf "$CONF" -silent -nogui -fastlaunch >/dev/null 2>&1
rm -f "$CONF"
cat "$CARD/SOUNDS/BUILD.LOG"; rm -f "$CARD/SOUNDS/BUILD.LOG"
