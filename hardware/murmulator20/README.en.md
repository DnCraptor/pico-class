# Workstation setup: Murmulator 2.0

[Русская версия](README.md) · English

> This is a translation of the Russian [`README.md`](README.md), made from commit `6253b68`. The Russian text is the reference: if the two differ, the Russian one is newer. [`MANIFEST.md`](MANIFEST.md) and [`../COMPATIBILITY.md`](../COMPATIBILITY.md) are in Russian only.

Status: working instructions, revision 0.1 (2026-10-14). The reference station has been assembled and tested with pico-launcher, MOS2, murm386 and PICO-BK. The board code in firmware names and in the compatibility matrix is `m2p2` (Murmulator 2 + Raspberry Pi Pico 2, [`../COMPATIBILITY.md`](../COMPATIBILITY.md)).

> **This is not the main board of the class.** The main one is the Olimex RP2040-PICO-PC + Pico 2 (class README, §1.7.1). The Murmulator 2.0 is an open community project (class README, §1.7.2): the board is ordered from the Gerber files and assembled yourself, or bought from independent assemblers. There is no factory final inspection, so the quality of soldering and parts varies from unit to unit — every board is checked before it goes to a station (section 2, item 1). On the other hand, the board has everything the class needs without additional modules: its own audio output with a DAC, HDMI (or VGA), a microSD slot, PS/2 and gamepad connectors, and the Pico 2 module plugs into sockets and is replaced separately from the board, as on the Olimex.

## 1. The board

