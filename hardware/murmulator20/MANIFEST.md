# Манифест комплекта: Murmulator 2.0 + Raspberry Pi Pico 2 (m2p2)

Состояние на 2026-10-14: есть pico-launcher, MOS2, murm386, PICO-BK и NES, а также схема и гербер-файлы платы. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность — по образцу [`../olimex-pico-pc/MANIFEST.md`](../olimex-pico-pc/MANIFEST.md). При добавлении или замене любого файла обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

Прошивки каталога собраны 2026-10-14 с профилем платы Murmulator 2; pico-launcher, MOS2, murm386 и PICO-BK — с тех же коммитов, что и для PCp2; NES — из `main` pico-nes (`3bfd622`, раздел 1, «Эмуляторы»); образы — для RP2350 в режиме ARM Secure (UF2 family `0xe48bff59`). Параметры сборки восстановлены по именам файлов.

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
