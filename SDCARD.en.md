# Common SD-card content

[Русская версия](SDCARD.md) · English

> This is a translation of the Russian [`SDCARD.md`](SDCARD.md), made from commit `6253b68`. The Russian text is the reference: if the two differ, the Russian one is newer. File names, folder names and program titles are given as they appear on the card; descriptions of Russian-language programs keep their Russian titles with a translation in brackets.

State as of 2026-10-13. The `sdcard/` folder at the root of the repository is the part of the class SD card common to all boards: MOS2 and its applications, FreeDOS for murm386, games and learning programs, files for workstation acceptance. This file describes what is in it, where it comes from and how to check its integrity. When the content changes, this file and [`SHA256SUMS`](SHA256SUMS) are updated.

## 1. Preparing the card

1. Format the card as FAT32 with a 4 KB cluster size — see the instructions for your board (for the Olimex PICO-PC — [`hardware/olimex-pico-pc/README.en.md`](hardware/olimex-pico-pc/README.en.md), section 3).
2. Copy the **contents** of the common [`sdcard/`](sdcard/) folder to the root of the card.
3. Then copy the **contents** of your board's `sdcard/` folder on top — for example, [`hardware/olimex-pico-pc/sdcard/`](hardware/olimex-pico-pc/sdcard/) for the Olimex PICO-PC. It holds the firmwares built for that board (emulators in `/emu`); where names match, the board's files replace the common ones.
4. Add what the repository does not include — for example, the disk image for the Mac 128K (section 18) — and test the card on one workstation.
5. **Take a full image of the finished card** (the "master image") with a disk-imaging program, for example Win32 Disk Imager (the Read button) on Windows or `dd` on Linux, and keep it on the teacher's computer.

**Before every lesson** the master image is written to the cards of all workstations (Win32 Disk Imager — the Write button). It is fine for the card to get messed up during lessons: children save, delete and change emulator settings, and writing the image returns all stations to the same starting state in one go, without sorting out what was changed where. The target card must be at least as large as the one the image was taken from; writing takes longer the larger the card, so it is convenient to use the smallest suitable size for the master card. When the card content changes, the master image is taken again.

**A pupil's own card.** A pupil may bring their own SD card: the content of the master card is copied onto it — as files and folders, not as a whole image, so as not to erase what is already on the card — and in the lesson the pupil works with their own card instead of the workstation's. Then everything they did — LogoWriter pages, BASIC and Pascal programs, saved games — goes home with them and can be continued in free time in class or at home, if they have the same board there. Before first use, format the card the same way as the class cards (FAT32, 4 KB cluster); writing the master image before the lesson does not apply to personal cards, their content is the owner's business. Which programs without an open licence (they are marked in the tables of this file and in [`hardware/olimex-pico-pc/MANIFEST.md`](hardware/olimex-pico-pc/MANIFEST.md)) and which things not included in the repository (for example, the Mac disk image from section 18) may be copied to cards that leave the class is decided by the class leader.

## 2. Contents

