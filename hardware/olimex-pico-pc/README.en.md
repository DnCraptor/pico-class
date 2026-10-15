# Workstation setup: Olimex RP2040-PICO-PC + Raspberry Pi Pico 2

[Русская версия](README.md) · English

> This is a translation of the Russian [`README.md`](README.md), made from commit `6253b68`. The Russian text is the reference: if the two differ, the Russian one is newer. [`MANIFEST.md`](MANIFEST.md) and [`../COMPATIBILITY.md`](../COMPATIBILITY.md) are in Russian only.

Status: working instructions, revision 0.2 (2026-10-10). Minimal configuration: all core modules of the programme run on it.

## 1. Parts for one workstation

| Component | Qty | Note |
|---|---:|---|
| Olimex RP2040-PICO-PC | 1 | Main board |
| Raspberry Pi Pico 2 | 1 | or a fully electrically compatible clone with the RP2350 MCU, with soldered pin headers, for example: WeAct / Waveshare RP2350-Plus, Pimoroni Pico 2 Plus |
| HDMI monitor | 1 | With a compatible video mode (640×480 at 60 Hz must be supported), an HDMI input and an HDMI–HDMI cable; the BK emulator (PICO-BK) needs 720×576 at 50 Hz (CEA 576p) by default, and selecting Pico 2 modules (section 5.3) also needs 800×600 and 1024×768 at 60 Hz; single-mode cash-register monitors that do not support 640×480 at 60 Hz will not do |
| USB HID keyboard, 101+ keys | 1 | Full-size, with a NumPad |
| microSD card | 1 | Formatted FAT32 with a 4 KB cluster (section 3); must be checked for SPI-mode compatibility (most often cards of up to 32 GB) |
| Headphones | 1 | For the pupil; the teacher's and demonstration stations get powered speakers |
| Cables and power | set | A 5 V power supply with a micro-USB cable for the power connector of the Olimex board (testing beforehand is recommended to avoid ripple problems). The USB keyboard is connected to the micro-USB port of the Pico 2 module through a USB-A (female) → micro-USB (male) OTG adapter; the adapter has to be tested with the specific unit, and in some cases a separately powered USB OTG hub may be needed |
| Mat under the board | 1 | A mouse pad or a piece of soft material: the board has no case, and the mat protects its contacts and the desk (class README, §1.7.8) |

External PSRAM, a USB mouse and an NES DPAD are options, not needed for a basic workstation.

## 2. Hardware preparation

1. Inspect the boards and connectors for mechanical damage. Before a Pico 2 module goes to a pupil's station, it passes the selection in section 5.3.
2. Put the main board on a soft mat (a mouse pad): there is no case, and when RESET and BOOTSEL are pressed the pins on the back must not be pressed into the desk (class README, §1.7.8).
3. With the power off, insert the Pico 2 into the sockets of the Olimex main board, observing the orientation marked by Olimex. Do not try to insert a module with incompatible pins.
4. Connect the HDMI monitor. **Turn the volume of the monitor's built-in speakers down to zero** (see the note on sound below).
5. Connect the headphones to the standard audio output. Use powered speakers at the teacher's and demonstration stations.
6. Prepare the microSD card as in section 3 and insert it into the board's slot.
7. Prepare the Pico 2 module as in section 4.
8. Connect the USB HID keyboard to the Pico 2 (with a USB OTG adapter/hub if needed).
9. Connect power as described in the board documentation. Power on for the first time only after all connections are made.

Note: according to the manufacturer's documentation the RP2040-PICO-PC has HDMI, audio, microSD and USB OTG. Support for each device also depends on the chosen firmware.

**Sound over HDMI.** MOS2 outputs sound only to the board's jack, but some firmwares also send sound over HDMI. If the monitors' volume is left up, running such a firmware on several stations at once makes a cacophony for the whole class. So the monitor speaker volume is set to zero when the station is prepared, and checked before each lesson; the pupil hears sound only through headphones. Whether a firmware outputs sound over HDMI is shown in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 3. Preparing the SD card

