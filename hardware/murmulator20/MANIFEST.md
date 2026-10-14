# Манифест комплекта: Murmulator 2.0 + Raspberry Pi Pico 2 (m2p2)

Состояние на 2026-10-14: есть pico-launcher, MOS2, murm386, PICO-BK, NES, murmapple (Apple IIe), pico-z26 (Atari 2600), atari800, pico-speccy, Game & Watch, murmc64 (Commodore 64), frank-micro (BBC Micro), frank-msx (MSX) и emu80v4 («Корвет», «Вектор-06Ц»), а также схема и гербер-файлы платы. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность — по образцу [`../olimex-pico-pc/MANIFEST.md`](../olimex-pico-pc/MANIFEST.md). При добавлении или замене любого файла обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

Прошивки каталога собраны 2026-10-14 с профилем платы Murmulator 2; pico-launcher, MOS2, murm386 и PICO-BK — с тех же коммитов, что и для PCp2; NES — из `main` pico-nes (`3bfd622`), murmapple и pico-z26 — из `main` (`d3b263d` и `68b7a25`), тех же коммитов, что для z0p2; atari800 — из `master` = `d72f2c4`, сборки z0p2 плюс профиль m2p2 и исправление вывода I2S; pico-speccy — готовые сборки из релиза v1.0.9; Game & Watch и murmc64 — из `4bd3f4d` и `8440245`, тех же коммитов, что для z0p2; frank-micro — из `main` = `4b50892`, сборки z0p2 плюс настройка CMake для m2p2; frank-msx — из `main` = `9b8e9e4`, того же коммита, что для z0p2; emu80v4 — из веток `korvet` = `730ea0f` и `vector06c` = `f55ad6b`, тех же коммитов, что для PCp2 и z0p2 (раздел 1, «Эмуляторы»); образы — для RP2350 в режиме ARM Secure (UF2 family `0xe48bff59`). Параметры сборки восстановлены по именам файлов.

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `pico-launcher/m2p2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 — тот же коммит, что для PCp2 | профиль платы m2p2, HDMI, USB-клавиатура (`HID`) — **основной вариант** | `2763def2932f58edfad395558e7149bf7bc9e37bfe5e4974a9c6e93d7c065d37` |
| `pico-launcher/m2p2-uf2-launcher-VGA-HID.uf2` | там же | то же | m2p2, VGA, USB-клавиатура | `16020ead36c2e568ea496e1e55868ca4217ba4427bc101cb6b2fc7f378322af1` |
| `pico-launcher/m2p2-uf2-launcher-HDMI.uf2` | там же | то же | m2p2, HDMI, клавиатура PS/2 | `44ef7f43a1114fa387c175a60fb918bd67d94e1e677c71fa2bdff6d6c9e2f02f` |
| `pico-launcher/m2p2-uf2-launcher-VGA.uf2` | там же | то же | m2p2, VGA, клавиатура PS/2 | `d531672152a4f477e1bb5fa79b5b2a5cf9ab4b73a14b0ff8163f62d57b207c0c` |
| `murmulator-os2/m2p2-murmulator-os-VGA-HDMI-HID-2.3.3-252MHz-1.uf2` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | 2.3.3, `main` = `a77484c` (2026-10-12), build 1 — тот же коммит, что для PCp2 | `VGAHDMI=ON` (видеовыход определяется сам), `HID=ON`, `CPU_MHZ=252` | `28364a4bbf98a3d66cda083813bbbcc088cce7c4baf0cbc2591daf3556d10337` |
| `murmulator-os2/m2p2-murmulator-os-VGA-HDMI-2.3.3-252MHz-1.uf2` | там же | то же | `VGAHDMI=ON`, клавиатура PS/2, `CPU_MHZ=252` | `bbbae9427b9207204b9fe6fb6be8af8f69dbe70911856a647beddf2479b40fbb` |

### Эмуляторы (`sdcard/emu/`)

Запускаются из pico-launcher или MOS2. Сами выставляют частоту и напряжение ядра при старте.

| Файл | Исходники | Версия / коммит | Частота / напряжение | SHA-256 |
|---|---|---|---|---|
| `sdcard/emu/m2p2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `fc80323` (2026-10-06) — тот же коммит, что для PCp2; профиль платы M2; сборка без PSRAM (`VIDEO_MODE=RUNTIME`, подкачка гостевой памяти) | 504 МГц / 1.6 В | `c2c33a4b706d4e44406330ad96c3228a287db692078015cfc168cdb15dbf878a` |
| `sdcard/emu/psram/m2p2-386-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, профиль M2; сборка с PSRAM: ядро 386, VGA256, гостевая память прямо в PSRAM 66 МГц (`NO_PAGING`) | 504 МГц / 1.6 В | `5c95b7b5e6580d0fcad94a6d67531cb8a9b1a0f28a49df061514423be71b742d` |
| `sdcard/emu/psram/m2p2-286-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, профиль M2; сборка с PSRAM: ядро 8086/186/286 (`CPU_TARGET=286`), остальное как у сборки 386 | 504 МГц / 1.6 В | `0538235fce3d0acbb6ed3efd5e0faabeaf15863323dd3995daa4e74aab6bacfc` |
| `sdcard/emu/m2p2-bk-VGA-HDMI-400MHz-I2S-1.5.3.uf2` | [DnCraptor/PICO-BK](https://github.com/DnCraptor/PICO-BK) | 1.5.3, `master` = `2704872` (2026-10-12) — тот же коммит, что для PCp2; звук через ЦАП I2S платы (TDA1387) и по HDMI | как у PCp2: по умолчанию 270 МГц / 1.3 В (HDMI 720×576); в меню Home — 400 МГц / 1.5 В (800×600) и 512 МГц / 1.6 В (1024×768) | `dea74f660555c4a72d11890b3f7054aa9d2089baf576f8983655e588aaee51e9` |
| `sdcard/emu/m2p2-bk-VGA-HDMI-400MHz-PWM-1.5.3-1.uf2` | там же | 1.5.3, `2704872`; то же, звук PWM | как у сборки I2S | `f47515098522243f7a7d383e0885b45d483406b11ae4dcb7c2bb48eac0b0d702` |
| `sdcard/emu/m2p2-nes-VGA-HDMI-I2S-TDA1387-305.uf2` | pico-nes (NES / Dendy) | 305, `main` = `3bfd622` (2026-10-08) | 378 МГц / 1.6 В | 99993d04eb8e1da5ccd15fea2047ac330fc17f637f21a06a9cfb649d5415307c |
| `sdcard/emu/m2p2-nes-VGA-HDMI-PWM-305.uf2` | pico-nes (NES / Dendy) | 305, `main` = `3bfd622` (2026-10-08) | 378 МГц / 1.6 В | 538729df6c21c5b7e30e38c70bdbc1586ac1151e86386f94b657eba07dd3536e |
| `sdcard/emu/m2p2-frank_apple-HDMI-378MHz-I2S-1.05.uf2` | [DnCraptor/murmapple](https://github.com/DnCraptor/murmapple) | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; HDMI, звук — ЦАП I2S платы (TDA1387); сборка без PSRAM: образы дисков читаются с SD-карты | 378 МГц / 1.6 В | `8ba78971c5007e027273e3ce6aa0c545bd6404621a4f9565e359619767bb9de5` |
| `sdcard/emu/m2p2-frank_apple-HDMI-378MHz-PWM-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; HDMI, звук — PWM; сборка без PSRAM: образы дисков читаются с SD-карты | 378 МГц / 1.6 В | `496fed103dd256e77deaa235f071c92560734f4d9ce1180bd7524ec9542dbd98` |
| `sdcard/emu/m2p2-frank_apple-VGA-378MHz-I2S-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; VGA, звук — ЦАП I2S платы (TDA1387); сборка без PSRAM: образы дисков читаются с SD-карты | 378 МГц / 1.6 В | `d28cf6a442712dcfaf99a816ee52ee20ed983f084eea12daa078e0ea95f4e7c6` |
| `sdcard/emu/m2p2-frank_apple-VGA-378MHz-PWM-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; VGA, звук — PWM; сборка без PSRAM: образы дисков читаются с SD-карты | 378 МГц / 1.6 В | `d8092758653478af0f6c9e64b6ffdc4d1aa5d86bee6ac761a1be0f856be29650` |
| `sdcard/emu/psram/m2p2-frank_apple-HDMI-378MHz-P84-I2S-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; HDMI, звук — ЦАП I2S платы (TDA1387); сборка с PSRAM 84 МГц на модуле Pico 2: образы дисков держатся в PSRAM | 378 МГц / 1.6 В | `fa829120de1bd8332960abede7f75564d71b56c44922f68cc986b2691663be9c` |
| `sdcard/emu/psram/m2p2-frank_apple-HDMI-378MHz-P84-PWM-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; HDMI, звук — PWM; сборка с PSRAM 84 МГц на модуле Pico 2: образы дисков держатся в PSRAM | 378 МГц / 1.6 В | `b26f56f12ef92b02eefc69c5fd782d0248f88284102f2b1141a328d4a53d49dd` |
| `sdcard/emu/psram/m2p2-frank_apple-VGA-378MHz-P84-I2S-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; VGA, звук — ЦАП I2S платы (TDA1387); сборка с PSRAM 84 МГц на модуле Pico 2: образы дисков держатся в PSRAM | 378 МГц / 1.6 В | `be861f26f4b1fb23f38669d79a77f4905ef7fd65f8592f0a66898f1af28cce87` |
| `sdcard/emu/psram/m2p2-frank_apple-VGA-378MHz-P84-PWM-1.05.uf2` | там же | 1.05, `main` = `d3b263d` (2026-10-08), `BOARD_VARIANT=M2`; VGA, звук — PWM; сборка с PSRAM 84 МГц на модуле Pico 2: образы дисков держатся в PSRAM | 378 МГц / 1.6 В | `65cc2c7d87cb3d2c5074ed5bdc01544be3b5880148b85457e40f87a9cf820171` |
| `sdcard/emu/m2p2-z26-HDMI-I2S-4.0.8.uf2` | [DnCraptor/pico-z26](https://github.com/DnCraptor/pico-z26) | 4.0.8, `main` = `68b7a25` (2026-10-08), `PICO_BOARD=murmulator2`; HDMI, звук — ЦАП I2S платы (TDA1387) | 378 МГц / 1.6 В | `a2e8f80a59c0863335ffadb2d6b09d6fd4224e494e2dbb1fab4111287a3e98a3` |
| `sdcard/emu/m2p2-z26-HDMI-PWM-4.0.8.uf2` | там же | 4.0.8, `main` = `68b7a25` (2026-10-08), `PICO_BOARD=murmulator2`; HDMI, звук — PWM | 378 МГц / 1.6 В | `0ed4c0b32d1670787c8efc0ac2d955bd923ac52e13d574ac8ef00281f1b636ef` |
| `sdcard/emu/m2p2-z26-VGA-I2S-4.0.8.uf2` | там же | 4.0.8, `main` = `68b7a25` (2026-10-08), `PICO_BOARD=murmulator2`; VGA, звук — ЦАП I2S платы (TDA1387) | 378 МГц / 1.6 В | `3e59ccedc405b5d861a7df071816ff7cf9cdc86cbf13dd41c6d623ef0d842f4b` |
| `sdcard/emu/m2p2-z26-VGA-PWM-4.0.8.uf2` | там же | 4.0.8, `main` = `68b7a25` (2026-10-08), `PICO_BOARD=murmulator2`; VGA, звук — PWM | 378 МГц / 1.6 В | `03f9f7d1df321ec644889671aa8315d1eee1a46c98097ad8dd04ec4db0e6803c` |
| `sdcard/emu/m2p2-atari800-HDMI-I2S-504-3.3.1.uf2` | [DnCraptor/atari800](https://github.com/DnCraptor/atari800) | 3.3.1, `master` = `d72f2c4` (2026-10-14), `M2`; HDMI, звук — ЦАП I2S платы (TDA1387), подача в FIFO PIO по прерыванию, USB HID | 504 МГц / 1.6 В | `bed167076403a7c0e35749cca483c75fb511ab1d5d8cc3c7cc6a751c5410fcbc` |
| `sdcard/emu/m2p2-atari800-HDMI-PWM-504-3.3.1.uf2` | там же | 3.3.1, `master` = `d72f2c4` (2026-10-14), `M2`; HDMI, звук — PWM, USB HID | 504 МГц / 1.6 В | `2309b8e802bb4355e9b10f3f48a5122c8cedffdb99af1feec011660fb4eb14f1` |
| `sdcard/emu/m2p2-atari800-VGA-I2S-504-3.3.1.uf2` | там же | 3.3.1, `master` = `d72f2c4` (2026-10-14), `M2`; VGA, звук — ЦАП I2S платы (TDA1387), подача в FIFO PIO по прерыванию, USB HID | 504 МГц / 1.6 В | `3c8c68d0081fa45222d03bed38d8e6fd81246932dc926c555b9d9695ca7b2ff4` |
| `sdcard/emu/m2p2-atari800-VGA-PWM-504-3.3.1.uf2` | там же | 3.3.1, `master` = `d72f2c4` (2026-10-14), `M2`; VGA, звук — PWM, USB HID | 504 МГц / 1.6 В | `c1ffff6a4eb89e9b40e7d58e1b0ea06a7faf59372af65beaba5adc4b51e31e48` |
| `sdcard/emu/m2p2-speccy-VGA-HDMI-1.0.9.uf2` | [drewpo28/pico-speccy](https://github.com/drewpo28/pico-speccy) | релиз [v1.0.9](https://github.com/drewpo28/pico-speccy/releases/tag/v1.0.9), готовая сборка из релиза; тег `v1.0.9` сейчас указывает на `12737dd` (2026-10-09), в прошивке — строка сборки `12737dd5` и дата Oct 9 2026 (сборка z0p2 в комплекте — более ранняя, от 7 октября, `f5f57d4`); `MURM2`, видео HDMI или VGA через PIO; звук — I2S (TDA1387) или PWM, выбор в меню | по умолчанию сборки, меняется в меню | `0531deacc0daebc398c6330e7a95c83ca58191674e0845db176d40714ae719ab` |
| `sdcard/emu/m2p2-speccy-VGA-HDMI-HSTX-1.0.9.uf2` | там же | v1.0.9, `12737dd`; то же, HDMI через HSTX (GP12–GP19) | по умолчанию сборки, меняется в меню | `6ee8575c22cd2e258a0dbc73b40ac7d26b3974d89c986d5c570352faff7cf111` |
| `sdcard/emu/m2p2-gnw-VGA-HDMI-TFT-0.1.18.uf2` | [DnCraptor/murmulator_game_n_watch](https://github.com/DnCraptor/murmulator_game_n_watch) | 0.1.18, `master` = `4bd3f4d` (2026-10-13), `PICO_BOARD=murmulator2`; одна сборка для HDMI, VGA и TFT; звук — I2S (TDA1387, GP9–GP11) или PWM (GP10/GP11, пищалка GP9), выбор в настройках | 252 МГц / 1.3 В | `821412fa849fc7a6b3f6788d393120616852fbb47f1a2e319712db42f9a6ddaa` |
| `sdcard/emu/m2p2-frank-c64-HDMI-504MHz-F66-I2S-v1.08.uf2` | [DnCraptor/murmc64](https://github.com/DnCraptor/murmc64) | v1.08, `8440245` (2026-10-08), тот же коммит, что для z0p2; профиль m2p2, HDMI, звук — ЦАП I2S платы (TDA1387), flash 66 МГц (`F66`); сборка без PSRAM | 504 МГц / 1.65 В | `492a1b75564eb36a104f7cc9f81fae04048cc21366bbc1e8a2c9af5bf002597c` |
| `sdcard/emu/m2p2-frank-c64-HDMI-504MHz-F66-PWM-v1.08.uf2` | там же | v1.08, `8440245` (2026-10-08), тот же коммит, что для z0p2; профиль m2p2, HDMI, звук — PWM, flash 66 МГц (`F66`); сборка без PSRAM | 504 МГц / 1.65 В | `2ef2b9ef28a599fb9f11f5cb36d367b9ab22be4546cea6f84b43df0f8c922024` |
| `sdcard/emu/m2p2-frank-c64-VGA-504MHz-F66-I2S-v1.08.uf2` | там же | v1.08, `8440245` (2026-10-08), тот же коммит, что для z0p2; профиль m2p2, VGA, звук — ЦАП I2S платы (TDA1387), flash 66 МГц (`F66`); сборка без PSRAM | 504 МГц / 1.65 В | `2578f4fee135b53722346c74b9bb39ada52b5ac3f47f9f4157e6b72ebcf016cd` |
| `sdcard/emu/m2p2-frank-c64-VGA-504MHz-F66-PWM-v1.08.uf2` | там же | v1.08, `8440245` (2026-10-08), тот же коммит, что для z0p2; профиль m2p2, VGA, звук — PWM, flash 66 МГц (`F66`); сборка без PSRAM | 504 МГц / 1.65 В | `7a93ced632a4cea6c09ca554886c3660fe5c9e78785929b2850180ff0cec8c43` |
| `sdcard/emu/m2p2-frank-micro-HDMI-VGA-252MHz-1.00.uf2` | [DnCraptor/frank-micro](https://github.com/DnCraptor/frank-micro) | 1.00, `main` = `4b50892` (2026-10-14): `332ad17` (тот же, что для z0p2) плюс настройка CMake для m2p2 — `PLATFORM=m2`, видеодрайвер `HDMI_PIO` (HDMI или VGA, определяется по GP12/13; звука через HDMI нет), USB HID включён; звук — I2S (TDA1387, GP9–GP11) или PWM (GP10/GP11), выбор в F12, по умолчанию I2S; PSRAM необязательна, определяется при старте | 252 МГц / 1.5 В | `1eb685b00850359f1db5b8718cf7025923ddb0d2bd84b85debeea78c849daa19` |
| `sdcard/emu/m2p2-frank-msx-HDMI-VGA-252MHz-0.01.uf2` | [DnCraptor/frank-msx](https://github.com/DnCraptor/frank-msx) | 0.01, `main` = `9b8e9e4` (2026-10-13), тот же коммит, что для z0p2; `PLATFORM=m2`, видео HDMI или VGA (определяется по GP12/13), USB HID; звук — I2S (TDA1387, GP9–GP11) или PWM, выбор в F12, по умолчанию I2S; PSRAM необязательна, определяется при старте (без неё — MSX1, с ней — MSX2/MSX2+) | 252 МГц / 1.5 В | `a2ee6f9c5920cba3f30c3243eda6dab81f1201fa55e3aca8bdb752e11a990270` |
| `sdcard/emu/m2p2-korvet-400-HDMI-DVI-0.7.1.uf2` | [DnCraptor/emu80v4](https://github.com/DnCraptor/emu80v4), ветка `korvet` | 0.7.1, `korvet` = `730ea0f`, тот же коммит, что для PCp2 и z0p2; профиль платы `murmulator2`, HDMI 800×600 через libdvi (`murmulator2_cfg`); звук — I2S (TDA1387) или PWM, определяется при старте, меняется в меню; PSRAM необязательна | 400 МГц / 1.5 В | `9ab8609e0dc32bfb9a3488f1169488c029c7a61dd40b1eaebe058289d4cc9b87` |
| `sdcard/emu/m2p2-korvet-400-VGA-0.7.1.uf2` | там же, ветка `korvet` | 0.7.1, `korvet` = `730ea0f`, тот же коммит, что для PCp2 и z0p2; профиль платы `murmulator2`, VGA (vga-nextgen); звук — I2S (TDA1387) или PWM, определяется при старте, меняется в меню; PSRAM необязательна | 400 МГц / 1.5 В | `0c201c7ae3d2e1870f75edc1df88706fa1d8cac42d602c2e0fd25dfe7f3659d6` |
| `sdcard/emu/m2p2-v06c-400-HDMI-DVI-0.7.6.uf2` | там же, ветка `vector06c` | 0.7.6, `vector06c` = `f55ad6b`, тот же коммит, что для PCp2 и z0p2; профиль платы `murmulator2`, HDMI 800×600 через libdvi (`murmulator2_cfg`); звук — I2S (TDA1387) или PWM, определяется при старте, меняется в меню; PSRAM необязательна | 400 МГц / 1.5 В | `7f39511e502ad767fee6edb479beaba8fef73dda0ee6a9c5772d6e51f493a217` |
| `sdcard/emu/m2p2-v06c-400-VGA-0.7.6.uf2` | там же, ветка `vector06c` | 0.7.6, `vector06c` = `f55ad6b`, тот же коммит, что для PCp2 и z0p2; профиль платы `murmulator2`, VGA (vga-nextgen); звук — I2S (TDA1387) или PWM, определяется при старте, меняется в меню; PSRAM необязательна | 400 МГц / 1.5 В | `2597bd44a47c20818a247deec692477f1721aa7f04d1ac76d6c5415ab315620a` |

Сборка PICO-BK выбирается по положению перемычки PWM_Enable на плате (README, раздел 3.1): I2S — для ЦАП TDA1387, PWM — для звука PWM. murm386 выбирает выход в меню **Win+F11** (Audio output) и хранит настройки для m2p2 в `/.config/386/M2/`, отдельно от других плат. В подкаталоге `emu/psram` — сборки, которым нужна PSRAM **на модуле Pico 2** (например, Pimoroni Pico Plus 2); микросхема PSRAM на самой плате Murmulator 2.0 для них не используется.

## 2. Материалы платы (`assets/`, `img/`)

| Файл | Что это | Источник | SHA-256 |
|---|---|---|---|
| `assets/MURMULATOR2_HW20.pdf` | принципиальная схема основной платы 38NJU24, HW 2.0 (KiCad, 2024-11-23) | [murmulator.ru/mm2-0](https://murmulator.ru/mm2-0) («Схема_МУРМУЛЯТОР2_HW20.pdf») | `e4871d577c53881f86673aafb95ebb4583c2e173468a760d3f3bc976671924ae` |
| `assets/gerber_38NJU24_V1.0.zip` | гербер-файлы основной платы 38NJU24, V1.0 | там же | `c9b9a527d6e9c04a31dc2345e24ebb768af2ca3125efe86af74e3583037569e5` |
| `assets/gerber_38EJU24.zip` | гербер-файлы поддона 38EJU24 (HDMI, композитный выход, USB-хаб, второй геймпад) | там же | `a25ac5ed62e8b6dfac053801bbdd37232e4af43696606dad4486fd0bcf27a9b9` |
| `img/m2p2-pinout.jpg` | схема подключения: выводы Pico 2, разъёмы и перемычки (JAVAVIUA, 01.12.2024) | автор схемы указан на самой картинке; добавлена в репозиторий 2026-10-14 (`b412067`) | в `SHA256SUMS` не входит, как и картинки других плат |

Интерактивные перечни элементов (iBOM) на сайте: [основная плата](https://file.murmulator.ru/data/murmulator2-types/mm2-0/plata2/ibom-mm2.0.html), [поддон](https://file.murmulator.ru/data/murmulator2-types/mm2-0/poddon2/ibom-mp2.0.html).

## 3. Платформенная часть SD-карты (`sdcard/`)

Карту готовят как для Olimex: сначала общий `sdcard/` из корня репозитория, затем поверх — `sdcard/` этой платы.

| Путь на карте | Происхождение | Отличия от источника |
|---|---|---|
| `/emu/*.uf2`, `/emu/psram/*.uf2` | см. раздел 1, «Эмуляторы» | побайтово совпадают с файлами сборки |

## 4. Проверка целостности

Из каталога `hardware/murmulator20`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Здесь проверяются только файлы платы; общая часть карты — по `SHA256SUMS` в корне репозитория (см. `SDCARD.md`, раздел 23).

## 5. Лицензии

pico-launcher, MOS2 и PICO-BK распространяются под GPLv3, murm386 — под MIT. Схема и гербер-файлы Murmulator 2.0 опубликованы автором платы на murmulator.ru для изготовления; лицензия на странице не указана.