The Murmulator 2.0 (board **38NJU24**, HW 2.0, schematic of November 2024) is a board of the Murmulator project for the Raspberry Pi Pico 2 module (RP2350A). Designed by AlexEkb ([murmulator.ru](https://murmulator.ru/nextgen), in Russian); connection diagram by JAVAVIUA. The board is **not pin-compatible** with the Murmulator 1.x: firmwares must be built specifically for the M2 (prefix `m2p2-`). Differences from the M1, as intended by the designer: GP8 is given to the PSRAM chip select (CS1), GP0 and GP1 are free (COM port, PS/2 mouse or I2C), the PS/2 keyboard moved to GP2 and GP3, and the gamepad connector is universal (2 × NES or 2 × Wii over I2C).

Pico 2 pin assignment (from the schematic `assets/MURMULATOR2_HW20.pdf` and the connection diagram `img/m2p2-pinout.jpg`):

| Block | Pins | Note |
|---|---|---|
| HDMI / VGA | GP12–GP19 | the eight resistors R1–R8 are shared by both connectors: for HDMI all are 270 Ω; for VGA 1 kΩ / 330 Ω / 100 Ω as in the schematic, and jumpers JP1–JP3 closed. The VGA connector (J1) is optional |
| microSD (SPI0) | SCK GP6, MOSI GP7, MISO GP4, CS GP5 | — |
| PSRAM (U3, ESP-PSRAM, 8 MB) | SCLK GP6, SI GP7, SO GP4, CE GP8 | shared bus with the SD card; jumper JP5 disconnects the chip. The board designer asks **not to solder it on**: it is an experiment for programmers |
| PS/2 keyboard (J7, mini-DIN) | CLK → GP2, DATA → GP3 | through a 5 V → 3.3 V level shifter and **jumpers on J2: 31–32 (CLK) and 29–30 (DATA)** — without them the PS/2 keyboard does not work |
| Second PS/2 device and the upper USB-A (J10) in PS/2 mode | CLK → GP0, DATA → GP1 | pins 6 and 2 of the J7 connector (a "keyboard + mouse" splitter) and the USB-A connector; through jumpers on J6. This is **not** a USB host: a USB keyboard in HID mode cannot be connected here |
| Gamepad (J14, DB-9 male) | CLOCK GP20, LATCH GP21, DATA1 GP26, DATA2 GP27 | two NES (Dendy) gamepads through a splitter; the same pins serve as I2C1/UART for Wii gamepads |
| I2S DAC (U2, TDA1387) | DATA GP9, BCK GP10, LRCK GP11 | output to the J12 jack (Audio OUT) through a transistor amplifier |
| PWM sound | GP9, GP10, GP11 | the same J12 output; I2S or PWM is selected with the PWM_EN jumper (section 3.1) |
| Tape input (J13, Audio IN) | LOAD_IN_D → GP22 | through a comparator and jumper J2 17–18; for loading from audio (BK, ZX Spectrum) |
| Reset | RUN (JP4) | the RST button; BOOTSEL is the button on the Pico 2 module itself |

3.3 V power comes from the Pico 2 module's regulator (jumper JP6 3V3_Pico_EN) or from a separate regulator U4 on the board (optional). 5 V power — the J17 connector (USB Type-C or micro-USB, power only) or the J5 PWR 5V pins; the micro-USB port of the Pico 2 module then stays free for the keyboard.

There is a **bottom board** for the main board — an extra board **38EJU24** with HDMI, composite video output, a USB hub and a second gamepad connector. A class workstation does not need it; the designer asks not to solder on the XN297L chip on it (also an experiment).

## 2. Parts for one workstation

| Component | Qty | Note |
|---|---:|---|
| Murmulator 2.0 board (38NJU24) | 1 | Assembled and checked. **Recommended build for the class: HDMI (R1–R8 at 270 Ω), no PSRAM on the board (do not solder U3), no bottom board.** VGA instead of HDMI is optional: both variants are tested |
| Raspberry Pi Pico 2 | 1 | or an electrically compatible RP2350 module in the Pico form factor with soldered pin headers. The firmwares in `/emu/psram` need a module with PSRAM **on the module itself** (for example, Pimoroni Pico Plus 2) — the PSRAM on the board is not used by them |
| HDMI monitor and HDMI → HDMI cable | 1 | 640×480 at 60 Hz must be supported; the BK emulator (PICO-BK) needs 720×576 at 50 Hz (CEA 576p) by default, and selecting Pico 2 modules (section 6.3) also needs 800×600 at 60 Hz. Single-mode cash-register monitors that do not support 640×480 at 60 Hz will not do. For the VGA build — a VGA monitor |
| USB (HID) or PS/2 keyboard, 101+ keys | 1 | Full-size, with a NumPad. A USB keyboard is connected to the micro-USB port of the Pico 2 module through a USB-A (female) → micro-USB (male) OTG adapter — the adapter is needed only for a USB keyboard and has to be tested with the specific unit. A PS/2 keyboard is connected to the board's PS/2 connector (jumpers J2 29–30 and 31–32) and works with firmwares without `HID` in their name (section 3.1) and with emulators that support PS/2 (see `COMPATIBILITY.md`) |
| microSD card | 1 | Formatted FAT32 with a 4 KB cluster (section 4); must be checked for SPI-mode compatibility (most often cards of up to 32 GB) |
| Headphones | 1 | Plugged into the Audio OUT jack (J12); the teacher's and demonstration stations get powered speakers |
| Power | set | A 5 V power supply with a cable for the board's J17 connector (USB Type-C or micro-USB, depending on the build). It is best to check the power supply beforehand to avoid ripple problems |
| Mat under the board | 1 | A mouse pad or a piece of soft material: the board has no case, and the mat protects its contacts and the desk (class README, §1.7.8) |

A mouse, NES gamepads, PSRAM and the bottom board are options, not needed for a basic workstation. The exception is at least one firmware: pico-genesisPlus (Sega Mega Drive) supports neither a PS/2 keyboard nor VGA — it works only over HDMI and is controlled with a USB keyboard or an NES gamepad. On a workstation with a PS/2 keyboard it needs an NES gamepad, and on a VGA build of the board it does not work. The current list of such limitations is in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 3. Hardware preparation

1. Inspect the board: the soldering of the HDMI, microSD, PS/2 and DB-9 connectors, the Audio OUT jack and the module headers, no solder bridges or mechanical damage. The board is home-made or assembled by a third party, so inspecting every unit is required. Before a Pico 2 module goes to a pupil's station, it passes the selection in section 6.3.
2. Set the jumpers as in section 3.1.
3. Put the board on a soft mat (a mouse pad): there is no case, and when the buttons are pressed the pins on the back must not be pressed into the desk (class README, §1.7.8).
4. With the power off, insert the Pico 2 into the board's sockets, observing the orientation of the module outline on the board's silkscreen (pin 1 is GP0). Do not try to insert a module with incompatible pins.
5. Connect the monitor. **Turn the volume of the monitor's built-in speakers down to zero** (see the note on sound over HDMI below).
6. Plug the headphones into the Audio OUT jack (J12). Powered speakers at the teacher's and demonstration stations.
7. Prepare the microSD card as in section 4 and insert it into the board's slot.
8. Install the firmware manager as in section 5.
9. Connect the USB keyboard to the micro-USB port of the Pico 2 module through an OTG adapter.
10. Connect the 5 V power supply to the J17 connector. Power on for the first time only after all connections are made.

**Sound over HDMI.** MOS2 outputs sound only to the board's jack, but some firmwares (PICO-BK) also send sound over HDMI. If the monitors' volume is left up, running such a firmware on several stations at once makes a cacophony for the whole class. So the monitor speaker volume is set to zero when the station is prepared, and checked before each lesson; the pupil hears sound only through headphones. Whether a firmware outputs sound over HDMI is shown in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

### 3.1. Jumpers

![Murmulator 2.0: pin assignment, connectors and jumpers](img/m2p2-pinout.jpg)

| Jumper | Position for the class | Why |
|---|---|---|
| PWM_Enable (J2, pin 14 — PWM_EN, with +3V3 on pin 16 and GND on pin 13 next to it; highlighted in green on the connection diagram) | as described by the board designer (the left jumper): **vertical — I2S sound (TDA1387 DAC)**, horizontal — PWM sound | selects the sound source for the Audio OUT jack. The position must match the firmware: PICO-BK has `…-I2S-…` and `…-PWM-…` builds, murm386 selects the output in the Win+F11 menu |
| J2 31–32 and 29–30 (PS/2 Kbd, highlighted in blue) | close if a PS/2 keyboard is used | connect the PS/2 connector to GP2 (CLK) and GP3 (DATA) |
| J6 10 and 12 | as needed | a second PS/2 device or USB-A in PS/2 mode on GP0/GP1; if a firmware needs these pins for something else (COM port, I2C), remove the jumpers |
| J2 17–18 (LOAD_IN_D, highlighted in pink) | as needed | tape input on GP22 |
| JP1–JP3 | close only in the VGA build | connect the red, green and blue channels of the VGA connector |
| JP5 (PSRAM off) | as built | disconnects the PSRAM on the board; in the recommended build the chip is absent |

Component values in the audio path that the board designer recommends for better sound and that are already given in the schematic: R28, R40 — 620 Ω; R29, R38 — 10 kΩ; R30, R37 — 200 kΩ; R34, R42 — 2 kΩ; R31, R36 — 1 kΩ with a 1 nF capacitor soldered on top.

## 4. Preparing the SD card

1. Save any data you need from the card: formatting erases the existing content.
2. Create a partition with the **FAT32 file system and a 4 KB cluster size** (4096 bytes). With a different cluster size, FreeDOS (which murm386 boots) may have problems. Factory formatting and the SD Card Formatter program usually set a 32 KB cluster for 16–32 GB cards, so format the card yourself:
   - Windows: `format X: /FS:FAT32 /A:4096 /Q` (X is the card's drive letter), or in the "Format" window choose FAT32 and an "Allocation unit size" of 4096 bytes;
   - Linux: `sudo mkfs.vfat -F 32 -s 8 -S 512 /dev/sdX1`;
   - macOS: `sudo newfs_msdos -F 32 -c 8 /dev/diskXs1` (after `diskutil unmountDisk`).
3. Copy the files to the card in two steps — the order matters:
   1. the **contents** of the common [`sdcard/`](../../sdcard/) folder from the root of the repository — to the root of the card. This is the part common to all boards; the content is described in [`../../SDCARD.en.md`](../../SDCARD.en.md);
   2. then the **contents** of this board's [`sdcard/`](sdcard/) folder (`hardware/murmulator20/sdcard`) — on top, to the root of the same card. It holds the emulators built for the `m2p2` (`/emu`); where names match, the board's files replace the common ones. The content is listed in [`MANIFEST.md`](MANIFEST.md).
4. Add only firmwares built specifically for the **m2p2**. Builds for other boards (with the prefixes `PCp2-`, `z0p2-`, `m1p2-`) do not work here: they have different video, SD-card, keyboard and sound pins. MOS2 recognises its board's firmwares by the `.m2p2` extension (case-sensitive); both managers also start ordinary `.uf2` files. It is recommended to name the kit's firmwares with the `m2p2-` prefix, as in the `sdcard/emu` folder.
5. Put learning programs, data and games into separate folders. The content of the kit and the test status of the firmwares are in [`../COMPATIBILITY.md`](../COMPATIBILITY.md).
6. Before making copies, the integrity of the reference set is checked against two files: [`SHA256SUMS`](../../SHA256SUMS) in the root of the repository — the common part (see `SDCARD.en.md`, section 23), and [`SHA256SUMS`](SHA256SUMS) in the board folder — the UF2 files and the board materials (see `MANIFEST.md`, section 4).

The same card can serve boards of different types if it holds emulators for each: the files differ by prefix, and murm386 keeps its settings for each board separately (for this one, in `/.config/386/M2/`).

## 5. Installing the firmware manager

One of two managers is installed on the board. Each has builds for the way the keyboard is connected (`HID` in the name — USB keyboard, no `HID` — PS/2), and pico-launcher also for the video output:

- **pico-launcher — the main option** for the beginner level: [`pico-launcher/m2p2-uf2-launcher-HDMI-HID.uf2`](pico-launcher/) (for the VGA build — `…-VGA-HID.uf2`; with a PS/2 keyboard — `…-HDMI.uf2` or `…-VGA.uf2`). A file browser for the SD card with help (F1), a viewer (F3) and a simple editor (F4) for text files; it starts the firmwares of the kit. Acceptance — section 6.1.
- **MOS2 (murmulator-os2) — the advanced option** for lessons with a command line, a file system and BASIC: [`murmulator-os2/m2p2-murmulator-os-VGA-HDMI-HID-2.3.3-252MHz-1.uf2`](murmulator-os2/) (with a PS/2 keyboard — the build without `HID`). MOS2 detects the video output by itself — one build works with both HDMI and VGA.

The TCC C compiler that comes with MOS2 is described in [`../../SDCARD.en.md`](../../SDCARD.en.md), section 2.

The versions, sources and checksums are in [`MANIFEST.md`](MANIFEST.md).

The first installation is done through the standard RP2350 boot loader (BOOTSEL mode), as on the Olimex:

1. With USB disconnected, hold the **BOOTSEL** button on the Pico 2 module.
2. Connect the module to a computer with a micro-USB cable.
3. Release BOOTSEL once the boot loader's USB drive appears.
4. Copy the manager's UF2 file to the drive.

The module can also be flashed separately from the board and then inserted into the sockets.

In pico-launcher and MOS2, holding **F11** at power-on or reboot prevents the previously chosen firmware from starting automatically and opens the file manager, where you can choose the target firmware — for example, a game or an emulator.

**If something does not load** (black screen, the wrong firmware starts straight away, the manager "goes dark" with the SD card inserted), do this:

1. Power on or reboot the board while holding **F11**.
2. If it still does not load with F11, take out the SD card, delete the file `/.firmware` from it on a computer and put the card back. This marker file is left on the card after a firmware is started from the manager and makes the board go back into that firmware; deleting it does no harm — the manager creates it again the next time a firmware is chosen.

The usual way to reboot is **Ctrl+Alt+Del**: pico-launcher, MOS2 and almost all firmwares of the kit support it. The RST button and unplugging the power are needed only if a firmware has hung or does not react to the keyboard.

## 6. Workstation acceptance

Acceptance is done for the manager installed on the station: 6.1 — pico-launcher (the main option), 6.2 — MOS2. Common preconditions: the card is prepared as in section 4, the manager is installed as in section 5, and the headphones are not on tightly while the sound is checked — first make sure the volume is moderate.

If there is no sound, first rule out the headphones: listen to them on another source (a phone), wiggle the plug. Then check the position of the PWM_EN jumper (section 3.1): it must match the firmware's sound output. If the manager does not load, follow the order in section 5 ("If something does not load").

### 6.1. Workstation with pico-launcher

The checks are the same as for the Olimex ([`../olimex-pico-pc/README.en.md`](../olimex-pico-pc/README.en.md), section 5.1, items 1–8): boot, video, navigation, help, viewer, editor with writing to SD, delete with cancel, starting a firmware from `/emu` and returning with Ctrl+Alt+Del and F11. Only item 9 differs:

| # | Check | Actions | Expected result |
|---:|---|---|---|
| 9 | Sound | Start a firmware accepted on this board (status ✅ in `COMPATIBILITY.md`), for example murm386 (`/emu/m2p2-386-RUNTIME-…`) or PICO-BK in the build matching the PWM_EN jumper position (`/emu/m2p2-bk-…-I2S-…` or `…-PWM-…`) | Sound in the headphones plugged into the Audio OUT jack. The launcher itself outputs no sound |

### 6.2. Workstation with MOS2

The checks are the same as for the Olimex ([`../olimex-pico-pc/README.en.md`](../olimex-pico-pc/README.en.md), section 5.2): 252 MHz in `cpu`, the video modes `mode 0/1/2` and `gmode 2`, the keyboard (`basic /test/kbdtest.bas`), the F keys in `mc`, reading and writing the SD card, sound with `wav /test/snd_mono.wav` and `wav /test/snd_lr.wav`, switching firmwares and rebooting. MOS2 outputs sound to the Audio OUT jack; if the channels are swapped, or there is no sound with the PWM_EN jumper set correctly, note it in `COMPATIBILITY.md`.

### 6.3. Selecting Pico 2 modules for overclocking

The Pico 2 module plugs into sockets, so, as with the Olimex, it is the **module** that is selected, not the whole board; modules selected on an Olimex are good here too. The selection threshold is the most demanding firmware of the kit, currently murm386: 504 MHz / 1.6 V.

| # | Firmware | Mode | Actions | Criterion |
|---:|---|---|---|---|
| 1 | PICO-BK | 720×576, 270 MHz / 1.3 V | Start `/emu/m2p2-bk-…uf2` (the build matching the PWM_EN position) | Stable picture, sound in the headphones |
| 2 | PICO-BK | 800×600, 400 MHz / 1.5 V | Home → HDMI item → space; start a game from the built-in floppy (Tab, arrows, Enter) and leave it running | No hangs, resets, picture or sound artefacts |
| 3 | murm386 | the selection threshold, currently 504 MHz / 1.6 V (Win+F11 menu) | Start `/emu/m2p2-386-RUNTIME-…uf2`*, wait for Volkov Commander; start `\FREEDOS\SI.EXE`; leave it running | FreeDOS boots, SI shows the performance index, the system does not hang |

\* if the Pico 2 module has PSRAM, it makes sense to choose the firmwares from the `/emu/psram` folder: they are adapted to PSRAM and faster than the universal build. Of the 286/386 emulators, the 286 variant gives the best performance — its emulator is "lighter".

Each stage lasts at least 10 minutes without failures. After the check, set PICO-BK back to the 720×576 video mode (Home → HDMI). Module classes A, B and C are as for the Olimex (section 5.3 of the Olimex instructions): class A modules go to the pupils' stations.

Overclocking and voltages above nominal are outside the modes guaranteed by the manufacturer and may reduce the chip's stability over time. Items 2–3 are repeated once a school term and whenever unexplained hangs appear on a workstation.

The acceptance result is recorded in [`../COMPATIBILITY.md`](../COMPATIBILITY.md): date, board and module unit, items passed, remarks, the module's class.

## 7. Rolling out to the whole class

After the reference station has passed acceptance, record the board build variant (HDMI or VGA, jumpers), the versions of the manager and the firmwares, the content of the SD card, the settings and the test results. Make copies of the reference SD card for the other stations; check booting and sound on each unit. Do not take a check of one sample as proof that the whole batch works: the boards are assembled by hand, and a soldering fault on one unit says nothing about another.

The spare part is the **Pico 2 module** with the manager already installed: if the pico-launcher or MOS2 firmware is damaged, it is faster to swap the module during the lesson than to reflash it, and deal with the repair at a calmer time. It is good to have one spare board as well.

Be prepared for children to mess up the data on the SD card: it is best to have spare cards prepared in advance for quick replacement without holding up the lesson.

## Similar boards

The **FRANK** boards (by Mikhail Matveev, rh1tech) started as an offshoot of the Murmulator project and are pin-compatible with the Murmulator 2: the full-size and compact FRANK boards use the M2 pin layout, so firmwares for the M2 run on them — allowing for differences in hardware (FRANK, for example, also has a USB keyboard input through a separate RP2040-Zero, a composite output and Wi-Fi). These boards have not been tested in the class.

- [rh1tech/frank](https://github.com/rh1tech/frank) — current boards (FRANK, MiniFRANK, MicroFRANK and others), Gerber files, bills of materials, documentation; project site — [rh1.tech/projects/frank](https://rh1.tech/ru-ru/projects/frank?area=about), firmwares — [rh1.tech/projects/frank?area=firmware](https://rh1.tech/ru-ru/projects/frank?area=firmware)
- [rh1tech/frank-archive](https://github.com/rh1tech/frank-archive) — archive of earlier revisions, including FRANK M1 and M2. Links to FRANK M1 / M2 HW 2.05 from the announcement point to `rh1tech/frank/hardware/frank_m1` and `…/frank_m2`, which are no longer in the main repository: the FRANK M1 HW 2.05 documentation is in `frank-archive/frank_m1/docs/2_05/`, the Gerber files in `frank-archive/frank_m1/gerbers/frank_m1_2_05.zip`; the full-size FRANK board HW 2.05 and newer is in `frank/hardware/frank/docs/2/05/`
- [Murmulator: Frank boards](https://murmulator.ru/mm-frank), [Frank M2](https://murmulator.ru/frank-m2) (in Russian); AliExpress links in the FRANK bills of materials may be out of date

## Files in this folder

| Path | What is here |
|---|---|
| [`MANIFEST.md`](MANIFEST.md) | what is in the board folder, what sources it was built from, checksums (in Russian) |
| [`SHA256SUMS`](SHA256SUMS) | checksums of the board's files (how to check — [`MANIFEST.md`](MANIFEST.md), section 4) |
| [`assets/`](assets/) | schematic (`MURMULATOR2_HW20.pdf`) and Gerber files of the main board (`gerber_38NJU24_V1.0.zip`) and the bottom board (`gerber_38EJU24.zip`) |
| [`img/`](img/) | connection diagram: pins, connectors and jumpers (section 3.1) |
| [`pico-launcher/`](pico-launcher/) | pico-launcher builds for the `m2p2`: HDMI or VGA, USB keyboard (`HID`) or PS/2 |
| [`murmulator-os2/`](murmulator-os2/) | MOS2 builds for the `m2p2`: USB keyboard (`HID`) or PS/2 |
| [`sdcard/emu/`](sdcard/emu/) | emulators built for the `m2p2`: murm386 (three builds, two of them in `psram/`), PICO-BK (two builds: I2S and PWM), NES (two builds: I2S and PWM), murmapple (eight builds: HDMI or VGA, I2S or PWM; four with PSRAM in `psram/`), pico-z26 and atari800 (four builds each: HDMI or VGA, I2S or PWM), pico-speccy (two builds: video through PIO and HDMI through HSTX), Game & Watch (one build), murmc64 (four builds: HDMI or VGA, I2S or PWM), frank-micro and frank-msx (one build each), emu80v4 Korvet and Vector-06C (two builds each: HDMI and VGA), pico-pce (four builds: HDMI or VGA, I2S or PWM), pico-genesisPlus (two builds: HDMI with I2S or PWM; VGA and PS/2 are not supported), pico-gameboy, pico-lynx, pico-mac, pico-gamate, pico-watara, pico-wonderswan and pico-neogeo-pocket (four builds each: HDMI or VGA, I2S or PWM) |

## Sources

All sources below are in Russian.

- [Murmulator: project development (M2)](https://murmulator.ru/nextgen) — the Murmulator 2 home page, list of firmwares for the M2
- [Murmulator 2.0 by AlexEKB](https://murmulator.ru/mm2-0) — schematic, Gerber files and interactive bills of materials of the main and bottom boards, the sound selection jumper, recommended component values, do not solder PSRAM and XN297L; [M2 board types](https://murmulator.ru/types2)
- Support group — [Telegram: ZX_MURMULATOR](https://t.me/ZX_MURMULATOR/186817)
- The materials in `assets/` are from murmulator.ru (schematic in KiCad, 2024-11-23); the connection diagram in `img/` is by JAVAVIUA, 01.12.2024