| Path on the card | Origin | Differences from the source |
|---|---|---|
| `/mos2/*` (applications, `bin/`, `lib/libmos.a`) | `apps/compiled` from murmulator-os2, `main` = `a77484c` (2026-10-12), version 2.3.3 | byte-for-byte identical |
| `/mos2/bin/tcc`, `/mos2/include/*`, `/mos2/src/*`, `/mos2/.tcc-install.json` | the TCC C compiler for MOS2 with headers, library and sources — output of `tools/install_sdk.py --with-tcc` from murmulator-os2 (`a77484c`); `.tcc-install.json` lists the installed files with their SHA256 for the next update | text files have CRLF line endings, otherwise identical; TCC is LGPL 2.1 (`/mos2/src/tcc/COPYING`) |
| `/mos2/config.sys` | same | paths brought to one letter case (`/mos2`), CRLF line endings |
| `/mos2/UPSTREAM-README.md` | same, `apps/compiled/UPSTREAM-README.md` (the root `README.md` of murmulator-os2) | byte-for-byte identical |
| `/mos2/README.md` | written for this repository; since version 2.3.3 it is also in murmulator-os2 (`apps/compiled/README.md`), byte-for-byte identical | MOS2 quick guide for the PCp2 (in Russian); CP866 encoding so that the file can be read in MOS2 (`type`, `mcview`) |
| `/test/kbdtest.bas` | this repository | keyboard test utility for MOS2 (Stefan's BASIC) |
| `/test/edit.txt` | this repository | file for testing the viewer, the editor and writing to the SD card |
| `/test/snd_mono.wav` | this repository | 8000 Hz, mono, 16 bit: three tones of 440/660/880 Hz at −12 dBFS |
| `/test/snd_lr.wav` | this repository | 8000 Hz, stereo, 16 bit: 440 Hz left, 660 Hz right, 880 Hz in both channels, −12 dBFS |
| `/freedos/bin/*`, `/freedos/configs/*`, `/freedos/nls/*`, `/freedos/version.fdi` | FreeDOS 7.1, a selection of files (`version.fdi` says 1.4 of 2025-04-02 — the internal version of the original kernel) | for booting murm386 straight from the SD card |
| `/freedos/bin/MODE.COM` | MODE from FreeDOS (Eric Auer), file dated 2015-11-25; GPL | sets up the COM port (speed, byte format) and the screen; needed by `\DOSPPP\PPP9600.BAT` for connecting through the ESP32 modem (section 22) |
| `/freedos/bin/debug.com`, `/freedos/bin/debug.lic` | Debug/X 2.51 — [Baron-von-Riedesel/DOS-debug](https://github.com/Baron-von-Riedesel/DOS-debug) (`4ac7cd6`), `DEBUG.COM` built with JWasm from `src/DEBUG.ASM` without changes; licence MIT (Paul Vojta), Japheth's additions are public domain; the terms are in `debug.lic` | debugger, a free counterpart of MS-DOS DEBUG: memory, ports, assembler and disassembler; material of [teaching module 24](program/modules/24-bios-dos.md) |
| `/fdconfig.sys`, `/FDAUTO.BAT` | FreeDOS 7.1 configuration | adapted for murm386: the shell is Volkov Commander, `\FREEDOS\EDU\TP55` and `\FREEDOS\EDU\TC201` added to `PATH`; CRLF line endings |
| `/freedos/VC.COM` | Volkov Commander 4.05 | file manager, started from `FDAUTO.BAT` |
| `/freedos/SI.EXE` | Norton System Information | performance index; useful when checking the murm386 clock |
| `/freedos/LTEMM.EXE` | LTEMM r01 — EMS driver for the Lo-tech 2MB EMS board (A. Tsourikov, 1988; Lo-tech rework), [lo-tech.co.uk](https://www.lo-tech.co.uk/wiki/LTEMM.EXE) | murm386 emulates this board (window at D000). In `/fdconfig.sys` the line that loads it is commented out for now (`REM`); to enable EMS, remove `REM` and reboot. Source code — BSD 3-Clause, the Lo-tech build — Standard Lo-tech License |
| `/386/bios.bin`, `/386/vgabios.bin` | SeaBIOS and SeaVGABIOS, build `rel-1.17.0-7-g106549a4-dirty` (2026-03-27); LGPL v3 | guest BIOS for murm386 builds with the 386 core (data folder `/386`; the 286 build uses `/286`). By default murm386 uses its built-in BIOS and boots FreeDOS from the SD card; SeaBIOS is selected in the Disk Manager (Win+F12), BIOS line, a reboot is needed |
| `/386/fdd0free.img` | 1.44 MB floppy image with FreeDOS 7.1 (the same selection as in `/freedos`: kernel, `COMMAND.COM`, `bin`, `nls`, `configs`; `FDCONFIG.SYS` and `FDAUTO.BAT` as on the SD card), plus Volkov Commander, CuteMouse, LTEMM, CheckIt 1; for [teaching module 24](program/modules/24-bios-dos.md) `\FREEDOS\BIN\DEBUG.COM` (with `DEBUG.LIC`) and a `\BIOS` folder were added — the built `PAMYAT.EXE`, `PORTY.EXE` (Turbo Pascal), `TRI.EXE` (Turbo C) and their sources from `/freedos/EDU/BIOS` | guest OS for murm386 with the 386 core; the floppy is attached in the Disk Manager (Win+F12), FDD-0 line. FreeDOS is GPL; CheckIt is a commercial program |
| `/386/fdd0dr81.img` | 1.44 MB floppy image with DR-DOS 8.1 (DrDOS, Inc., 2005): `COMMAND.COM`, `SYS.COM`, `MEM.EXE` and others, FDXXMS, UMBPCI, CuteMouse, LTEMM, Volkov Commander, Norton SI, CheckIt 1 and 3 | same, a different guest OS. DR-DOS 8.1 is proprietary ("All rights reserved"), CheckIt and Norton SI are commercial programs; the image will be removed from the repository if the rights holder objects |
| `/NES/*.nes` | free homebrew games for pico-nes, see section 3 | see section 3 |
| `/ZX/*` | free programs and games for pico-speccy, see section 5 | see section 5 |
| `/apple/*.DSK` | disk images for murmapple (Apple IIe), see section 6 | see section 6 |
| `/z26/*.bin` | free homebrew games for pico-z26 (Atari 2600), see section 7 | see section 7 |
| `/atari800/*` | free programs for atari800 (Atari 400/800, XL/XE, 5200), see section 8 | see section 8 |
| `/c64/*.PRG` | free programs for murmc64 (Commodore 64), see section 9 | see section 9 |
| `/micro/*` | disk images and extra ROMs for frank-micro (BBC Micro), see section 10 | see section 10 |
| `/MSX/*` | BIOS ROMs, tapes with lesson programs and free games for frank-msx (MSX), see section 11 | see section 11 |
| `/korvet/*.KDI` | disk images for the Korvet (emu80v4 emulator, `korvet` build), see section 12 | see section 12 |
| `/vector06c/*` | free games for the Vector-06C (emu80v4 emulator, `v06c` build), see section 13 | see section 13 |
| `/PCE/*.pce` | programs for pico-pce (PC Engine), see section 14 | see section 14 |
| `/MD/*.bin` | programs for pico-genesisPlus (Sega Mega Drive / Genesis), see section 15 | see section 15 |
| `/GB/*.gb` | programs for pico-gameboy (Nintendo Game Boy), see section 16 | see section 16 |
| `/LYNX/*.lnx` | programs for pico-lynx (Atari Lynx), see section 17 | see section 17 |
| `/GAMATE/*.bin` | programs for pico-gamate (Gamate), see section 19 | see section 19 |
| `/WATARA/*.sv` | programs for pico-watara (Watara Supervision), see section 20 | see section 20 |
| `/WS/*.wsc` | a free game for pico-wonderswan (WonderSwan Color), see section 21 | see section 21 |
| `/m128/umac0.img` | disk image for pico-mac (Mac 128K); **not part of the reference card**, downloaded separately, see section 18 | see section 18 |
| `/freedos/GAMES/*`, `/freedos/EDU/*` | free programs for murm386, see section 4 | see section 4 |
| `/dosppp/*`, `/MTCP/*`, `/386/Zimodem-4.0.3.2-WROVER-USB-UART0.bin` | networking for murm386: PPP driver, TCP/IP programs and the ESP32 Wi-Fi modem firmware, see section 22 | see section 22 |

The `/mos2/README.md` guide was written for the MOS2 build for the Olimex PICO-PC (PCp2): the video modes and the sound and gamepad pins in it are those of that board.

### TCC compiler (`/mos2`)

MOS2 includes the TCC (TinyC Compiler) C compiler: `/mos2/bin/tcc`, headers in `/mos2/include`, the library `/mos2/lib/libmos.a`, sources in `/mos2/src/tcc`. Small programs compile without PSRAM (QSPI) too. TCC compiling itself needs more memory than is available without PSRAM:

```
cd /mos2/src/tcc
/mos2/bin/tcc @selfhost-compile.rsp
/mos2/bin/tcc @selfhost-link.rsp
/mos2/bin/tcc-self
```

## 3. Games for pico-nes (`/NES`)

pico-nes looks for games in the `/NES` folder of the SD card (extension `.nes`). Only games whose authors allow free distribution are included. Commercial NES cartridge images are not part of the kit.

| File | Game | Genre | Source | Licence | Mapper, size |
|---|---|---|---|---|---|
| `croom.nes` | Concentration Room | "Memory", matching card pairs | [pinobatch/croom-nes](https://github.com/pinobatch/croom-nes) `ed19c3c`, built from source | GPLv3; ROM copies may be distributed without the source | 0, 24 KB |
| `thwaite.nes` | Thwaite | defending towns from missiles | [pinobatch/thwaite-nes](https://github.com/pinobatch/thwaite-nes) `00e3674`, built from source | GPLv3 | 0, 40 KB |
| `rfk.nes` | robotfindskitten | "zen simulation", English text | [pinobatch/rfk-nes](https://github.com/pinobatch/rfk-nes) `b9764ce`, built from source | zlib | 0, 32 KB |
| `nova.nes` | Nova the Squirrel | puzzle platformer, English dialogue | [NovaSquirrel/NovaTheSquirrel](https://github.com/NovaSquirrel/NovaTheSquirrel) `e9e79ae`, built from source | code GPLv3, graphics and levels CC BY-NC-SA 4.0 | 1 (MMC1), 256 KB |
| `jetpaco.nes` | Jet Paco | jetpack platformer | [mojontwins/MK1_NES](https://github.com/mojontwins/MK1_NES) `ec3fbcc`, `examples/` | the authors allow free distribution of the games in `examples` | 0, 40 KB |
| `lala.nes` | Lala the Magical: Prologue | platformer | same | same | 0, 40 KB |
| `bootee.nes` | Bootèe | platformer | same | same | 0, 40 KB |
| `cheril_bosque.nes` | Cheril of the Bosque | adventure platformer | same | same | 0, 40 KB |
| `cheril_perils.nes` | Cheril Perils Classic | platformer | same | same | 0, 40 KB |
| `cheril_goddess.nes` | Cheril the Goddess | platformer | same | same | 3 (CNROM), 64 KB |
| `cheril_writer.nes` | Cheril the Writer | platformer | same | same | 3 (CNROM), 64 KB |
| `espitene.nes` | Espitene | platformer | same | same | 3 (CNROM), 64 KB |
| `cadaverion.nes` | Cadàveriön | dungeon puzzle | same | same | 0, 40 KB |
| `dveelng.nes` | D'Veel'Ng | top-down action | same | same | 0, 40 KB |

All mappers (0, 1, 3) are supported by pico-nes. The games by Damian Yerrick and Nova the Squirrel were built from the source of the listed commits (cc65 2.19); the Mojon Twins games were taken ready-made from the repository. The files were renamed to short names without apostrophes and diacritics.

## 4. Programs for murm386 (`/freedos/GAMES`, `/freedos/EDU`)

The selection is sized for the speed of murm386 without PSRAM: roughly 2–3 PC XTs, that is, the level of a Turbo XT or an 8 MHz PC AT. Without PSRAM it cannot be faster because of frequent swapping. The EGA128 and VGA128 modes (an extra 128 KB of SRAM) and MCGA (another 64 KB) help reduce cache misses. Programs that need PSRAM are not included — they can be added separately. On workstations with PSRAM the murm386 builds from `/emu/psram` can be used (see `hardware/olimex-pico-pc/MANIFEST.md`): their guest memory is entirely in PSRAM, no swapping is needed.

Selection criteria: XT/AT-level speed; text, CGA, EGA or VGA/MCGA video. murm386 supports XMS, so programs that use extended memory are suitable too — the limit is speed, not the amount of memory. The source and distribution terms of each program are given in the table. Programs are started from Volkov Commander: go into the folder, press Enter on the `.EXE`/`.COM`.

| Folder | Program | What it is | Video | Source | Distribution terms |
|---|---|---|---|---|---|
| `/freedos/GAMES/KROZ/KINGDOM` | Kingdom of Kroz (1987) | Apogee's first game, a text-mode arcade maze | text | [tangentforks/kroz](https://github.com/tangentforks/kroz) `5d080fb` — Apogee's official 2009 freeware release | freeware: free to play and distribute; selling and reusing the materials is not allowed (`/freedos/GAMES/KROZ/KROZ.TXT`) |
| `/freedos/GAMES/KROZ/KINGDOM2`, `CAVERNS2`, `DUNGEON2`, `RETURN`, `TEMPLE`, `CRUSADE`, `LOST` | the other Kroz episodes (1990) | same | text | same | same |
| `/freedos/GAMES/TYPEFAST` | TypeFast | typing tutor as a game: type the falling words in time | text | [clasqm/freedos-repo](https://github.com/clasqm/freedos-repo) `5254279` | the authors claimed no copyright and ask to keep the comments in the source |
| `/freedos/GAMES/DRMIND` | Dr. Mind Lite | "Bulls and Cows" (Mastermind) in colour | VGA/MCGA | same | CC BY-ND 4.0 |
| `/freedos/GAMES/DROBOS` | Drobos | "Daleks": lead the hero away from robots, making them crash into each other | EGA | same | free distribution, no selling |
| `/freedos/GAMES/SHUFFLE` | Shuffle V | the "15 puzzle" | text | same | freeware for non-commercial use (J. R. Ferguson). **Known issue:** in murm386 v1.19 it hangs at start in all three builds (Turbo Pascal, Turbo Vision-style interface; the file is packed with LZEXE 0.91) |
| `/freedos/GAMES/HANOI` | Towers of Hanoi | the classic recursive puzzle | text | same | same |
| `/freedos/GAMES/LINES` | Color Lines (Gamos, 1992) | "Lines": make rows of five balls of one colour; rules in `LINES.TXT` | EGA | downloaded manually | not stated |
| `/freedos/GAMES/LIFE` | Life2 | Conway's Game of Life | text | same | freeware according to freedos-repo |
| `/freedos/GAMES/NERO5` | Nero 5 | chess | text | same | same |
| `/freedos/EDU/ELAN` | Elan-1 1.5 | a teaching programming language (University of Nijmegen), with turtle graphics and Karel the robot | text, graphics | same | may be copied and distributed free of charge, complete and with the copyright notice (`READ.1ST`). Needs the `ANSI.SYS` driver |
| `/freedos/EDU/robotlnd` | Robotlandia (Yu. Pervin) | a Russian computer-science course for primary-school children: keyboard, robot "executors", puzzle games. Incomplete in this selection: `MENU.EXE` is only a demo of the picture menu; "Pravilka" (`PRAV.EXE`) does not start — a picture file is missing; the "Mikron" teaching editor works (`MICRON.EXE`, description in `MICPC.TXT`) | graphics | downloaded manually | not stated |
| `/freedos/EDU/ALGORITM` | Algoritmika (INT) | Russian programming course for children through robot "executors". Pupil — `INF.EXE`, teacher ("Uchitelskaya": list of pupils, results) — `TI.EXE`; descriptions in `ALGOR.DOC`, `ALGOR_U.DOC`. Video adapter in murm386 — EGA 128K or higher, mouse — murm386's built-in BIOS driver or NumPad Mouse. Not suitable for the youngest: murm386's built-in mouse driver does not let the pointer into the bottom part of the screen, and without PSRAM it is very slow ([`hardware/COMPATIBILITY.md`](hardware/COMPATIBILITY.md), murm386 row) | graphics | downloaded manually; the pupil list `USERS.RUS` was replaced with a sample ("Group 1–3", "Pupil 01–46", same numbers): the original contained the surnames of a school's pupils | not stated |
| `/freedos/EDU/LOGO`, `/freedos/EDU/LOGO_DOC` | LogoWriter 3.1 (LCSI, 1988–1993), Russian version by INT | Logo with turtle graphics, material of [teaching module 18](program/modules/18-logo.md). Start with `LOGOWR.COM`; documentation in `LOGO_DOC` (CP866, in Russian; describes version 2.x with CGA graphics, same language). Needs an EGA or VGA video adapter: in murm386 — EGA 128K or VGA 128K (with MCGA 64K it hangs at start, with 256K it is slow). The keyboard has its own Russian layout, F5 switches to Latin, the characters `[ ] : "` are Alt+9, Alt+0, Alt+5, Alt+2 | EGA | downloaded manually | not stated |
| `/freedos/EDU/TP55` | Turbo Pascal 5.5 (Borland, 1989) | Pascal IDE and compiler with examples (`*.PAS`), BGI graphics and documentation (`DOC`, `README`); the `TURBO3` folder holds TP 3.0 compatibility tools. Start with `TURBO.EXE`; the folder is in `PATH` | text, BGI graphics | Borland Museum / Embarcadero Antique Software | Embarcadero: free, "as is", for personal use only; distribution over the Internet and on media is not allowed. Will be removed from the repository if the rights holder objects |
| `/freedos/EDU/BASIC` | GW-BASIC (Microsoft, 1983) and the BASCOMP 3.0 compiler (2026) | BASIC for the PC, material of [teaching module 19](program/modules/19-basic.md). `GWBASIC.EXE` is the interpreter built by tkchia from the Microsoft source code released in 2020; `GWBASICA.EXE` is the same with `TIMER`, `RANDOMIZE TIMER` and `MKDIR`/`CHDIR`/`RMDIR`. `LOAD`/`SAVE` work only in the current folder; for the compiler, save the program as text: `SAVE "NAME",A`. BASCOMP (`BASCOMP.COM`, `ASMPACK.COM`, `BASM.COM`, `HEADER.ASM`, `RUNTIME.ASM`, `TIMEIT.COM`): `build name` turns `NAME.BAS` into `NAME.COM`; integers only, `RND(n)` gives 1 to n. Lesson programs (written for this repository, CP866, CRLF): `PRIMES.BAS` — prime numbers with timing, the same program for both; `UGADAJ.BAS` — "Guess the number" for GW-BASIC | text, CGA graphics | GW-BASIC — [tkchia/GW-BASIC](https://codeberg.org/tkchia/GW-BASIC), build of 2026-10-05 (source `1432e38`), unchanged; BASCOMP — [tt1542/BASCOMP](https://github.com/tt1542/BASCOMP) 3.0 (`3555be6`), unchanged except for the line endings of `BASCOMP.LIC` (CRLF) and a fix in `RUNTIME.ASM`: the `D2_TWO` subroutine clobbered the DX register, so `TIME$` always gave 00 seconds and `DATE$` the wrong day; saving DX was added (`push dx`/`pop dx`). `RUNTIME.ASM` is inserted into every program at build time; `BASCOMP.COM` itself was not changed | GW-BASIC — MIT (`GWBASIC.LIC`); BASCOMP — CC0 1.0 (`BASCOMP.LIC`) |
| `/freedos/EDU/TC201` | Turbo C 2.01 (Borland, 1989) | the C language, material of [teaching module 21](program/modules/21-c.md): the `TC.EXE` IDE (editor, compiler and linker in one program, help in `TCHELP.TCH`) and the same tools separately for the command line — `TCC.EXE`, `TLINK.EXE`, `MAKE.EXE`, `TLIB.EXE`, `CPP.EXE`; headers in `INCLUDE`, libraries in `LIBRARY` for all memory models (tiny, small, medium, compact, large, huge), maths with coprocessor emulation and for the 8087, BGI graphics — `GRAPHICS.H`, `GRAPHICS.LIB`, `*.BGI` drivers and `*.CHR` fonts in the folder itself, `BGIOBJ.EXE`; examples in `EXAMPLES` (including `BGIDEMO.C`), the `MCALC` spreadsheet with sources and the ready program `MCALC.EXE` (built with this Turbo C from the sources in the folder using `MCALC.PRJ`, unchanged; material of [teaching module 31](program/modules/31-applications.md)), the startup code sources in `STARTUP`; `README`, `HELPME!.DOC`. The folder is in `PATH`: `tc` and `tcc` start from any folder, for example `C:\MY`. In the IDE **F9** builds, **Ctrl+F9** runs, **Alt+X** exits. `TCCONFIG.TC` (IDE) and `TURBOC.CFG` (`tcc`) were saved for this repository: paths to `INCLUDE` and `LIBRARY` without a drive letter, the graphics library is enabled in the IDE (Options → Linker → Graphics library); `tcc` with graphics — `tcc prog.c graphics.lib`. The driver path in a program is `initgraph(&gd, &gm, "\\FREEDOS\\EDU\\TC201")`. Tested in DOSBox-X (machine `ega`): text and graphics build both in the IDE and with `tcc` | text, BGI graphics | Borland Museum / Embarcadero Antique Software: files from the three floppies of the 1989 distribution, unchanged; the archives `BGI.ARC`, `EXAMPLES.ARC`, `MCALC.ARC`, `STARTUP.ARC` unpacked (names in capitals); `TC.EXE` is a copy already configured by the installer (`TCINST`) and differs from the distribution one only in its settings | Embarcadero: free, "as is", for personal use only; distribution over the Internet and on media is not allowed. Will be removed from the repository if the rights holder objects |
| `/freedos/EDU/PASCAL` | lesson programs in Turbo Pascal (written for this repository) | material of [teaching module 20](program/modules/20-pascal.md): `UGADAJ.PAS` — "Guess the number" without `GOTO`, `PRIMES.PAS` — prime numbers with timing, `RISUNOK.PAS` — a picture made of procedures (EGA 640 × 350, the BGI driver is taken from `\FREEDOS\EDU\TP55`). Open with `turbo name.pas`; CP866 encoding (Russian text), CRLF, without the letter "ё" | text, EGA | this repository | [`LICENSE`](LICENSE) |
| `/freedos/EDU/C` | lesson programs in Turbo C (written for this repository) | material of [teaching module 21](program/modules/21-c.md): `HELLO.C` — the first program, `UGADAJ.C` — "Guess the number" with input checking, `PRIMES.C` — prime numbers with timing, `RISUNOK.C` — a picture made of functions (EGA 640 × 350, the BGI driver is taken from `\FREEDOS\EDU\TC201`). Open with `tc name.c` or build with `tcc name.c` (`RISUNOK.C` — `tcc risunok.c graphics.lib`); CP866 encoding, CRLF, without the letter "ё" | text, EGA | this repository | [`LICENSE`](LICENSE) |
| `/freedos/EDU/DATA` | lesson programs about data types and structures (written for this repository) | material of [teaching module 23](program/modules/23-data.md): the same experiments in three languages — `.BAS` (GW-BASIC; for BASCOMP copy to `\FREEDOS\EDU\BASIC`), `.PAS` (Turbo Pascal), `.C` (Turbo C): `PEREPOL` — integer overflow, `DROBI` — inexact fractions, `ISTINA` — true and false, `MASSIV` — going past the end of an array, `STROKA` — strings, `REKORDY` — a high-score table of records in the shared file `REKORDY.TXT` (created on first run); `SOYUZ.C` — a `union`, `VARIANT.PAS` — a variant record. From this folder: `turbo name.pas`, `tc name.c`, `..\basic\gwbasic name.bas`; CP866 encoding (BASIC in Latin letters), CRLF, without the letter "ё". Tested in DOSBox-X | text | this repository | [`LICENSE`](LICENSE) |
| `/freedos/EDU/BIOS` | lesson programs about BIOS, DOS, memory and ports (written for this repository) | material of [teaching module 24](program/modules/24-bios-dos.md): `PAMYAT` (`.BAS`, `.PAS`, `.C`) — reading memory: the BIOS date at F000:FFF5, BIOS data from 0040:0000 (RAM, COM1 and LPT1 ports, Caps Lock), a letter straight into video memory at B800h; `PORTY` (`.BAS`, `.PAS`, `.C`) — the clock through ports 70h/71h and the speaker sound through the timer and port 61h; `TRI.C` — printing a string and reading the time in three ways: through DOS, through the BIOS and directly. The BASIC versions are for GW-BASIC (`..\basic\gwbasic name.bas`), BASCOMP does not build them. The built copies are on the floppy `/386/fdd0free.img` in `A:\BIOS`. CP866 encoding (BASIC in Latin letters), CRLF, without the letter "ё". Tested in DOSBox-X, including booting from the floppy | text | this repository | [`LICENSE`](LICENSE) |
| `/freedos/EDU/DMA` | lesson programs about interrupts and DMA (written for this repository) | material of [teaching module 25](program/modules/25-dma-irq.md): `SINUS` (`.BAS`, `.PAS`, `.C`) — a sine wave of a given frequency in a memory block, which the Sound Blaster plays in a loop through DMA; in Pascal and C with its own interrupt handler (a block counter), in GW-BASIC by polling, without a handler; `PIZVUK.C` — 500 digits of pi in three modes: no sound, sound through a Covox (the processor outputs every sample), sound through the Sound Blaster (DMA and interrupts), with timing. Needs SoundBlaster (220h, IRQ 5, DMA 1) and Covox Speech Thing (LPT2) enabled in murm386. CP866 encoding (BASIC in Latin letters), CRLF, without the letter "ё". Tested in DOSBox-X (SB16, IRQ 5, DMA 1): interrupts and DMA work; check the sound on a workstation | text, sound | this repository | [`LICENSE`](LICENSE) |
| `/freedos/EDU/ASM` | JWasm 2.21 (`JWASMR.EXE`, the real-mode DOS version) and lesson programs (written for this repository) | material of [teaching module 26](program/modules/26-asm.md): `SINB.BAS` — a sine wave through DMA for BASCOMP with places for the assembly patches `Z1.ASM`…`Z4.ASM` (buffer address, setting and restoring the vector, an IRQ handler that changes a BASIC variable); `VSTAVKA.EXE` (`.PAS`) inserts a patch into the listing after a line label; `INLINE.PAS` (`inline`) and `INLINE.C` (`asm`, pseudo-registers); `SUMMA.ASM` — a function in the Pascal and C calling conventions, `PSUMMA.PAS` and `CSUMMA.C` — linking it in. `TASM.EXE` (`.C`) is a shim: for `asm` inserts Turbo C calls TASM, and the shim translates the switches and runs JWasm; it works when `tcc` is started in this folder. Sources in CP866 (patches and BASIC in Latin letters), CRLF, without the letter "ё". Tested in DOSBox-X (SB16, IRQ 5, DMA 1) | text, sound | JWasm — the archive `JWasm_v221_dos.zip` from [Baron-von-Riedesel/JWasm](https://github.com/Baron-von-Riedesel/JWasm), `JWasmr.exe` unchanged (renamed to `JWASMR.EXE`), `License.txt` as `JWASM.LIC`; the rest is this repository | JWasm — Sybase Open Watcom Public License 1.0 (`JWASM.LIC`), source at the link; programs — [`LICENSE`](LICENSE) |
| `/freedos/EDU/ELEMENTS` | Elements | periodic table | text | same | freeware according to freedos-repo |
| `/freedos/EDU/GRAPHXY` | GraphXY | plotting function graphs | EGA/VGA | same | freeware. VGA mode 640×480, 16 colours; the `EGAVGA.BGI` driver and `*.CHR` fonts are loaded from the current folder — start it from `\FREEDOS\EDU\GRAPHXY`. **Known issue:** in murm386 v1.19 it works only with the 386 core and the VGA256 video adapter: in the 386 build with PSRAM, and in the build without PSRAM when VGA256 is selected in the Win+F11 menu (Video adapter), slowly. With VGA128 and simpler adapters, and in the 286 build, it does not start |
| `/freedos/EDU/SOUNDS` | SOUNDS (written for this course) | the opening of the "Ode to Joy" on eight PC sound devices: PC speaker, Tandy, AdLib, Covox Sound Master, MPU-401, Covox Speech Thing, Disney Sound Source, Sound Blaster; speech through a DAC (`SPEECH.RAW`), digital sound quality at different bit depths and sample rates, the game "Guess the device". Labels in Russian (CP866; needs murm386's built-in BIOS — with SeaBIOS the letters are unreadable). Start `SOUNDS.EXE` from this folder; the source `SOUNDS.PAS` is built with `tpc sounds`; description in [`tools/sound`](tools/sound/README.md) | text | this repository | [`LICENSE`](LICENSE) |
| `/freedos/EDU/VIDEO` | VIDEO (written for this course) | one picture in all the PC video modes that murm386 shows: CGA colour text, CGA 320 × 200 and 640 × 200, EGA 320 × 200 and 640 × 350, MCGA 640 × 480, VGA 640 × 480, 320 × 200 in 256 colours, SVGA (VESA) 640 × 400 in 256 colours; for each — bits per pixel, video memory bytes and drawing time. Modes that the selected adapter does not have (Win+F11, Video adapter) are skipped, and the program says so. Labels in Russian (CP866; needs murm386's built-in BIOS). Start `VIDEO.EXE`; the source `VIDEO.PAS` is built with `tpc video`; description in [`tools/video`](tools/video/README.md) | text, all graphics modes | this repository | [`LICENSE`](LICENSE) |

The files were taken from the freedos-repo packages unchanged, except for names: in a few cases they were shortened to 8.3 (`hanoie.exe` → `HANOI.EXE`, `Kingdom.com` → `KINGDOM.COM`). On murm386 v1.19 Shuffle V does not work in any build, and GraphXY works only with the 386 core and the VGA256 video adapter (see the table); the other programs have not yet been checked one by one.

Without PSRAM it is better to run SOUNDS with the MCGA 64K video adapter (Win+F11, Video adapter; Enter, Y — with a restart): less swapping, and the Covox and Disney Sound Source sound smoother. SOUNDS hears only the devices enabled in the murm386 settings: **Win+F11**, select the line with the arrows, ← → change the value, **Esc** and **Y** save (no reboot needed). By default PC Speaker, AdLib, SoundBlaster, Roland MPU-401 and Covox in Speech Thing mode (LPT2) are enabled — items 1, 3, 5, 6, 8, A and D; game G also needs Tandy Sound. For item 2 — Tandy Sound: Enabled; for item 7 — Disney Sound Source (LPT1): Enabled; for item 4 — Covox: Sound Master (240h), **240h exactly, not 220h** (SOUNDS writes only to port 240h). The Sound Master takes the ports and DMA channel of the sound cards, so when it is selected murm386 itself turns off AdLib and SoundBlaster; after item 4 set Covox back to Speech Thing (LPT2) and turn AdLib and SoundBlaster on again.

## 5. Programs and games for pico-speccy (`/ZX`)

pico-speccy opens files from any folder of the SD card through its file browser (F5) and understands almost all Spectrum formats: `.tap`, `.tzx`, `.z80`, `.sna`, `.trd`, `.scl` and others. ZX81 files (`.p`) are opened with F2; the emulator switches to ZX81+ mode by itself. The selection is in `/ZX`.

Controls: **F1** — emulator menu (computer model — Machine → Spectrum: 48K, 128K, +2, +3, ZX81+ and others); **F5** — open a file, Enter — load and run; **Shift** — CAPS SHIFT; **Ctrl** — SYMBOL SHIFT; **Esc** — BREAK; **F11** — reset the Spectrum; **F9/F10** — volume.

Next to each file there is a `.TXT` file with a description, the authors and the licence text. The files were renamed to short names; their content is unchanged.

Games by The Mojon Twins. The authors distribute their work (games, graphics, music, sources) under [CC BY-NC-SA 3.0](https://creativecommons.org/licenses/by-nc-sa/3.0/): free non-commercial distribution with attribution. Source of the files: [retrobrews/zxspectrum-games](https://github.com/retrobrews/zxspectrum-games), commit `b3cb92e`.

| File | Game | Genre |
|---|---|---|
| `COLUMNS.TZX` | Columns | puzzle: make rows of three identical gems |
| `CHERILB.TZX` | Cheril of the Bosque | adventure platformer |
| `CHERILP.TZX` | Cheril Perils | platformer |
| `CHERILG.TAP` | Cheril the Goddess | platformer |
| `LALA.TZX` | Lala the Magical: Prologue | platformer |
| `CADAVER.TAP` | Cadàveriön | dungeon puzzle |
| `MARIANO.TZX` | Mariano the Dragon: Capers in Cityland | arcade |
| `ABABOL.TZX` | Sir Ababol | platformer |
| `ABABOL2.TAP` | Sir Ababol 2: The Ice Palace | platformer |

Cheril the Goddess, Cheril Perils, Cheril of the Bosque, Lala Prologue and Cadàveriön are also in the NES selection (section 3) — the same game can be compared on two platforms: graphics, colour, sound, controls.

The intro of Sir Ababol is a humorous story about a crusader.

Other programs:

| File | Program | What to do | Machine | Source | Licence |
|---|---|---|---|---|---|
| `MAZE3D.TAP` | ZXMAZE3D (Andrej Oresnik) | a 3D maze: W — forward, S — back, A and D — turn | 48K and up | [aoresnik/zxmaze3d](https://github.com/aoresnik/zxmaze3d), branch `gh-pages` (`79a7f9e`), `files/zxmaze3d.tap` | BSD (3-clause) |

## 6. Disk images for the Apple IIe (`/apple`)

murmapple looks for disk images in the `/apple` folder (formats `.dsk`, `.nib`, `.woz`, `.bdsk`); the disk is chosen in the emulator's on-screen menu. The selection is meant mainly for checking picture and sound and for getting to know the Apple II; all images are DOS 3.3, 140 KB.

| File | What it is | What it checks | Author, source | Licence |
|---|---|---|---|---|
| `SHORTPRG.DSK` | Short Programs — a collection of one- and two-line Applesoft BASIC programs: graphics, animation, sound | HGR/LORES graphics, BASIC; good material for the programming module — the programs are short and can be taken apart | Lee Fastenau, [thelbane/Apple-II-Programs](https://github.com/thelbane/Apple-II-Programs) `3bc2568` | MIT |
| `FIRE.DSK` | Fire — small "fire" effects | LORES graphics | Vince Weaver, [deater/dos33fsprogs](https://github.com/deater/dos33fsprogs) `3a02734` | GPL v2 |
| `TB6502.DSK` | Tom Bombem — an arcade shooter | gameplay, sound | same | GPL v2 |
| `TFV.DSK` | Talbot Fantasy 7 — a humorous role-playing game in LORES mode | gameplay, keyboard | same | GPL v2 |

The images were taken from the repositories unchanged and renamed to short names. Checked on 2026-10-02 on the PCp2: the images of the selection work in both murmapple builds. There are no images for testing the Mockingboard in the selection.

How to start a disk: F11 — the on-screen disk menu, choose the image, then Boot. If the program did not start by itself, in Applesoft BASIC: `CATALOG` lists the files on the disk, `RUN name` runs a BASIC program (type `A`), `BRUN name` runs machine code (type `B`); `PR#6` reboots from the disk drive.

## 7. Games for pico-z26 (`/z26`)

pico-z26 looks for games in the `/z26` folder of the SD card (extensions `.bin`, `.a26`, `.rom`). Only free homebrew games with an open licence are included; commercial Atari 2600 cartridge images are not part of the kit. Controls — see the pico-z26 row in [`hardware/COMPATIBILITY.md`](hardware/COMPATIBILITY.md): arrows — joystick, Z — button, Enter — RESET (start), Esc — SELECT.

| File | Game | What to do | Source | Licence | Size, bank scheme |
|---|---|---|---|---|---|
| `2048.bin` | 2048 2600 (Carlos Duarte do Nascimento, 2014) | the "2048" puzzle: slide the tiles with the joystick and add up equal numbers; start with the button | [chesterbr/2048-2600](https://github.com/chesterbr/2048-2600) `35de3c1` | MIT | 2 KB |
| `berta.bin` | Berta and Butterflies (vandalton, 2024) | Berta the little elephant catches butterflies from four sides — inspired by "Nu, pogodi!" / Game & Watch "Egg" | [vandalton/BertaAndButterflies](https://github.com/vandalton/BertaAndButterflies), release v1.00 (`7f81d40`), `berta-and-butterflies.v1.00.ntsc.en.bin` | MIT | 4 KB |

The images were taken from the repositories unchanged (Berta from the GitHub release) and renamed to short names. The sources are in the same repositories. Checked on 2026-10-03 on the PCp2 with pico-z26 4.0.8 (`9689fc5`): both games work. Games of 8 KB and more with bank switching do not start in this version of pico-z26, see [`hardware/COMPATIBILITY.md`](hardware/COMPATIBILITY.md).

## 8. Programs for atari800 (`/atari800`)

atari800 keeps its files in the `/atari800` folder of the SD card and opens its file selector there. No separate ROM images are needed: the emulator has built-in AltirraOS for the 400/800 and XL/XE, Altirra 5200 OS and Altirra BASIC, as well as the original Atari OS Rev. B (400/800), XL OS BB01 Rev. 2 and Atari BASIC Rev. C. The 130XE is emulated by default. The selection covers different machines of the line: the 400/800 and XL/XE computers and the 5200 console.

Controls: **F1** — emulator menu; **Alt+R** — run a program (`.XEX`, `.ATR`); **Alt+C** — cartridge (`.CAR`); **Alt+D** — disk drives; **Alt+Y** — system settings (machine type, memory size, OS); **F2/F3/F4** — the console keys Option/Select/Start; **Pause/Break** — the BREAK key (stops a BASIC program); **F9/F10** — volume. The joystick is emulated with the keyboard: arrows (or the numeric keypad, with diagonals on 7, 9, 1, 3) — directions, Ctrl — button. An NES DPAD gamepad can be connected instead of the keyboard.

On the XL/XE the built-in BASIC is on by default; it takes 8 KB of memory, and some programs do not start with it. Turn it off before running a `.XEX`: Alt+Y → BASIC.

| File | Program | What to do | Machine | Source | Licence |
|---|---|---|---|---|---|
| `FASTBAS.ATR` | FastBasic 4.7 (Daniel Serpell) | a modern BASIC with an editor right on the Atari; the disk has examples, including the games Joyas and Carrera 3D | 400/800 48 KB and up | [dmsc/fastbasic](https://github.com/dmsc/fastbasic), release v4.7 (`55dc52f`), `fastbasic-v4.7.atr` | GPL v2+ with an exception for compiled programs; BW-DOS (freeware) on the disk |
| `ACTION.CAR` | Action! 3.6 (Clinton Parker, OSS, 1983) | a programming language with editor and compiler on a cartridge: type a program in the editor, Shift+Ctrl+M — monitor, `C` — compile, `R` — run; Shift+Ctrl+E — back to the editor | 400/800, XL/XE | cartridge image from [jhusak/atari_action_compiler](https://github.com/jhusak/atari_action_compiler) `7de960e`, `inc/action_36.c`; sources — [pjones1063/Atari_OSS_Action](https://github.com/pjones1063/Atari_OSS_Action) `e35661c` | GPL v3 (sources released by the author in 2015) |
| `FIREFITE.XEX` | Firefighter (Bill Kendrick, 2025) | put out the fire and lead people to the exit; two players possible. F4 (Start) — begin; arrows — walk; Ctrl + arrow — water jet in that direction | 48 KB, BASIC off | [billkendrick/firefighter](https://github.com/billkendrick/firefighter), release 0.1-beta10 (`546c7a7`) | GPL v3 |
| `GEMDROP.XEX` | Gem Drop Deluxe (Bill Kendrick) | puzzle: catch and throw coloured gems, making sets of three | 48 KB | [billkendrick/gemdrop_deluxe](https://github.com/billkendrick/gemdrop_deluxe) `e6a3dc4`, built from source (cc65) | GPL v2 |
| `SCORCH.XEX` | Scorch (Pecus, pirx) | artillery duel for 2–6 players. In the menu — arrows, Return — next; in battle ←/→ — barrel angle, ↑/↓ — power, space or Ctrl — fire | 48 KB | [pkali/scorch_src](https://github.com/pkali/scorch_src) `f1e2c46`, `scorch.xex` | Unlicense |
| `SCORCH52.CAR` | Scorch for the Atari 5200 console | the same on the 5200: arrows and Ctrl, as above | Atari 5200 (machine type 5200 in Alt+Y) | same, `scorch.bin` with a CAR header (type 4, 5200 32 KB) | Unlicense |
| `DLI1.XEX` | display list interrupt example (Seban/Slight, 2016) | demonstrates a colour trick: lines of different colours on one screen and a scrolling line | 48 KB | [seban-slt/atari8_6502_code_examples](https://github.com/seban-slt/atari8_6502_code_examples) `da91a1a`, built from source (xasm) | public domain (`README.md`) |

The images were taken from the repositories and releases unchanged, except as noted: `GEMDROP.XEX` and `DLI1.XEX` were built from source, and a 16-byte CAR header was added to the Scorch image for the 5200. The files were renamed to 8.3 names. Checked on 2026-10-04 on the PCp2 (130XE, AltirraOS, PAL): FastBasic, Action!, Gem Drop, Scorch, Scorch 5200 and DLI1 start; Firefighter with BASIC off (checked on a PC).

## 9. Programs for murmc64 (`/c64`)

murmc64 (a Commodore 64 emulator) opens files from the `/c64` folder with the **F10** key. A `.PRG` file is loaded into memory and started automatically (the emulator types `RUN`); a `.D64` or `.D81` disk image is attached as drive 8 and loaded with `LOAD"*",8,1`. No separate ROM images are needed.

Controls: the keyboard works like the C64's (Tab or left Ctrl — CTRL, left Alt — C=, Home — CLR/HOME); **F1–F8** — the C64 function keys; **F10** — file selection; **F11** — RESTORE; **Ctrl+Alt+Del** — reset the C64; **F9** — switch the joystick port. The joystick (port 2 by default) is emulated with the keyboard: arrows — directions, right Ctrl or right Alt — button. An NES DPAD gamepad can be connected instead of the keyboard.

All programs are examples from the C compiler for the C64 [drmortalwombat/oscar64](https://github.com/drmortalwombat/oscar64) (Dr. Mortal Wombat), licence GPL v3; the sources are in the `samples` folder of the same repository. They were built from the sources of commit `3e2ffb6` with the switches from `samples/*/build.sh` by oscar64 of that commit itself; the files were renamed to 8.3 names. All games are controlled with a joystick in port 2.

| File | Program | What to do | Source |
|---|---|---|---|
| `SNAKE.PRG` | Snake | arrows — direction; do not crash into the walls or yourself | `samples/games/snake.c` |
| `LANDER.PRG` | Moon landing | ↑ — engine, ←/→ — move sideways; land softly | `samples/games/lander.c` |
| `MAZE3D.PRG` | 3D maze | ↑/↓ — step forward and back, ←/→ — turn | `samples/games/maze3d.c` |
| `MISSILE.PRG` | Missile defence (after Missile Command) | arrows — aim, button — fire; protect the cities | `samples/games/missile.c` |
| `BREAKOUT.PRG` | Breakout | ←/→ — paddle, button — launch the ball | `samples/games/breakout.c` |
| `CONNECT4.PRG` | Connect Four against the computer | ←/→ and the button, or keys 1–7 — drop a disc into a column | `samples/games/connectfour.c` |
| `SHMUP.PRG` | Scrolling space shooter | arrows — ship, button — fire | `samples/games/hscrollshmup.c` |
| `MANDEL3D.PRG` | The Mandelbrot set in 3D | watch the picture being built; any key — exit | `samples/fractals/mbmulti3d.c` |
| `FUNC3D.PRG` | 3D function plot | same | `samples/hires/func3d.c` |
| `FRACTREE.PRG` | Fractal tree | same | `samples/hires/fractaltree.c` |
| `CUBE3D.PRG` | Rotating wireframe cube | watch | `samples/hires/cube3d.c` |

## 10. Programs for frank-micro (`/micro`)

frank-micro (an emulator of the BBC Micro Model B and Master 128) keeps its files in the `/micro` folder: disk images in `/micro/disk`, extra ROMs in `/micro/roms`, settings in `/micro/micro.ini`, screenshots in `/micro/screenshot`. MOS, BBC BASIC and DFS are built into the firmware, so the files in `/micro/roms` are not needed to work. The Master 128 is emulated by default.

Controls: the keyboard works like the BBC Micro's (**Esc** — ESCAPE, **F1–F10** — function keys f0–f9, **Backspace** — DELETE, **End** — COPY; symbols follow the BBC keyboard layout: `"` — **Shift+2**, `=` — **Shift+-**, `:` — **'**, `*` — **Shift+'**); **F11** — choose a disk image: the disk in drive 0 boots by itself (SHIFT+BREAK); **F12** — settings (model, monitor colour, volume, sound output — the options available depend on the board, gamepad keys); **Ctrl+Alt+Del** — reset the BBC; **Print Screen** — screenshot.

| File | Program | What to do | Source | Licence |
|---|---|---|---|---|
| `/micro/disk/CLASS.SSD` | BBC BASIC programs for the lessons: a menu, `HELLO` (procedures and `REPEAT`), `TELETXT` (MODE 7 teletext colours), `SHAPES` (triangles in MODE 1), `SPIRAL` (a spiral in MODE 2), `MUSIC` (a tune with the `SOUND` command), `GUESS` ("Guess the number") | F11 → `CLASS.SSD`; the disk boots into the menu, keys 1–6 start a program; when a program has finished, any key returns to the menu; ESCAPE — exit to BASIC, where `LIST` shows the program text, `RUN` runs it again, `CHAIN "MENU"` returns to the menu | this repository; sources and build command in [`tools/bbc`](tools/bbc/README.md) | [`LICENSE`](LICENSE) |
| `/micro/disk/GLOBE.SSD` | a rotating star globe (6502 assembly demo) | F11 → `GLOBE.SSD`; Z and X — rotation speed, ESCAPE — exit | [stardot/beebasm](https://github.com/stardot/beebasm) `ca2cc5f`, `demo.ssd` unchanged | GPL v3 (source `demo.6502` in the same place) |
| `/micro/roms/*` | extra ROMs: DFS and ADFS variants, Acorn DNFS, BASIC, GXR, OS 1.2, Master 128 ROM sets (MOS 3.20 and 3.50: View, ViewSheet, Edit, Terminal, BASIC IV), Master Compact | not needed to work | `sdcard/micro.zip` from [DnCraptor/frank-micro](https://github.com/DnCraptor/frank-micro) `06f7894`, unchanged | ROMs of Acorn and other 1980s companies, distributed with emulators without an open licence |

## 11. Programs for frank-msx (`/MSX`)

frank-msx (an emulator of the MSX, MSX2 and MSX2+, a port of fMSX) keeps everything in the `/MSX` folder: BIOS ROMs, cartridges, settings (`/MSX/msx.ini`). Without PSRAM only an MSX1 with 64 KB of RAM is emulated; floppy images do not load in this mode, and a selected cartridge is written to the board's flash, so the first load of a new cartridge takes a little longer than usual.

Controls: the keyboard works like the MSX's (**End** — SELECT, **Page Up** — STOP, **Alt** — GRAPH, **F1–F5** — the MSX function keys); **F11** — the loader: Cartridge A → choose a `.ROM` file, leave the mapper type on automatic, the MSX restarts with the cartridge; **F12** — settings (model, PAL/NTSC frame rate, joysticks, sound output); **Ctrl+Alt+Del** — reset the MSX. Without a cartridge the MSX starts in MSX BASIC. The NES gamepad is the MSX joystick in port A.

Tapes with MSX BASIC programs for teaching module 10 (written for this repository; sources and build in [`tools/msx`](tools/msx/README.md), terms in [`LICENSE`](LICENSE)). Loading: **F11** → Cassette tape → Insert tape (or Change tape) → the file, then the command `RUN"CAS:"`.

| File | Program |
|---|---|
| `HELLO.CAS` | asks for your name, says hello and changes the background colour |
| `MUSIC.CAS` | the "Ode to Joy" in three voices, then notes from the keyboard (A–K), Esc — exit |
| `DRAW.CAS` | a picture in graphics mode: a house, the sun, a flower and a spiral with the `DRAW` command |
| `SPRITE.CAS` | arrows control a sprite, a second one flies by itself, collisions are counted; space — exit |
| `GUESS.CAS` | the "Guess the number" game |

Games by Santiago Ontañón (Brain Games), made for the MSX1 in 2016–2021; all run on an MSX1 with 16 KB of RAM. The sources are open; the ROMs were built from the sources of the listed commits with the build scripts from the repositories (the Glass and MDL Java assemblers, kept in the same place). For The Menace from Triton the aPLib-compatible [apultra](https://github.com/emmanuel-marty/apultra) was used instead of the `oapack` packer (which is not in the repository), and for Westen House — [ZX0](https://github.com/einar-saukas/ZX0) with the `-c` switch, as the game's README requires; both builds were repeated until the image stopped changing. All five images were tested in openMSX on an MSX1 machine with the ROM `/MSX/MSX.ROM`. Next to each file there is a `.TXT`: description, controls, source and the full licence text.

| File | Game | Genre | Controls | Source | Licence |
|---|---|---|---|---|---|
| `TRANSBAL.ROM` | Transball | a ship in a cave with gravity and inertia: pick up the ball and carry it out of the level | ←/→ — turn, ↑ or M — thrust, space — fire | [santiontanon/transballmsx](https://github.com/santiontanon/transballmsx) `65e6baf` (1.3.2 plus fixes) | GPL v3 |
| `POPOLON.ROM` | Tales of Popolon | a first-person 3D maze (ray casting) based on Ancient Greek myths | ←/→ — turn, ↑/↓ — step, space — weapon, space+←/→ — sidestep, M — second weapon | [santiontanon/talesofpopolon](https://github.com/santiontanon/talesofpopolon) `36092a5` (1.3.1) | GPL v3 |
| `XSPELUNK.ROM` | XSpelunker | a Spelunky-style platformer: Maya ruins, each level is generated anew | ←/→ — walk, ↑ — jump, ↓ — crouch, space — machete, M or 1–8 — choose an item | [santiontanon/xspelunker](https://github.com/santiontanon/xspelunker) `431422d` (1.4.3 plus fixes) | GPL v3 |
| `TRITON.ROM` | The Menace from Triton | a horizontally scrolling space shooter with upgrades bought between missions | arrows — ship, space — fire, M — take an upgrade, F1 — pause | [santiontanon/triton](https://github.com/santiontanon/triton) `5ece52a` (1.2 plus fixes) | Apache 2.0 |
| `WESTEN.ROM` | Westen House | an isometric adventure: collect a scientist's notes in a mysterious house | arrows — walk, space — action, M — items; a tutorial at the start of the game | [santiontanon/westen](https://github.com/santiontanon/westen) `8942931` (1.3.1 plus fixes) | Apache 2.0 |

Transball, Tales of Popolon and XSpelunker take 32 KB, Triton and Westen House 48 KB: these are plain cartridges without a mapper, like the games of the early MSX years.

The BIOS ROMs and the emulator's service files were taken from `sdcard/msx.zip` of the [DnCraptor/frank-msx](https://github.com/DnCraptor/frank-msx) repository (`083d87a`), unchanged; the F11 loader does not show them. Without PSRAM only `MSX.ROM` is needed; the others are for the MSX2 and MSX2+ on workstations with PSRAM.

| File | What it is |
|---|---|
| `MSX.ROM` | BIOS and MSX BASIC 1.0 (Microsoft, 1983) |
| `MSX2.ROM`, `MSX2EXT.ROM` | BIOS, MSX BASIC 2.0 and the MSX2 BIOS extension |
| `MSX2P.ROM`, `MSX2PEXT.ROM` | same for the MSX2+ |
| `DISK.ROM` | Disk BASIC — disk controller |
| `FMPAC.ROM` | FM-PAC cartridge ROM (FM sound) |
| `PAINTER.ROM` | Yamaha Painter graphics editor |
| `CMOS.ROM` | initial content of the MSX2 battery-backed clock |
| `CARTS.SHA` | fMSX database of cartridge checksums with mapper types |

The MSX BIOS ROMs are proprietary (Microsoft, ASCII and the computer makers) and are distributed with emulators without an open licence; they will be removed from the repository if the rights holder objects. `CARTS.SHA` is part of fMSX (Marat Fayzullin), under the same terms as fMSX.

## 12. Disks for the Korvet (`/korvet`)

The Korvet firmware (a port of Emu80, branch `korvet` of the [DnCraptor/emu80v4](https://github.com/DnCraptor/emu80v4) repository) keeps disk images in the `/korvet` folder and creates it by itself on first start. The Korvet ROMs are built into the firmware; without a disk the Korvet starts in its built-in BASIC.

Controls: **Alt** — emulator menu (disks, settings), **Alt+A** — insert a disk image into drive A; the system boots from drive A after a reset — **F11**. **Alt+J** — Russian ЙЦУКЕН layout, **Alt+Q** — QWERTY. **Ctrl+Alt+Del** — reboot the board. NES gamepad: Start — menu, D-pad — arrows, A — space, B — Enter.

This is a small starter selection: three disks from collections shared by Korvet enthusiasts (chat [ZX_MURMULATOR](https://t.me/ZX_MURMULATOR/282025/282087), [more](https://t.me/ZX_MURMULATOR/282025/282029)). The images are unchanged, only renamed. All three disks were tested in the desktop Emu80 v4 (branch `original` of the same repository): the disk boots; of the programs, `KLAVIT.TRN`, `LOGIKA`, `FIZIKA`, `KLAD`, `XONIX`, `TENNIS` and `MW` were checked, the rest only by the disk directory. The programs are in Russian.

| File | What is on the disk | How to start |
|---|---|---|
| `SCHOOL.KDI` | MicroDOS 2.6 (NIISCHETMASH, 1990) and Soviet school programs in BASIC from 1988–1990: a typing tutor "for KORVET-type computers" (`KLAVIT.TRN`), "Fundamentals of Informatics and Computing, 10th grade" — the AND and OR logic gates (`LOGIKA`), BASIC programming lessons (`UROK1`–`UROK10`), "Motion of a body under gravity" (`FIZIKA`, Chernivtsi, 1988), the periodic table (`TABMEN`), "Pupils draw" (`RISUNOK`), "Mosaic" (`MOZAIKA`), the games "Reversi" (`REWERSI`) and "Artillery" (`ORUDIE`) | after booting (`A>`) type `BASIC`, then, for example, `RUN"KLAVIT.TRN"` or `RUN"LOGIKA"` |
| `GAMES.KDI` | CP/M 2.2 ("PK Kontur", 1990) and games: "Klad" (Lode Runner, `KLAD`, `KLAD1`–`KLAD3`), "Xonix" (`XONIX`, Moscow State University, 1988), "Sokoban" (`SOKOBAN`), "Tennis" (`TENNIS`), "Chess" (`CHESS`), "Karate" (`KARATE`), "Rally" (`RALLY`), "Racing" (`GONKI`), `JUMPER`, `ARK`, `ARC` and others | after booting (`A>`): `DIR` — list, the game's name — start it, for example `KLAD` |
| `MUSIC.KDI` | CP/M 2.2 and the Music World music editor (Woodman, 1990) with tunes: "Kalinka", "Vologda", "Nightingale", "Minuet" and others | after booting type `MW` |

Korvet programs from the 1980s–1990s are distributed without an open licence; the images will be removed from the repository if the rights holder objects.

## 13. Programs for the Vector-06C (`/vector06c`)

The Vector-06C firmware (a port of Emu80, branch `vector06c` of the [DnCraptor/emu80v4](https://github.com/DnCraptor/emu80v4) repository) opens files from the `/vector06c` folder and creates it by itself on first start. The Vector's boot loader is built into the firmware.

Controls: **Alt** — emulator menu, **Alt+F3** — load and run a file (`.rom`, `.fdd` and others), **Alt+F11** — reset the Vector, **Ctrl+Alt+Del** — reboot the board. NES gamepad: Start — menu, D-pad — arrows, A — space, B — Enter.

Games by the RetroSouls team (Denis Grachev, music by Oleg Nikitin), made for the Vector-06C in recent years. Licence [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/): free non-commercial distribution with attribution. The files were taken from the authors' archives unchanged (only the descriptions were renamed); the authors' descriptions (CP1251 encoding, in Russian) and the licence text are in `/vector06c/DOC`. Source of the games: [retrosouls.net](https://www.retrosouls.net), [retrosouls.itch.io](https://retrosouls.itch.io/).

| File | Game | Year | Controls |
|---|---|---|---|
| `OT.ROM` | "Old Tower" — puzzle: two heroes in a tower must be led through rooms with traps, collecting gold | 2023 | arrows — move, space — switch hero, Tab — restart the level |
| `PLYUK.ROM` | "Plyuk" — puzzle: Plyuk has fallen into a cave and gets out by making clones of himself | 2024 | arrows — move, space or Tab — make a clone, ЗБ (Backspace) — restart the level |

## 14. Programs for pico-pce (`/PCE`)

pico-pce (a PC Engine emulator, pce-go core) lists cartridges from the `/PCE` folder and keeps its settings (`pico-pce.cfg`) and saves there too. A selected cartridge is written to the board's flash, so starting a new cartridge takes a little longer than usual.

Controls: arrows (or WASD) — D-pad, **Z** — button I, **X** — button II, **Enter** — RUN, **Esc** or **Backspace** — SELECT; **Enter+Esc** — emulator menu (saves, clock, back to cartridge selection); **Ctrl+F1–F8** — save the game to a slot, **Alt+F1–F8** — load; **Ctrl+Alt+Del** — reboot the board. NES gamepad: A — I, B — II, Start — RUN, Select — SELECT, Start+Select — menu.

The cartridges were written in C for this repository (sources and build in [`tools/pce`](tools/pce/README.md), terms in [`LICENSE`](LICENSE)). On-screen text is in Latin letters.

| File | Program |
|---|---|
| `hello.pce` | text and colours, a clock in seconds, shows the pressed buttons; I and II — sound |
| `sprite.pce` | arrows move a robot, a ball flies by itself; touch the ball — a point, I — another robot palette |
| `catch.pce` | the game "Catch the stars": RUN — start, ←/→ — basket, three misses — game over |

## 15. Programs for pico-genesisPlus (`/MD`)

pico-genesisPlus (a Sega Mega Drive / Genesis emulator, gwenesis core) lists `.md` and `.bin` files from any folder of the card, starting from the root. It keeps its own files in the root of the card: settings (`settings_*.dat`), the list of recent games (`recent_*.txt`), a record of the cartridge in flash (`flashedrom.dat`), and the folders `/SAVES` (cartridge saves) and `/SAVESTATES` (game snapshots). A selected cartridge is written to the board's flash, and the board reboots; without PSRAM cartridges of up to about 2.4 MB fit.

pico-genesisPlus has its own keys, different from the other emulators of the kit, and the keys in the menu differ from those in the game.

In the file list and the menu:

| Key | Action |
|---|---|
| ↑ ↓ ← → | select |
| **X** | open a folder, start a game, confirm |
| **Z** | back: folder up, leave the menu |
| **A** | settings |
| **C** | recent games |

In the game — a Mega Drive gamepad on the keyboard:

| Key | Mega Drive button |
|---|---|
| ↑ ↓ ← → | D-pad |
| **Z**, **X**, **C** | A, B, C |
| **Q**, **W**, **E** | X, Y, Z (six-button gamepad) |
| **S** | START |
| **A** | not a game button but a modifier key for combinations: |
| **A+S** | menu: game snapshots (save, load), reset the game, back to the file list |
| **A+↑** | screen mode |
| **S+Z** | show or hide the frame rate |

An NES gamepad on UEXT is player one while no USB keyboard is connected: B — B, A — C, SELECT — A, START — START, SELECT+START — menu. On some Dendy clones A and B are swapped, and C ends up on SELECT. When a USB keyboard is connected, it is player one and the NES gamepad is player two.

The cartridges were written in C for this repository (sources and build in [`tools/md`](tools/md/README.md), terms in [`LICENSE`](LICENSE)); they are the same programs as for the PC Engine in section 14. On-screen text is in Latin letters.

| File | Program |
|---|---|
| `hello.bin` | text and colours, a clock in seconds, shows the pressed buttons; A, B and C — sound |
| `sprite.bin` | arrows move a robot, a ball flies by itself; touch the ball — a point, A — another robot palette |
| `catch.bin` | the game "Catch the stars": START — start, ←/→ — basket, three misses — game over |

## 16. Programs for pico-gameboy (`/GB`)

pico-gameboy (a Nintendo Game Boy emulator, Peanut-GB core) lists `.gb` and `.gbc` cartridges from the `/GB` folder and keeps saves there too. A selected cartridge is written to the board's flash. Games for the Game Boy Color, the later model, hardly work in this build; games for the original Game Boy can be "coloured" by pico-gameboy — the palette is chosen in the menu.

Controls: arrows (or WASD) — D-pad, **Z** — button A, **X** — button B, **Enter** — START, **Esc** or **Backspace** — SELECT; **Enter+Esc** — emulator menu (palette, game snapshots, clock, **Reset to ROM select** — back to cartridge selection, **Return to game** — back to the game); **Ctrl+F1–F8** — save the game to a slot, **Alt+F1–F8** — load; **Ctrl+Alt+Del** — reboot the board. NES gamepad: A — A, B — B, Start — START, Select — SELECT, Start+Select — menu.

The cartridges were written in C for this repository (sources and build in [`tools/gb`](tools/gb/README.md), terms in [`LICENSE`](LICENSE)); they are the same programs as for the PC Engine and Mega Drive in sections 14 and 15. On-screen text is in Latin letters.

| File | Program |
|---|---|
| `hello.gb` | text and the four screen shades, a clock in seconds, shows the pressed buttons; A and B — sound |
| `sprite.gb` | arrows move a robot, a ball flies by itself; touch the ball — a point, A — another robot palette |
| `catch.gb` | the game "Catch the stars": START (**Enter**) — start, ←/→ — basket, three misses — game over |

## 17. Programs for pico-lynx (`/LYNX`)

pico-lynx (an Atari Lynx emulator, Handy core) lists `.lnx` cartridges (and `.com` and `.o` programs) from the `/LYNX` folder and keeps settings and saves there too. A selected cartridge is written to the board's flash. The Lynx boot ROM is not needed: the emulator does its job itself.

Controls: arrows (or WASD) — D-pad, **X** — button A, **Z** — button B (they can be swapped in the menu — Swap AB), **Enter** — OPTION 2, **Esc** or **Backspace** — OPTION 1; **Enter+Esc** — emulator menu (game snapshots, clock, **Reset to ROM select** — back to cartridge selection, **Return to game** — back to the game); **Ctrl+F1–F8** — save the game to a slot, **Alt+F1–F8** — load; **Ctrl+Alt+Del** — reboot the board. The PAUSE button is not mapped in this build. NES gamepad: A — A, B — B, Start — OPTION 2, Select — OPTION 1, Start+Select — menu.

The cartridges were written in C for this repository (sources and build in [`tools/lynx`](tools/lynx/README.md), terms in [`LICENSE`](LICENSE)); they are the same programs as in sections 14–16. On-screen text is in Latin letters.

| File | Program |
|---|---|
| `hello.lnx` | text and colours, a clock in seconds, shows the pressed buttons; A and B — sound |
| `sprite.lnx` | arrows move a robot, a ball flies by itself; touch the ball — a point, A — another robot palette, B — a bigger robot (2 and 3 times) |
| `catch.lnx` | the game "Catch the stars": A — start, ←/→ — basket, three misses — game over |

## 18. Disk for the Mac 128K (`/m128`)

pico-mac (a Macintosh emulator, umac core; in this build a Mac with 208 KB of memory and a 512 × 342 screen) boots from the disk image `/m128/umac0.img`. **The image is not part of the reference card:** it contains Apple system software and programs (System 3.2, Finder, MacPaint) that are distributed without an open licence. Without the image on the card the emulator boots from a 400 KB floppy built into the firmware (System 3.2, Finder 5.3) — read-only and with almost no programs.

Where to get the image: the file `umac0.img` — "a Mac 128K boot disk with games and programs, including MacPaint" — is published by the BlueSCSI community at [picomac.bluescsi.com](https://picomac.bluescsi.com/) (the PicoMicroMac project); the same file is attached to the releases of [DnCraptor/pico-mac-](https://github.com/DnCraptor/pico-mac-/releases) (v.0.0.4–v.0.0.6). Download it and put it into the `/m128` folder under the name `umac0.img`.

What is on the image (disk Untitled, 32 MB; for [teaching module 17](program/modules/17-mac-gui.md)):

| Folder | What is in it |
|---|---|
| `System Folder` | System 3.2, Finder 5.3, Font/DA Mover; in the Apple menu — Calculator, Alarm Clock, Key Caps, Scrapbook, Control Panel, Chooser |
| `Programs` | MacPaint 1.3 (drawing), MacWrite (word processor) |
| `Files` | examples: a MacPaint picture and a MacWrite text (`Sample Memo`) |
| `Games` | about 30 games from the mid-1980s: Lode Runner, Towers of Hanoi, Reversi, Solitaire, Sargon III (chess), Webster's Revenge (words), ChipWits, Asteroids, Missile Command and others |

Offer children only the games the teacher has started on a workstation themselves. Transylvania is a text adventure about a castle with a vampire; do not offer it to the youngest. The emulator opens the image for writing: everything the children draw and save stays in it; the original state is restored by writing the master image of the card before the lesson (section 1). For older cards the emulator also accepts the image in the root of the card (`/umac0.img`).

Controls (PS/2 or USB keyboard):

| PC keys | Mac |
|---|---|
| letters, digits, Shift, Enter, Backspace, Tab, Esc | the same keys |
| **Win** (left or right) | **Command** (⌘) |
| **Alt** | **Option** |
| **Ctrl+Alt+Del** (left Ctrl and Alt) | reboot the board |

Mouse: a USB HID mouse. Both a keyboard and a mouse can be connected to the single USB port of the PCp2 through a USB hub with an OTG adapter; with a PS/2 keyboard the mouse is plugged into USB directly. **Without a mouse** the pointer is moved with the keyboard's numeric keypad (NumPad):

| NumPad | Mouse |
|---|---|
| **8** | up |
| **2** or **5** | down |
| **4** | left |
| **6** | right |
| **Enter** (on the keypad) | mouse button: hold it — the button is held down (dragging, menus) |

A short press moves the pointer by one dot; holding a key accelerates the pointer — across the whole screen in about a second and a half. For diagonals hold two keys at once, for example 8 and 6. Keypad keys may also reach the Mac as digits: in text it is better to type digits with the top row of the keyboard. An NES gamepad on UEXT works the same way: D-pad — pointer, A — mouse button, Start+Select — reboot.

## 19. Programs for pico-gamate (`/GAMATE`)

pico-gamate (a Gamate emulator, core from MAME) lists `.bin` cartridges from the `/GAMATE` folder. A selected cartridge is written to the board's flash. The Gamate BIOS is built into the firmware: after power-on the console shows its splash screen and then starts the cartridge.

Controls: arrows (or WASD) — D-pad, **Z** — button A, **X** — button B, **Enter** — START, **Esc** or **Backspace** — SELECT; **Enter+Esc** — emulator menu (game snapshots, clock, cartridge selection); **F10** — cartridge list; Ctrl+F1–F8 / Alt+F1–F8 — save / load the game; **Ctrl+Alt+Del** — reboot the board.

The cartridges were written in C for this repository (sources and build in [`tools/gamate`](tools/gamate/README.md), terms in [`LICENSE`](LICENSE)); they are the same programs as for the PC Engine, Mega Drive, Game Boy and Lynx in sections 14–17. On-screen text is in Latin letters.

| File | Program |
|---|---|
| `hello.bin` | text and the four screen shades, a clock in seconds, shows the pressed buttons; A and B — sound |
| `sprite.bin` | arrows move a robot, a ball flies by itself; touch the ball — a point, A — other robot shades, B — a robot twice as big |
| `catch.bin` | the game "Catch the stars": A — start, ←/→ — basket, three misses — game over |

## 20. Programs for pico-watara (`/WATARA`)

pico-watara (a Watara Supervision emulator, Potator core) lists `.sv` and `.bin` cartridges from the `/WATARA` folder and keeps game snapshots there too. A selected cartridge is written to the board's flash.

Controls: arrows (or WASD) — D-pad, **Z** — button A, **X** — button B, **Enter** — START, **Esc** or **Backspace** — SELECT; **Enter+Esc** — emulator menu (game snapshots, clock, cartridge selection); **F10** — cartridge list; Ctrl+F1–F8 / Alt+F1–F8 — save / load the game; **Ctrl+Alt+Del** — reboot the board.

The cartridges were written in C for this repository (sources and build in [`tools/watara`](tools/watara/README.md), terms in [`LICENSE`](LICENSE)); they are the same programs as in sections 14–17 and 19. On-screen text is in Latin letters.

| File | Program |
|---|---|
| `hello.sv` | text and the four screen shades, a clock in seconds, shows the pressed buttons; A and B — sound |
| `sprite.sv` | arrows move a robot, a ball flies by itself; touch the ball — a point, A — other robot shades, B — a robot twice as big |
| `catch.sv` | the game "Catch the stars": A — start, ←/→ — basket, three misses — game over |

## 21. Game for pico-wonderswan (`/WS`)

pico-wonderswan (an emulator of the WonderSwan and WonderSwan Color, Oswan core) lists `.ws` and `.wsc` cartridges from the `/WS` folder.

Controls: arrows (or WASD) — the X D-pad, **X** — button A, **Z** — button B, **Enter** — START; **F10** — cartridge list, **F11** — console-shaped frame on/off, F1–F9 and F12 — palette; Ctrl+F1–F8 / Alt+F1–F8 — save / load the game; **Ctrl+Alt+Del** — reboot the board.

| File | Game | Source | Licence |
|---|---|---|---|
| `wondersnake.wsc` | WonderSnake — a snake game for the WonderSwan Color (Tomasz Słanina, 2007), 20 levels, level passwords. In the menu ↑/↓ — START or PASSWORD, Enter (START) or A — select; the D-pad steers the snake | sources at [tslanina/Retro-WonderSwanColor-Wondersnake](https://github.com/tslanina/Retro-WonderSwanColor-Wondersnake) (`56309a6`), built with free tools (JWasm instead of Turbo Assembler) — [`tools/ws`](tools/ws/README.md) | GPLv3 |

## 22. Networking for murm386 (`/dosppp`, `/MTCP`, `/386`)

Material of [teaching module 35](program/modules/35-communication.md). murm386 routes the guest machine's COM1 to the USB-UART (CH340) of an ESP32 board if **USB modem: < COM1 >** is selected in **Win+F11** and USB is in HOST mode; the Zimodem firmware turns the ESP32 into a Wi-Fi modem with Hayes AT commands and a PPP server. Tested on the PCp2 with the murm386 build with PSRAM and the 286 core (`/emu/psram/PCp2-286-VGA256-…`) and ESP32-WROOM / ESP32-WROVER boards with a CH340 bridge; the 386 build with PSRAM has the same COM1 but has not been tested with the modem. The paths in the batch files and `MTCP.CFG` assume that the folders are in the root of the card — `C:\DOSPPP` and `C:\MTCP`.

| Path on the card | Program | Source | Distribution terms |
|---|---|---|---|
| `/386/Zimodem-4.0.3.2-WROVER-USB-UART0.bin` | Zimodem 4.0.3.2 — ESP32 firmware: a Hayes modem emulator over Wi-Fi, PPP and SLIP modes. A merged esptool image (bootloader at 0x1000, partition table, application), main port UART0 (the board's USB-UART). Written to the ESP32 from the murm386 Disk Manager (**Win+F12**, **USB modem** line) | [DnCraptor/Zimodem](https://github.com/DnCraptor/Zimodem), tag `v.4.0.3.2` (`0356c38`) — a fork of [bozimmerman/Zimodem](https://github.com/bozimmerman/Zimodem) 4.0.3: main port UART0, debug output disabled, PPP at 9600 bit/s fixed | Apache 2.0 (Bo Zimmerman); the firmware includes libssh2 — BSD 3-Clause; `LICENSE` and `NOTICE` in the repository |
| `/dosppp/*` | DOS PPPD 0.6 beta (Antonio Lopez Molero, 1997) — PPP packet drivers: `EPPPDD.EXE` (Ethernet emulation, with debugging — started by `PPP9600.BAT`), `EPPPD.EXE`, `PPPD.EXE`, `PPPDD.EXE`, `CHAPSUPP.ZIP` — the same drivers with CHAP; `CHAT.EXE`, `CHAT0.EXE` — scripted dialling; `VJCSTAT.EXE`; documentation `README.TXT`, `PPPD.MAN`, `CHAT.MAN`, `SAMPLES.TXT` | the DOS PPPD 0.6 distribution | free distribution for **non-commercial** use, keeping the copyright notice (`README.TXT`, LICENSING section); based on Carnegie Mellon University's PPP code (BSD-like licence); `CHAT` is public domain |
| `/dosppp/COMTOOL.COM`, `COMTOOL.DOC` | COMTOOL (K. H. Weiss, 1994) — sends commands to the modem and waits for the answer from a batch file, a simple terminal | part of the DOS PPPD distribution | free distribution for **non-commercial** use, not as part of commercial packages |
| `/dosppp/TERMIN.COM`, `PKTSTAT.COM`, `ETHERSL.COM` | from the Crynwr packet driver collection (Russell Nelson): unloading a packet driver, its statistics; EtherSLIP — a SLIP driver with Ethernet emulation (for the `AT+SLIP` mode) | `TERMIN`, `PKTSTAT` — from the DOS PPPD distribution; `ETHERSL.COM` added separately | GPL (Crynwr Packet Driver Collection) |
| `/dosppp/PPP9600.BAT` | connecting: Zimodem at 1200 bit/s → `ATW` (Wi-Fi) → address check `ATI2` → `ATB9600` → `ATE0` → `AT+PPP` → `EPPPDD` on COM1 (0x3F8, IRQ 4, vector 0x60), log in `1.log` | this repository; tested on murm386 286 with PSRAM | [`LICENSE`](LICENSE). **Before taking the master image**, enter the Wi-Fi network name and password in the lines `set WIFI_SSID=` and `set WIFI_PASSWORD=` (placeholders in the repository) |
| `/dosppp/IP-UP.BAT` | the variables `myip`, `remip`, `netmask`, `peermru`; the driver rewrites the file after each connection — the repository holds an example from the test workstation | created by the driver | — |
| `/MTCP/*.EXE`, `SERVICES.BAT`, `*.TXT`, `SAMPLES/*`, `MANUAL/MANUAL.PDF` | mTCP 2024-10-20 (Michael Brutman) — TCP/IP programs for DOS: `DHCP`, `PING`, `HTGET`, `TELNET`, `IRCJR`, `FTP`, `FTPSRV`, `NC`, `SNTP`, `SPDTEST`, `PKTTOOL`, `DNSTEST`; the manual (PDF, 2.9 MB) is for reading on the teacher's computer | [brutman.com/mTCP](http://www.brutman.com/mTCP/mTCP.html), sources in the same place | GPLv3 (`COPYING.TXT`) |
| `/MTCP/MTCP.CFG` | mTCP settings: packet driver on vector 0x60, MTU 1500, address `IPADDR`, netmask, gateway, DNS 1.1.1.1 | the mTCP sample, adapted in this repository | GPLv3. On each station `IPADDR` is replaced with the address of its own modem (shown by `ATI2` and by the `myip` line in `\DOSPPP\IP-UP.BAT`): in PPP mode Zimodem gives the computer its own address in the Wi-Fi network. An example for a `192.168.0.x` network is filled in |
| `/MTCP/IP-UP.BAT` | sets up mTCP for PPP: `MTCPCFG`, `MTCPSLIP=true` (there is no ARP on a serial line), then a test download of `http://example.com/` | this repository | [`LICENSE`](LICENSE) |

## 23. Integrity check

From the root of the repository:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash on Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

`*.md` files are not included in `SHA256SUMS`: git may change their line endings on checkout. Git stores the files under `sdcard/` byte for byte (`.gitattributes`), so the checksums match on any OS.

## 24. Licences

MOS2 and its applications are distributed under GPLv3, sources in murmulator-os2, commit `a77484c`; the TCC compiler is LGPL 2.1 (`/mos2/src/tcc/COPYING`). The terms for FreeDOS, SeaBIOS, the DR-DOS image, games and programs are given in sections 2–22. The repository's own files (documents, `/test/*`) are distributed under [`LICENSE`](LICENSE).
