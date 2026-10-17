# Workstation setup: Murmulator 1.x with RP2350

[Русская версия](README.md) · English

> This is a translation of the Russian [`README.md`](README.md). The Russian text is the reference: if the two differ, the Russian one is newer. [`MANIFEST.md`](MANIFEST.md) and [`../COMPATIBILITY.md`](../COMPATIBILITY.md) are in Russian only.

Status: working instructions, revision 0.1 (2026-10-16). The reference station has been assembled and tested with pico-launcher, MOS2, murm386, PICO-BK, pico-nes, murmapple, pico-z26, atari800, pico-speccy, Game & Watch, murmc64, frank-micro, frank-msx, emu80v4 (Vector-06C, Korvet), pico-pce, pico-genesisPlus, pico-gameboy, pico-lynx, pico-mac, pico-gamate, pico-watara, pico-wonderswan and pico-neogeo-pocket. The board code in firmware names and in the compatibility matrix is `m1p2` (Murmulator 1.x + an RP2350 module, [`../COMPATIBILITY.md`](../COMPATIBILITY.md)).

> **This is not the main board of the class.** The main one is the Olimex RP2040-PICO-PC + Pico 2 (class README, §1.7.1). The Murmulator 1.x is an open community project (class README, §1.7.2): the board is ordered from the Gerber files and assembled yourself, or bought from independent assemblers. There is no factory final inspection, and the quality of soldering and parts varies from unit to unit, so every board is checked before it goes to a station (section 3, item 1). There are many build variants (section 1), and it is better to keep **identical** boards in a class: then the same firmwares and the same jumper positions fit every station.

## 1. The board