1. Save any data you need from the card: formatting erases the existing content.
2. Create a partition with the **FAT32 file system and a 4 KB cluster size** (4096 bytes). With a different cluster size, FreeDOS (which murm386 boots) may have problems. Factory formatting and the SD Card Formatter program usually set a 32 KB cluster for 16–32 GB cards, so format the card yourself:
   - Windows: `format X: /FS:FAT32 /A:4096 /Q` (X is the card's drive letter), or in the "Format" window choose FAT32 and an "Allocation unit size" of 4096 bytes;
   - Linux: `sudo mkfs.vfat -F 32 -s 8 -S 512 /dev/sdX1`;
   - macOS: `sudo newfs_msdos -F 32 -c 8 /dev/diskXs1` (after `diskutil unmountDisk`).
3. Copy the files to the card in two steps — the order matters:
   1. the **contents** of the common [`sdcard/`](../../sdcard/) folder from the root of the repository — to the root of the card. This is the part common to all boards: `/mos2` (MOS2 v2.3.3 with applications, the `tcc` C compiler and `config.sys`; there is no need to download `mos2.zip` separately), `/freedos` with games and learning programs for murm386, `/NES` with games for pico-nes, `/ZX` with programs and games for pico-speccy, `/apple` with disk images for the Apple IIe, `/z26` with games for the Atari 2600, `/atari800` with programs for the Atari 400/800, XL/XE and 5200, `/c64` with programs for the Commodore 64, `/micro` with disks and ROMs for the BBC Micro, `/test` with files for workstation acceptance (section 5). The content is described in [`../../SDCARD.en.md`](../../SDCARD.en.md);
   2. then the **contents** of this board's [`sdcard/`](sdcard/) folder (`hardware/olimex-pico-pc/sdcard`) — on top, to the root of the same card. It holds the emulators built for the PCp2 (`/emu`) and the frank-micro settings for this board (`/micro/micro.ini`); where names match, the board's files replace the common ones. The content is listed in [`MANIFEST.md`](MANIFEST.md).
4. Add only firmwares that are compatible specifically with the **PCp2 / Olimex PICO-PC + Pico 2**. Do not mix binaries for the RP2040, the RP2350 and other pinouts without testing. MOS2 and pico-launcher start `.uf2` and `.PCp2` files (the `.PCp2` extension is case-sensitive); it is recommended to give the kit's firmwares the `.PCp2` extension, so that the name shows which board they were built for.
5. Put learning programs, data and games into separate folders. The content of the kit and the test status of the firmwares are in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).
6. Before making copies, the integrity of the reference set is checked against two files: [`SHA256SUMS`](../../SHA256SUMS) in the root of the repository — the common part (see `SDCARD.en.md`, section 23), and [`SHA256SUMS`](SHA256SUMS) in the board folder — the UF2 files and the board's files (see `MANIFEST.md`, section 3).

## 4. Installing the firmware manager

One of two managers is installed on the Pico 2:

- **pico-launcher — the main option** for the beginner level. A file browser for the SD card with help (F1), a viewer (F3) and a simple editor (F4) for text files; it starts the firmwares of the kit. Copy: `hardware/olimex-pico-pc/pico-launcher/`. Acceptance — section 5.1.
- **murmulator-os2 (MOS2) — the advanced option** for lessons with a command line, a file system and BASIC. Copy: `hardware/olimex-pico-pc/murmulator-os2/`. Acceptance — section 5.2.

The versions, sources and checksums of both files are in [`MANIFEST.md`](MANIFEST.md).

To install the right UF2 on the Pico 2 for the first time, use the standard BOOTSEL mode:

1. With USB disconnected, hold the BOOTSEL button.
2. Connect the Pico 2 to a computer over USB.
3. Release BOOTSEL once the boot loader's USB drive appears.
4. Copy the UF2 file to the drive that appeared.

