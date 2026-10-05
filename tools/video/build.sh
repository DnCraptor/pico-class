#!/bin/sh
# build.sh - compiles VIDEO.PAS with Turbo Pascal 5.5 (from the card,
# /freedos/EDU/TP55) inside DOSBox-X, without a window. On a class machine
# the same is just "tpc video" in \FREEDOS\EDU\VIDEO: TP55 is in the PATH.
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
mount c "$CARD/VIDEO"
mount t "$CARD/TP55"
c:
t:\tpc /Tt:\ video.pas > build.log
exit
EOT
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy dosbox-x -conf "$CONF" -silent -nogui -fastlaunch >/dev/null 2>&1
rm -f "$CONF"
cat "$CARD/VIDEO/BUILD.LOG"; rm -f "$CARD/VIDEO/BUILD.LOG"