The Murmulator 1.x is the original board of the Murmulator project for the Raspberry Pi Pico module, designed by AlexEkb ([murmulator.ru/types](https://murmulator.ru/types), in Russian; [AlexEkb4ever/MURMULATOR_classical_scheme](https://github.com/AlexEkb4ever/MURMULATOR_classical_scheme)). Revisions: [1.0](https://murmulator.ru/mm1-0), [1.1](https://murmulator.ru/mm1-1), [1.2](https://murmulator.ru/mm1-2), [1.3](https://murmulator.ru/mm1-3), [1.4](https://murmulator.ru/mm1-4); in addition there are community variants (including FRANK boards for the M1), bottom boards and adapters. The class needs an **RP2350** module (Raspberry Pi Pico 2 or compatible) — the `m1p2-` builds do not run on the RP2040. The module usually plugs into sockets and is replaced separately from the board; on some units it is soldered on.

The board is **not pin-compatible** with the Murmulator 2.0: firmwares must be built specifically for the M1 (prefix `m1p2-`).

Pins used by the firmwares of the kit (from the `BOARD_M1` profile in `src/board_config.h` of murm386 and the M1 profile in `CMakeLists.txt` of MOS2):

| Block | Pins | Note |
|---|---|---|
| HDMI / VGA | GP6–GP13 | the same set of pins; whether HDMI, VGA or both (through an adapter or a bottom board) are fitted depends on the build variant |
| microSD (SPI0) | SCK GP2, MOSI GP3, MISO GP4, CS GP5 | — |
| PS/2 keyboard | CLK GP0, DATA GP1 | — |
| NES gamepad | CLK GP14, LATCH GP15, DATA GP16 | — |
| I2S sound (TDA1387 DAC) | DATA GP26, BCK GP27, LRCK GP28 | routed on board 1.4, added as a modification on earlier revisions ([murmulator.ru/murmulator-tda](https://murmulator.ru/murmulator-tda)) |
| PWM sound | right GP26, left GP27, beeper GP28 | the same pins as I2S: only one of the two works (section 3.1) |
| Reset, BOOTSEL | buttons on the module or the board | — |

The video, sound and keyboard options differ from unit to unit: HDMI or VGA, PWM or a TDA1387 DAC, PS/2 or USB, including through a hub on the board. So first find out what is fitted on your boards, and choose the manager builds and the jumper positions for that.

## 2. Parts for one workstation

| Component | Qty | Note |
|---|---:|---|
| Murmulator 1.x board | 1 | Assembled and checked; the same build variant across the class |
| RP2350 module | 1 | Raspberry Pi Pico 2 or an electrically compatible module in the Pico form factor with soldered pin headers. The firmwares in `/emu/psram` need a module with PSRAM **on the module itself** — tested with the Pimoroni Pico Plus 2 |
| HDMI or VGA monitor and cable | 1 | Matching the board's video output; 640×480 at 60 Hz must be supported. Single-mode cash-register monitors that do not support 640×480 at 60 Hz will not do |
| USB (HID) or PS/2 keyboard, 101+ keys | 1 | Full-size, with a NumPad. A USB keyboard is connected to the module's micro-USB port through a USB-A (female) → micro-USB (male) OTG adapter, or to the board's hub if it has one; the adapter has to be tested with the specific unit. A PS/2 keyboard goes into the board's PS/2 connector and works with builds without `HID` in their name |
| microSD card | 1 | Formatted FAT32 with a 4 KB cluster (section 4); must be checked for SPI-mode compatibility (most often cards of up to 32 GB) |
| Headphones | 1 | Into the board's audio jack; the teacher's and demonstration stations get powered speakers |
| Power | set | A 5 V power supply with a cable for the power connector of your board. It is best to check the power supply beforehand to avoid ripple problems |
| Mat under the board | 1 | A mouse pad or a piece of soft material: the board has no case, and the mat protects its contacts and the desk (class README, §1.7.8) |

A mouse, NES gamepads and PSRAM are options, not needed for a basic workstation.

## 3. Hardware preparation

1. Inspect the board: the soldering of the connectors, the microSD slot, the audio jack and the module headers, no solder bridges or mechanical damage. The board is home-made or assembled by a third party, so inspecting every unit is required. Before a module goes to a pupil's station, it passes the selection in section 6.3.
2. Set the sound jumpers as in section 3.1.
3. Put the board on a soft mat (a mouse pad): there is no case, and when the buttons are pressed the pins on the back must not be pressed into the desk (class README, §1.7.8).
4. With the power off, insert the module into the board's sockets, observing the orientation of the module outline on the silkscreen (pin 1 is GP0). Do not try to insert a module with incompatible pins.
5. Connect the monitor. **Turn the volume of the monitor's built-in speakers down to zero** (see the note on sound over HDMI below).
6. Plug the headphones into the board's audio jack. Powered speakers at the teacher's and demonstration stations.
7. Prepare the microSD card as in section 4 and insert it into the board's slot.
8. Install the firmware manager as in section 5.
9. Connect the keyboard: USB — through an OTG adapter to the module or to the board's hub, PS/2 — to the PS/2 connector.
10. Connect the power supply. Power on for the first time only after all connections are made.

**Sound over HDMI.** MOS2 outputs sound only to the board's jack, but some firmwares also send sound over HDMI. If the monitors' volume is left up, running such a firmware on several stations at once makes a cacophony for the whole class. So the monitor speaker volume is set to zero when the station is prepared, and checked before each lesson; the pupil hears sound only through headphones. Whether a firmware outputs sound over HDMI is shown in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

### 3.1. Sound: PWM or the TDA1387 DAC

PWM sound and the TDA1387 DAC use the same pins GP26–GP28; only one of the two works on a board:

- **PWM** — the three jumpers that ground the left and right channels and the beeper are in place.
- **TDA1387 DAC (I2S)** — the chip is fitted (board 1.4 has a footprint for it, earlier revisions need a modification) and these three jumpers are removed; they can be moved to free ground pins of the header. To go back to PWM, the jumpers are put back ([murmulator.ru/murmulator-tda](https://murmulator.ru/murmulator-tda)).

The firmware must output sound the same way:

| Firmware | Sound |
|---|---|
| murm386 | selected in the **Win+F11** menu (Audio output): I2S or PWM |
| MOS2 | PWM only (GP26/GP27); on a board switched to the TDA1387 DAC there is no MOS2 sound in the headphones |

### 3.2. Builds with composite output (TV) — demonstration station only

PICO-BK has `m1p2-bk-TV-…` builds, pico-speccy has `m1p2-speccy-TV-SOFT-…`, the Vector-06C has `m1p2-v06c-400-TV-SOFT-PAL-…` and the Korvet has `m1p2-korvet-400-TV-SOFT-PAL-…`: the picture goes to a CRT TV through the composite output — a board modification, a DAC on the same pins GP6–GP13 as VGA and HDMI ([murmulator.ru/mm-composite](https://murmulator.ru/mm-composite), in Russian). **These builds are meant for a separate demonstration station with a CRT TV (class README, §1.7.6) and are not installed on the pupils' stations:** a VGA or HDMI monitor shows nothing with them. It is best not to copy the TV builds to the pupils' cards at all, so that nobody starts one by mistake; if that happens, reboot the board holding F11 and choose another firmware.

**What goes on the demonstration station.** Only machines that really worked with a TV in their day: the BK-0010, ZX Spectrum, Vector-06C and Korvet. The CRT TV is part of the lesson about how such machines looked and worked. Handheld consoles with a built-in LCD (Gamate, Watara Supervision and the like) never worked with a TV, so their TV builds are not part of the kit, even when the releases include them.

**Sound chips (`HWAY`).** PICO-BK builds with `HWAY` in their name and pico-gamate builds with `AY-3-8910` in their name output sound to hardware sound chips (AY-3-8910 and SAA1099 for PICO-BK, AY-3-8910 for pico-gamate) on an expansion board through a 74HC595 shift register on GP26–GP28. Without such a board they have no sound — use the `I2S` or `PWM` build matching the jumpers of section 3.1.

## 4. Preparing the SD card

1. Save any data you need from the card: formatting erases the existing content.
2. Create a partition with the **FAT32 file system and a 4 KB cluster size** (4096 bytes). With a different cluster size, FreeDOS (which murm386 boots) may have problems. Factory formatting and the SD Card Formatter program usually set a 32 KB cluster for 16–32 GB cards, so format the card yourself:
   - Windows: `format X: /FS:FAT32 /A:4096 /Q` (X is the card's drive letter), or in the "Format" window choose FAT32 and an "Allocation unit size" of 4096 bytes;
   - Linux: `sudo mkfs.vfat -F 32 -s 8 -S 512 /dev/sdX1`;
   - macOS: `sudo newfs_msdos -F 32 -c 8 /dev/diskXs1` (after `diskutil unmountDisk`).
3. Copy the files to the card in two steps — the order matters:
   1. the **contents** of the common [`sdcard/`](../../sdcard/) folder from the root of the repository — to the root of the card. This is the part common to all boards; the content is described in [`../../SDCARD.en.md`](../../SDCARD.en.md);
   2. then the **contents** of this board's [`sdcard/`](sdcard/) folder (`hardware/murmulator1x/sdcard`) — on top, to the root of the same card. It holds the emulators built for the `m1p2` (`/emu`) and the MOS2 quick guide for this board (`/mos2/README.md`, in Russian); where names match, the board's files replace the common ones. The content is listed in [`MANIFEST.md`](MANIFEST.md).
4. Add only firmwares built specifically for the **m1p2**. Builds for other boards (with the prefixes `PCp2-`, `z0p2-`, `m2p2-`) do not work here: they have different video, SD-card, keyboard and sound pins. MOS2 recognises its board's firmwares by the `.m1p2` extension (case-sensitive); both managers also start ordinary `.uf2` files. It is recommended to name the kit's firmwares with the `m1p2-` prefix, as in the `sdcard/emu` folder.
5. Put learning programs, data and games into separate folders. The content of the kit and the test status of the firmwares are in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).
6. Before making copies, the integrity of the reference set is checked against two files: [`SHA256SUMS`](../../SHA256SUMS) in the root of the repository — the common part (see `SDCARD.en.md`, section 23), and [`SHA256SUMS`](SHA256SUMS) in the board folder — the board's UF2 files (see `MANIFEST.md`, section 3).

The same card can serve boards of different types if it holds emulators for each: the files differ by prefix, and murm386 keeps its settings for each board separately (for this one, in `/.config/386/M1/`). The `/mos2/README.md` guide will then be the one of the board whose folder was copied last.

## 5. Installing the firmware manager

One of two managers is installed on the module. Each has builds for the way the keyboard is connected (`HID` in the name — USB keyboard, no `HID` — PS/2), and pico-launcher also for the video output:

- **pico-launcher — the main option** for the beginner level: [`pico-launcher/m1p2-uf2-launcher-HDMI-HID.uf2`](pico-launcher/) (for a board with VGA — `…-VGA-HID.uf2`; with a PS/2 keyboard — `…-HDMI.uf2` or `…-VGA.uf2`). A file browser for the SD card with help (F1), a viewer (F3) and a simple editor (F4) for text files; it starts the firmwares of the kit. Acceptance — section 6.1.
- **MOS2 (murmulator-os2) — the advanced option** for lessons with a command line, a file system and BASIC: [`murmulator-os2/m1p2-murmulator-os-VGA-HDMI-HID-2.3.3-252MHz-1.uf2`](murmulator-os2/) (with a PS/2 keyboard — the build without `HID`). MOS2 detects the video output by itself — one build works with both HDMI and VGA. Acceptance — section 6.2.

The TCC C compiler that comes with MOS2 is described in [`../../SDCARD.en.md`](../../SDCARD.en.md), section 2.

The versions, sources and checksums are in [`MANIFEST.md`](MANIFEST.md).

The first installation is done through the standard RP2350 boot loader (BOOTSEL mode):

1. With USB disconnected, hold the **BOOTSEL** button on the module.
2. Connect the module to a computer with a USB cable.
3. Release BOOTSEL once the boot loader's USB drive appears.
4. Copy the manager's UF2 file to the drive.

The module can also be flashed separately from the board and then inserted into the sockets.

In pico-launcher and MOS2, holding **F11** at power-on or reboot prevents the previously chosen firmware from starting automatically and opens the file manager, where you can choose the target firmware — for example, a game or an emulator.

**If something does not load** (black screen, the wrong firmware starts straight away, the manager "goes dark" with the SD card inserted), do this:

1. Power on or reboot the board while holding **F11**.
2. If it still does not load with F11, take out the SD card, delete the file `/.firmware` from it on a computer and put the card back. This marker file is left on the card after a firmware is started from the manager and makes the board go back into that firmware; deleting it does no harm — the manager creates it again the next time a firmware is chosen.

The usual way to reboot is **Ctrl+Alt+Del**: pico-launcher, MOS2 and murm386 support it. The reset button and unplugging the power are needed only if a firmware has hung or does not react to the keyboard.

## 6. Workstation acceptance

Acceptance is done for the manager installed on the station: 6.1 — pico-launcher (the main option), 6.2 — MOS2. Common preconditions: the card is prepared as in section 4, the manager is installed as in section 5, and the headphones are not on tightly while the sound is checked — first make sure the volume is moderate.

If there is no sound, first rule out the headphones: listen to them on another source (a phone), wiggle the plug. Then check that the sound jumpers (section 3.1) match the firmware's sound output. If the manager does not load, follow the order in section 5 ("If something does not load").

### 6.1. Workstation with pico-launcher

| # | Check | Actions | Expected result |
|---:|---|---|---|
| 1 | Boot | Power on; if a firmware was chosen before, hold **F11** | The SD-card file browser, the `/mos2` and `/test` folders are visible, no SD error message |
| 2 | Video | Look at the screen | The picture is stable, without dropouts or flicker, text is not cut off at the edges |
| 3 | Navigation | Arrows, PgUp/PgDn, Home/End; Enter on the `/test` folder | The selection moves; the folder opens |
| 4 | Help | **F1**, then Esc | The help window opens and closes |
| 5 | Viewer | On `/test/edit.txt` — **F3**, arrows, then **F10** | The file text is visible and scrolls; F10 returns to the browser |
| 6 | Editor, keyboard and writing to SD | On `/test/edit.txt` — **F4**; type letters, digits, digits on the NumPad (with NumLock), Enter, Backspace; Ins; **F2**; Esc; then F3 again | All typed characters appear; Ins toggles INS/OVR; after F2 and exiting, the viewer shows the saved changes |
| 7 | Delete with cancel | On `/test/edit.txt` — **F8**, then Esc | A confirmation prompt appears; after Esc the file is still there |
| 8 | Starting a firmware | Enter on `/emu/m1p2-386-RUNTIME-…`; then **Ctrl+Alt+Del** while holding **F11** | murm386 starts and reboots on Ctrl+Alt+Del; with F11 — the browser again |
| 9 | Sound | Start murm386 (`/emu/m1p2-386-RUNTIME-…`), in **Win+F11** choose the sound output matching the board's jumpers (I2S or PWM) | Sound in the headphones. The launcher itself outputs no sound |
| 10 | PICO-BK sound through the DAC (only on a board with the TDA1387 DAC, section 3.1) | Start `/emu/m1p2-bk-VGA-HDMI-400MHz-I2S-…` and a game with sound; in the **Home** menu check the sound volume item and, if it is 0 or lower, raise it to 1 or higher (in the menu or with **Ctrl+"+"**) | Sound in the headphones at volume 1 and higher. Known bug in this version: at volume 0 and below there is no sound through the DAC (PWM is audible at 0 too). The volume is saved in the PICO-BK settings on the card — leave it at 1 or higher on the reference card |

The launcher does not use F5–F7, F9 and F12. F5–F7 are checked in `mc` during MOS2 acceptance (6.2), the others in the firmwares that use them.

### 6.2. Workstation with MOS2

Commands are typed in the MOS2 console: in `mc` the console is shown and hidden with **Ctrl+O**.

| # | Check | Actions | Expected result |
|---:|---|---|---|
| 1 | Boot | Power on | After a few seconds the monitor shows `mc` (Murmulator Commander), no SD error messages |
| 2 | CPU and memory | `cpu`, then `mem` | Clock 252 MHz; memory information is shown without errors |
| 3 | Video | `mode 0`, `mode 1`, `mode 2`, then `gmode 2`, then `mode 0` | In each mode the picture is stable, without dropouts or flicker, and the monitor does not say "no signal". `gmode 2` shows a colour test chart and reports the driver's graphics resolution — write it down |
| 4 | Keyboard | `basic /test/kbdtest.bas`, press keys one at a time; exit with Ctrl+C | Letters and digits print their code and character; Enter, Esc and the NumPad (with NumLock on) print their code. Ctrl+Shift switches the layout. BASIC does not receive the F keys — they are checked in item 4a |
| 4a | F keys | In `mc` on the file `/test/edit.txt`: **F1**, **F3**, **F4** — close each window with Esc; **F5**, **F6**, **F7**, **F8** — cancel each prompt with Esc | Each key opens its window or prompt; after cancelling, files and folders are unchanged |
| 5 | SD: reading and writing | `ls /`, `cp /mos2/config.sys /tmp/t.txt`, `type /tmp/t.txt`, `rm /tmp/t.txt` | The `/mos2` and `/test` folders are visible; the copy is created, the content of `config.sys` is printed, the file is deleted |
| 6 | Sound, mono | `wav /test/snd_mono.wav` (only on a board with PWM sound, section 3.1) | The file parameters are shown (8000 Hz, 1 channel); three tones rising in pitch, in both ears, without crackling. If MOS2 does not respond after playback, reset the board |
| 7 | Sound, channels | `wav /test/snd_lr.wav` (likewise) | The low tone only on the left, the middle one only on the right, the high one in both. If the channels are swapped, note it in `COMPATIBILITY.md` |
| 8 | Switching firmwares | In `mc` choose a firmware from `/emu`, Enter; then **Ctrl+Alt+Del** while holding **F11** | The firmware starts and reboots on Ctrl+Alt+Del; with F11 — MOS2 again |
| 9 | Reboot | Ctrl+Alt+Del | MOS2 boots again, as in item 1 |

The MOS2 quick guide for this board is `/mos2/README.md` on the card (in Russian, CP866, readable right in MOS2: `type /mos2/README.md | less`).

### 6.3. Selecting modules for overclocking

The module plugs into sockets, so it is the **module** that is selected, not the whole board; modules selected on other boards of the class are good here too. The emulators run the RP2350 core overclocked and at a raised voltage, and RP2350 chips differ in how far they can be overclocked, so every module is checked before it goes to a workstation. The selection threshold is the most demanding firmware of the kit, currently murm386: 504 MHz / 1.6 V.

| # | Firmware | Mode | Actions | Criterion |
|---:|---|---|---|---|
| 1 | murm386 | 504 MHz / 1.6 V (Win+F11 menu) | Start `/emu/m1p2-386-RUNTIME-…uf2`*, wait for Volkov Commander; start `\FREEDOS\SI.EXE`; leave it running for at least 10 minutes | FreeDOS boots, SI shows the performance index, the system does not hang, the sound has no artefacts |

\* if the module has PSRAM, it makes sense to choose the firmwares from the `/emu/psram` folder: they are adapted to PSRAM and faster than the universal build. Of the 286/386 emulators, the 286 variant gives the best performance — its emulator is "lighter".

Modules that pass the check go to the pupils' stations; the others are labelled and used as spares for light tasks (the managers run at 252 MHz) or at the teacher's station.

Overclocking and voltages above nominal are outside the modes guaranteed by the manufacturer and may reduce the chip's stability over time. The check is repeated once a school term and whenever unexplained hangs appear on a workstation.

The acceptance result is recorded in [`../COMPATIBILITY.md`](../COMPATIBILITY.md): date, board and module unit, items passed, remarks.

## 7. Rolling out to the whole class

After the reference station has passed acceptance, record the board build variant (revision, video, sound, keyboard, jumpers), the versions of the manager and the firmwares, the content of the SD card, the settings and the test results. Make copies of the reference SD card for the other stations; check booting and sound on each unit. Do not take a check of one sample as proof that the whole batch works: the boards are assembled by hand, and a soldering fault on one unit says nothing about another.

The spare part is the **module** with the manager already installed: if the pico-launcher or MOS2 firmware is damaged, it is faster to swap the module during the lesson than to reflash it, and deal with the repair at a calmer time. It is good to have one spare board as well.

Be prepared for children to mess up the data on the SD card: it is best to have spare cards prepared in advance for quick replacement without holding up the lesson.

## Files in this folder

| Path | What is here |
|---|---|
| [`MANIFEST.md`](MANIFEST.md) | what is in the board folder, what sources it was built from, checksums (in Russian) |
| [`SHA256SUMS`](SHA256SUMS) | checksums of the board's files (how to check — [`MANIFEST.md`](MANIFEST.md), section 3) |
| [`pico-launcher/`](pico-launcher/) | pico-launcher builds for the `m1p2`: HDMI or VGA, USB keyboard (`HID`) or PS/2 |
| [`murmulator-os2/`](murmulator-os2/) | MOS2 builds for the `m1p2`: USB keyboard (`HID`) or PS/2 |
| [`sdcard/emu/`](sdcard/emu/) | emulators built for the `m1p2`: murm386 (three builds, two of them in `psram/`), PICO-BK (six builds: VGA/HDMI or TV, sound I2S, PWM or sound chips — `HWAY`; the TV builds are for the demonstration station only, section 3.2), pico-nes (NES / Dendy: two builds — I2S or PWM sound), murmapple (Apple IIe: eight builds — HDMI or VGA, I2S or PWM sound, four of them with PSRAM in `psram/`), pico-z26 (Atari 2600: four builds — HDMI or VGA, I2S or PWM sound), atari800 (Atari 400/800, XL/XE, 5200: four builds — HDMI or VGA, I2S or PWM sound), pico-speccy (ZX Spectrum: a VGA/HDMI build and a TV build — demonstration station only, section 3.2), murmulator_game_n_watch (Game & Watch: one build for HDMI, VGA and TFT, I2S or PWM sound chosen in the settings), murmc64 (Commodore 64: eight builds — HDMI or VGA, I2S or PWM sound, four of them with PSRAM in `psram/`), frank-micro (BBC Micro: one build for HDMI and VGA, I2S or PWM sound chosen with F12), frank-msx (MSX / MSX2 / MSX2+: one build for HDMI and VGA, I2S or PWM sound chosen with F12), emu80v4 Vector-06C and Korvet (HDMI, VGA and TV builds of each — the TV builds are for the demonstration station only, section 3.2), pico-pce (PC Engine: four builds — HDMI or VGA, I2S or PWM sound), pico-genesisPlus (Mega Drive: two builds — HDMI only, I2S or PWM sound, no PS/2 keyboard), pico-gameboy (Game Boy / Game Boy Color: four builds — HDMI or VGA, I2S or PWM sound), pico-lynx (Atari Lynx: four builds — HDMI or VGA, I2S or PWM sound), pico-mac (Macintosh 128K: four builds — HDMI or VGA, I2S or PWM sound), pico-gamate (Gamate: six builds — HDMI or VGA, I2S, PWM or an AY-3-8910 sound chip on an expansion board), pico-watara (Watara Supervision: four builds — HDMI or VGA, I2S or PWM sound), pico-wonderswan (WonderSwan: four builds — HDMI or VGA, I2S or PWM sound), pico-neogeo-pocket (Neo Geo Pocket / Color: four builds — HDMI or VGA, I2S or PWM sound) |
| [`sdcard/mos2/`](sdcard/mos2/) | MOS2 quick guide for this board (in Russian, CP866) |

## Sources

Most sources below are in Russian.

- [Murmulator: board types](https://murmulator.ru/types) — revisions 1.0–1.4, modifications (PSRAM, I2S sound, composite output), community boards
- [Murmulator 1.4](https://murmulator.ru/mm1-4) — schematic, Gerber files, interactive bill of materials, bottom boards, cases
- [Modification: TDA1387 DAC](https://murmulator.ru/murmulator-tda) — connecting it to GP26–GP28 and the sound jumpers
- [AlexEkb4ever/MURMULATOR_classical_scheme](https://github.com/AlexEkb4ever/MURMULATOR_classical_scheme) — the board project
- Connection diagrams and community materials — [Telegram: ZX_MURMULATOR_DATA](https://t.me/ZX_MURMULATOR_DATA); support group — [Telegram: ZX_MURMULATOR](https://t.me/ZX_MURMULATOR)
- [murm386: the `BOARD_M1` board profile](https://github.com/DnCraptor/murm386/blob/main/src/board_config.h)