There is no need to reconnect USB to enter BOOTSEL: while holding BOOTSEL on the Pico 2, briefly press RESET, then release BOOTSEL. If the Pico 2 board itself has no RESET button, use the RESET button on the Olimex main board.

In `pico-launcher` and `murmulator-os2`, holding **F11** at power-on or reboot prevents the previously chosen firmware from starting automatically and opens the file manager, where you can choose the target firmware — for example, a game or an emulator.

**If something does not load** (black screen, the wrong firmware starts straight away, the manager "goes dark" with the SD card inserted), do this:

1. Power on or reboot the board while holding **F11**.
2. If it still does not load with F11, take out the SD card, delete the file `/.firmware` from it on a computer and put the card back. This marker file is left on the card after a firmware is started from the manager and makes the board go back into that firmware; deleting it does no harm — the manager creates it again the next time a firmware is chosen.

The usual way to reboot is **Ctrl+Alt+Del**: pico-launcher, MOS2 and almost all firmwares of the kit support it (firmwares that cannot do it, or do something else, are usually marked in the corresponding teaching module). The RESET button on the main board and unplugging the power are needed only if a firmware has hung and does not react to the keyboard or to **Ctrl+Alt+Del**.

## 5. Workstation acceptance

Acceptance is done for the manager installed on the station: 5.1 — pico-launcher (the main option), 5.2 — MOS2. Common preconditions: the card is prepared as in section 3, the manager is installed as in section 4, and the headphones are not on tightly while the sound is checked — first make sure the volume is moderate. If there is no sound or it drops out, first check the headphones on another source (a phone) and wiggle the plug and the volume control on the cord: a bad contact there is more common than a firmware bug. If the manager does not load, follow the order in section 4 ("If something does not load": F11, then deleting `/.firmware`).

### 5.1. Workstation with pico-launcher

| # | Check | Actions | Expected result |
|---:|---|---|---|
| 1 | Boot | Power on; if a firmware was chosen before, hold **F11** | The SD-card file browser, the `/mos2` and `/test` folders are visible, no SD error message |
| 2 | Video | Look at the screen | The picture is stable, without dropouts or flicker, text is not cut off at the edges |
| 3 | Navigation | Arrows, PgUp/PgDn, Home/End; Enter on the `/test` folder | The selection moves; the folder opens |
| 4 | Help | **F1**, then Esc | The help window opens and closes |
| 5 | Viewer | On `/test/edit.txt` — **F3**, arrows, then **F10** | The file text is visible and scrolls; F10 returns to the browser |
| 6 | Editor, keyboard and writing to SD | On `/test/edit.txt` — **F4**; type letters, digits, digits on the NumPad (with NumLock), Enter, Backspace; Ins; **F2**; Esc; then F3 again | All typed characters appear; Ins toggles INS/OVR; after F2 and exiting, the viewer shows the saved changes |
| 7 | Delete with cancel | On `/test/edit.txt` — **F8**, then Esc | A confirmation prompt appears; after Esc the file is still there |
| 8 | Starting a firmware | Enter on a firmware in `/emu` (for example, `PCp2-bk-…`); then **Ctrl+Alt+Del** while holding **F11** | The firmware starts and reboots on Ctrl+Alt+Del; with F11 — the browser again |
| 9 | Sound | Start a firmware accepted on this board with sound (status ✅ in `COMPATIBILITY.md`), for example PICO-BK (`/emu/PCp2-bk-…`) or pico-nes (`/emu/PCp2-nes-…`) | Sound in the headphones. The launcher itself outputs no sound |

The launcher does not use F5–F7, F9 and F12. F5–F7 are checked in `mc` during MOS2 acceptance (5.2), the others in the firmwares that use them.

### 5.2. Workstation with MOS2

Commands are typed in the MOS2 console: in `mc` the console is shown and hidden with **Ctrl+O**.

