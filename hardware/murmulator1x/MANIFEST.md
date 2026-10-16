# Манифест комплекта: Murmulator 1.x с RP2350 (m1p2)

Состояние на 2026-10-16: есть pico-launcher, MOS2, murm386 и PICO-BK. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность — по образцу [`../olimex-pico-pc/MANIFEST.md`](../olimex-pico-pc/MANIFEST.md). При добавлении или замене любого файла обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

Прошивки каталога собраны 2026-10-16 с профилем платы Murmulator 1.x, с тех же коммитов, что и для m2p2 (и PCp2): pico-launcher — `5d871d7`, MOS2 — `a77484c`, murm386 — `fc80323`, PICO-BK — `2704872`. Образы — для RP2350 в режиме ARM Secure (UF2 family `0xe48bff59`). Параметры сборки восстановлены по именам файлов.

Выводы Murmulator 1.x, общие для этих прошивок (по профилю `BOARD_M1` в `src/board_config.h` murm386 и профилю M1 в `CMakeLists.txt` MOS2): HDMI и VGA — GP6–GP13, SD-карта — SCK 2, MOSI 3, MISO 4, CS 5, клавиатура PS/2 — GP0 (CLK) / GP1 (DATA), геймпад NES — CLK 14, LAT 15, DATA 16, звук PWM — GP26 / GP27 (бипер GP28), I2S у murm386 — DATA 26, BCK 27, LRCK 28.

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `pico-launcher/m1p2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 — тот же коммит, что для PCp2 и m2p2 | профиль платы m1p2, HDMI, USB-клавиатура (`HID`) — **основной вариант** | `b65cb77125782d47b9979b5a4f8eaf7150b0c971d88256e0392d5e9370d0ea76` |
| `pico-launcher/m1p2-uf2-launcher-VGA-HID.uf2` | там же | то же | m1p2, VGA, USB-клавиатура | `8b1ed987eab0d72348bfea4f26c653f16a092b4c5fc9b2fbdf66d93b48b4e38b` |
| `pico-launcher/m1p2-uf2-launcher-HDMI.uf2` | там же | то же | m1p2, HDMI, клавиатура PS/2 | `3d926ce1bb3d6fd308caa7cfd761fe37688b53ac3b35aba33d42468ce93b5db7` |
| `pico-launcher/m1p2-uf2-launcher-VGA.uf2` | там же | то же | m1p2, VGA, клавиатура PS/2 | `8e91316e638d79e1601edaf4862a4b6a47e48efaae8e0c12143401d7fa5f931d` |
| `murmulator-os2/m1p2-murmulator-os-VGA-HDMI-HID-2.3.3-252MHz-1.uf2` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | 2.3.3, `main` = `a77484c` (2026-10-12), build 1 — тот же коммит, что для PCp2 и m2p2 | `VGAHDMI=ON` (видеовыход определяется сам), `HID=ON`, `CPU_MHZ=252`; звук — PWM на GP26/GP27 | `f3fe3ec9cf5b587264e093c7f169bb4b8d6f65eb32ac2db10ced5394a21d8a6a` |
| `murmulator-os2/m1p2-murmulator-os-VGA-HDMI-2.3.3-252MHz-1.uf2` | там же | то же | `VGAHDMI=ON`, клавиатура PS/2, `CPU_MHZ=252` | `319d212b4fcd784a18b12bdbd1cbe8664133b7ff9265b3071470614c78c4242f` |

### Эмуляторы (`sdcard/emu/`)

Запускаются из pico-launcher или MOS2. Сами выставляют частоту и напряжение ядра при старте.

| Файл | Исходники | Версия / коммит | Частота / напряжение | SHA-256 |
|---|---|---|---|---|
| `sdcard/emu/m1p2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `fc80323` (2026-10-06) — тот же коммит, что для PCp2 и m2p2; профиль платы M1; сборка без PSRAM (`VIDEO_MODE=RUNTIME`, подкачка гостевой памяти); видеовыход и звук (I2S или PWM) выбираются в меню Win+F11, настройки — в `/.config/386/M1/` | 504 МГц / 1.6 В | `6cf6a60f9c3b0550bbf616d0c82851f6cb3962eb2c7439fe5fc8e56ac0a98ade` |
| `sdcard/emu/psram/m1p2-386-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, профиль M1; сборка с PSRAM: ядро 386, VGA256, гостевая память прямо в PSRAM 66 МГц (`NO_PAGING`) | 504 МГц / 1.6 В | `2449ca9b811f619e41c22647b626cbffcdc3eed9bd5709c45129dd8d4fdef965` |
| `sdcard/emu/psram/m1p2-286-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, профиль M1; сборка с PSRAM: ядро 8086/186/286 (`CPU_TARGET=286`), остальное как у сборки 386 | 504 МГц / 1.6 В | `5b847875905e92bccc1329d931923b072846698c0367dbb2a393a9510bdadee0` |
| `sdcard/emu/m1p2-bk-VGA-HDMI-400MHz-I2S-1.5.3.uf2` | [DnCraptor/PICO-BK](https://github.com/DnCraptor/PICO-BK) | 1.5.3, `master` = `2704872` (2026-10-12) — тот же коммит, что для PCp2 и m2p2; профиль платы M1 (`AY_TYPE=I2S`); видеовыход VGA или HDMI определяется при старте; звук — ЦАП TDA1387 по I2S (GP26–GP28), в сборке HDMI — также по HDMI | VGA — 400 МГц / 1.5 В; HDMI — как у PCp2: по умолчанию 270 МГц / 1.3 В (720×576), в меню Home — 400 МГц / 1.5 В и 512 МГц / 1.6 В | `c1129dec63a25d84bbf06278b64b81945f2db5fd4ab78b13f140b113db373bd4` |
| `sdcard/emu/m1p2-bk-VGA-HDMI-400MHz-PWM-1.5.3.uf2` | там же | 1.5.3, `2704872`, профиль M1 (`AY_TYPE=PWM`); то же, звук — PWM (GP26/GP27, бипер GP28) | VGA — 400 МГц / 1.5 В; HDMI — как у PCp2: по умолчанию 270 МГц / 1.3 В (720×576), в меню Home — 400 МГц / 1.5 В и 512 МГц / 1.6 В | `d02b3d0ef6830b8ad1af609c1b33c91021a8eac7261f944f256e65c96e583d23` |
| `sdcard/emu/m1p2-bk-VGA-HDMI-400MHz-HWAY-1.5.3.uf2` | там же | 1.5.3, `2704872`, профиль M1 (`AY_TYPE=HWAY`); то же, звук — аппаратные звуковые чипы (AY-3-8910, SAA1099) на плате расширения, управление через сдвиговый регистр 74HC595 на GP26–GP28 | VGA — 400 МГц / 1.5 В; HDMI — как у PCp2: по умолчанию 270 МГц / 1.3 В (720×576), в меню Home — 400 МГц / 1.5 В и 512 МГц / 1.6 В | `7d414ac719bccd60b65bbae30662b32916827a5b7c6ceaedfde23c26db24bfe3` |
| `sdcard/emu/m1p2-bk-TV-319.2MHz-I2S-1.5.3.uf2` | там же | 1.5.3, `2704872`, профиль M1, `TV_OUT=ON` (`AY_TYPE=I2S`): композитный видеовыход через ЦАП на GP6–GP13 (доработка платы); звук — ЦАП TDA1387 по I2S (GP26–GP28). **Сборки TV — только для демонстрационного места с ЭЛТ-телевизором, на рабочие места учащихся их не ставят.** | 319,2 МГц / 1.4 В (фиксировано для ТВ-сигнала PAL) | `4b9cba07baacb1ff8ad430c6c35cc0be6811aa61c83e79f81fa7bca9a38d7da4` |
| `sdcard/emu/m1p2-bk-TV-319.2MHz-PWM-1.5.3.uf2` | там же | 1.5.3, `2704872`, профиль M1, `TV_OUT=ON` (`AY_TYPE=PWM`): композитный видеовыход через ЦАП на GP6–GP13 (доработка платы); звук — PWM (GP26/GP27, бипер GP28). **Сборки TV — только для демонстрационного места с ЭЛТ-телевизором, на рабочие места учащихся их не ставят.** | 319,2 МГц / 1.4 В (фиксировано для ТВ-сигнала PAL) | `faff0ce1d4e6d7f0cf345c007736bc2ddc01bae6180f9604d4d2a43042230318` |
| `sdcard/emu/m1p2-bk-TV-319.2MHz-HWAY-1.5.3.uf2` | там же | 1.5.3, `2704872`, профиль M1, `TV_OUT=ON` (`AY_TYPE=HWAY`): композитный видеовыход через ЦАП на GP6–GP13 (доработка платы); звук — аппаратные звуковые чипы (AY-3-8910, SAA1099) на плате расширения, управление через сдвиговый регистр 74HC595 на GP26–GP28. **Сборки TV — только для демонстрационного места с ЭЛТ-телевизором, на рабочие места учащихся их не ставят.** | 319,2 МГц / 1.4 В (фиксировано для ТВ-сигнала PAL) | `4e6210bd3009318eb3bd20da2f6d39a7caecde5a9f07dd41fdd527a9cfe6b998` |

В подкаталоге `emu/psram` — сборки, которым нужна PSRAM на шине QSPI модуля RP2350 (выбор микросхемы — GP47 на RP2350B, GP19 на RP2350A, определяется при старте).

## 2. Платформенная часть SD-карты (`sdcard/`)

Карту готовят как для Olimex: сначала общий `sdcard/` из корня репозитория, затем поверх — `sdcard/` этой платы.

| Путь на карте | Происхождение | Отличия от источника |
|---|---|---|
| `/emu/*.uf2`, `/emu/psram/*.uf2` | см. раздел 1, «Эмуляторы» | побайтово совпадают с файлами сборки |
| `/mos2/README.md` | памятка по MOS2 для m1p2, написана для этого репозитория | кодировка CP866, концы строк LF — читается прямо в MOS2 (`type`, `mcview`) |

## 3. Проверка целостности

Из каталога `hardware/murmulator1x`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Здесь проверяются только файлы платы; общая часть карты — по `SHA256SUMS` в корне репозитория (см. `SDCARD.md`, раздел 23).

## 4. Лицензии

pico-launcher и MOS2 распространяются под GPLv3, murm386 — под MIT.