| # | Check | Actions | Expected result |
|---:|---|---|---|
| 1 | Boot | Power on | After a few seconds the monitor shows `mc` (Murmulator Commander), no SD error messages |
| 2 | CPU and memory | `cpu`, then `mem` | Clock 252 MHz; memory information is shown without errors |
| 3 | Video | `mode 0`, `mode 1`, `mode 2`, then `gmode 2`, then `mode 0` | In each mode the picture is stable, without dropouts or flicker, and the monitor does not say "no signal". `gmode 2` shows a colour test chart and reports the driver's graphics resolution — write it down. Return with `mode 0`: on HDMI in `mode 1` the font is displayed with errors (a known MOS2 shortcoming; it does not fail the acceptance) |
| 4 | Keyboard | `basic /test/kbdtest.bas`, press keys one at a time; exit with Ctrl+C | Letters and digits print their code and character; Enter, Esc and the NumPad (with NumLock on) print their code. Ctrl+Shift switches the layout. BASIC does not receive the F keys — they are checked in item 4a |
| 4a | F keys | In `mc` on the file `/test/edit.txt`: **F1**, **F3**, **F4** — close each window with Esc; **F5**, **F6**, **F7**, **F8** — cancel each prompt with Esc | Each key opens its window or prompt; after cancelling, files and folders are unchanged. F2, F9, F11, F12 are not used in `mc`; F11 is checked in item 9 |
| 5 | SD: reading and writing | `ls /`, `cp /mos2/config.sys /tmp/t.txt`, `type /tmp/t.txt`, `rm /tmp/t.txt` | The `/mos2` and `/test` folders are visible; the copy is created, the content of `config.sys` is printed, the file is deleted |
| 6 | Sound, mono | `wav /test/snd_mono.wav` | The file parameters are shown (8000 Hz, 1 channel); three tones rising in pitch, in both ears, without crackling. After playback MOS2 hangs (a known issue, see below) — reset the board |
| 7 | Sound, channels | `wav /test/snd_lr.wav` | The low tone only on the left, the middle one only on the right, the high one in both. If the channels are swapped, note it in `COMPATIBILITY.md`. After playback — reset, as in item 6 |
| 8 | Switching firmwares | In `mc` choose a firmware from the kit, Enter; then **Ctrl+Alt+Del** while holding **F11** | The firmware starts and reboots on Ctrl+Alt+Del; with F11 — MOS2 again |
| 9 | Reboot | Ctrl+Alt+Del | MOS2 boots again, as in item 1 |

Known issues of MOS2 2.3.2 on the PCp2:

- `wav` hangs after playback finishes (tested without PSRAM). The sound is played in full, so for acceptance it is enough to reset the board after each file.
- `blimp` is not used for checking sound: in this build it outputs the signal on GP26, which is not connected to the audio jack.

### 5.3. Selecting Pico 2 modules for overclocking

**Status:** draft; the modes of the stages are refined as firmwares are added to the kit.

Most emulators of the kit run the RP2350 core overclocked and at a raised voltage (see the "Частота / напряжение" (clock / voltage) column in [`../COMPATIBILITY.md`](../COMPATIBILITY.md)). RP2350 chips differ in how far they can be overclocked, so every Pico 2 module is checked before it goes to a workstation. The check is done on an assembled workstation with the SD card from section 3.

**The selection threshold** is set by the most demanding firmware of the kit — the maximum values in the clock / voltage column of the compatibility matrix. Currently this is murm386: 504 MHz / 1.6 V. The threshold is expected to rise as firmwares are added (for voltage — to 1.65 V and above); then the last stage of the check is run at the new values, and the modules selected earlier are checked again.

**Stage length.** As a guide, at least 10 minutes of work without failures. A batch is conveniently checked in a round: start the stage on all stations one after another and come back to the first — if the module has not hung or shown artefacts in that time, the stage is passed.

| # | Firmware | Mode | Actions | Criterion |
|---:|---|---|---|---|
| 1 | PICO-BK | 720×576, 270 MHz / 1.3 V | Start `/emu/PCp2-bk-…uf2` | Stable picture, sound in the headphones |
| 2 | PICO-BK | 800×600, 400 MHz / 1.5 V | Home → HDMI item → space; start a game from the built-in floppy (Tab, arrows, Enter) and leave it running | No hangs, resets, picture or sound artefacts |
| 3 | PICO-BK | 1024×768, 512 MHz / 1.6 V | Home → HDMI → space; repeat item 2 | Same. If the monitor does not show 1024×768, skip the item — item 4 checks the clock |
| 4 | murm386 | the selection threshold, currently 504 MHz / 1.6 V (Win+F11 menu) | Start `/emu/PCp2-386-…uf2`*, wait for Volkov Commander; start `\FREEDOS\SI.EXE`; leave it running | FreeDOS boots, SI shows the performance index, the system does not hang |

\* if PSRAM is fitted on the board, it makes sense to choose the firmwares from the `/emu/psram` folder: they are adapted to PSRAM and faster than the universal build. Of the 286/386 emulators, the 286 variant gives the best performance — its emulator is "lighter".

After the check, set PICO-BK back to the 720×576 video mode (Home → HDMI), so that the card and the module are in their original state.

A module is classed by the highest mode it passed:

| Class | Passed | Suitable for |
|---|---|---|
| A | items 1–4 at the selection threshold | all firmwares of the kit |
| B | items 1–2 | firmwares up to 400 MHz / 1.5 V: pico-launcher, MOS2, PICO-BK (up to 800×600), pico-speccy, pico-genesisPlus, frank-micro, frank-msx, Game & Watch, Korvet, Vector-06C, pico-mac; the 378 MHz / 1.6 V firmwares — pico-nes, pico-z26, atari800, murmc64, murmapple, pico-pce, pico-gameboy, pico-lynx, pico-gamate, pico-watara, pico-wonderswan, pico-neogeo-pocket — check separately |
| C | item 1 | firmwares up to 270 MHz: pico-launcher, MOS2, frank-micro, frank-msx, Game & Watch |

Class A modules go to the pupils' stations. Class B and C modules are labelled and used as spares for light tasks or at the teacher's station. In general, spare modules are useful for quick replacement. If the main pico-launcher / MOS2 firmware is damaged, it is fastest to take out the damaged module for now and swap in another than to reflash it during the lesson, and deal with the repair at a calmer time.

Overclocking and voltages above nominal are outside the modes guaranteed by the manufacturer and may reduce the chip's stability over time. Items 2–4 are repeated once a school term and whenever unexplained hangs appear on a workstation.

The acceptance result is recorded in [`../COMPATIBILITY.md`](../COMPATIBILITY.md): date, board unit, items passed, remarks; for Pico 2 modules — the class according to section 5.3.

**Acceptance status:** the reference PCp2 station was accepted on 2026-10-01–2026-10-06 with the managers and all firmwares of the kit; the results and remarks for each firmware are in [`../COMPATIBILITY.md`](../COMPATIBILITY.md). The acceptance applies to the files in [`MANIFEST.md`](MANIFEST.md); this document does not claim that arbitrary release binaries are compatible with each other. Every new workstation goes through section 5 itself.

## 6. Rolling out to the whole class

After the reference station has passed acceptance, record the versions of the manager and the firmwares, the content of the SD card, the settings and the test results. Make copies of the reference SD card for the other stations; check booting and audio on each unit. Do not take a check of one sample as proof that the whole batch of equipment works.

Be prepared for children to mess up the data on the SD card; it is best to have spare cards prepared in advance for quick replacement without holding up the lesson.

## Sources

- [Olimex RP2040-PICO-PC: hardware project](https://github.com/OLIMEX/RP2040-PICO-PC)
- [murmulator-os2: releases](https://github.com/DnCraptor/murmulator-os2/releases)
- [pico-launcher: README](https://github.com/DnCraptor/pico-launcher)
