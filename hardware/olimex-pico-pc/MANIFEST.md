# Манифест комплекта: Olimex RP2040-PICO-PC + Raspberry Pi Pico 2 (PCp2)

Состояние на 2026-10-02. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность. При замене любого файла комплекта обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `murmulator-os2/PCp2-murmulator-os-VGA-HDMI-HID-2.3.2-252MHz-8.uf2` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | тег `v.2.3.2` = `aad5813` (2026-07-15), build 8 | `PICO_PC=ON`, `VGAHDMI=ON`, `HID=ON`, `CPU_MHZ=252` | `9cdb1263981fdb7df9e3e2a887afb73e24679a9f3e2ff30751399f50bf442c73` |
| `pico-launcher/PCp2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 | `PICO_PC=ON`, `HID=ON`, `MinSizeRel` (HDMI включается автоматически) | `2cde325b9a32b3a6a569bdb28421338559cfb660445232f7f1031926741e0334` |

Параметры сборки MOS2 восстановлены по имени файла согласно правилам формирования имени в `CMakeLists.txt` upstream. Оба образа собраны для RP2350 в режиме ARM Secure (UF2 family `0xe48bff59`).

### Эмуляторы (`sdcard/emu/`)

Запускаются из pico-launcher или MOS2. Сами выставляют частоту и напряжение ядра при старте. У PICO-BK частота задаётся выбором видеорежима HDMI в меню (Home), у murm386 частота и напряжение меняются независимо в меню Win+F11.

| Файл | Исходники | Версия / коммит | Частота / напряжение | SHA-256 |
|---|---|---|---|---|
| `sdcard/emu/PCp2-bk-VGA-HDMI-400MHz-PWM-1.4.9.uf2` | [DnCraptor/PICO-BK](https://github.com/DnCraptor/PICO-BK) | 1.4.9, `master` = `8c0e735` (2026-10-02) | по умолчанию 270 МГц / 1.3 В (HDMI 720×576); в меню — 400 МГц / 1.5 В (800×600) и 512 МГц / 1.6 В (1024×768) | `65b22b8344d73dcf088451392420a58d6e7c163a7355b5cac50092fe4f4d169d` |
| `sdcard/emu/PCp2-nes-VGA-HDMI-PWM-305.uf2` | [DnCraptor/pico-nes](https://github.com/DnCraptor/pico-nes) | 305, `main` = `fa24bc7` (2026-10-01) | 378 МГц / 1.6 В | `3557d29136b2d88de534c984149bd7978379f588d75189faced817f1dd398d74` |
| `sdcard/emu/PCp2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `a73cfd3` (2026-10-02) | 504 МГц / 1.6 В | `fe5e41767e916a4596c3b0a4590404aeb73f23190be29e261c6704e45690903c` |
| `sdcard/emu/PCp2-speccy-VGA-HDMI-HSTX-1.0.8.uf2` | [drewpo28/pico-speccy](https://github.com/drewpo28/pico-speccy) | релиз [v1.0.8](https://github.com/drewpo28/pico-speccy/releases/tag/v1.0.8), тег `v1.0.8` = `119f372` (2026-09-29) | 378 МГц / 1.5 В (по умолчанию; 252–504 МГц и 1.15–1.80 В — в меню) | `9f577078666fc52e566ffde7736a0eb3132ce7676303c0f6cbce0a8e88f06155` |
| `sdcard/emu/PCp2-frank_apple-HDMI-378MHz-1.05.uf2` | [DnCraptor/murmapple](https://github.com/DnCraptor/murmapple) | 1.05, `b95b7c7` (2026-10-02), сборка без PSRAM | 378 МГц / 1.6 В | `088957d8cbb4b1b82de98bbef82766eb0a4e593dd7c198842a0521ef2ac32a0f` |
| `sdcard/emu/psram/PCp2-frank_apple-HDMI-378MHz-P84-1.05.uf2` | там же | 1.05, `b95b7c7` (2026-10-02), сборка с PSRAM 84 МГц | 378 МГц / 1.6 В | `843f27b6a76e26b46544499c3a6e3ade9792a4c0ed403ee6099daa7cccf33c7a` |

Частота и напряжение взяты из имени файла и исходников сборки (`boards/olimex-pico-pc.h` у pico-nes, таблица видеорежимов у PICO-BK). Образ pico-nes не содержит дополнительного блока UF2 для errata RP2350-E10; на запуск из launcher и MOS2 это не влияет.

В подкаталоге `emu/psram` лежат сборки, которым нужна внешняя PSRAM; на рабочих местах без PSRAM они не работают или работают не полностью. Сейчас там одна сборка: murmapple с PSRAM держит в ней образы дисков и без PSRAM диски не загружает.

## 2. Платформенная часть SD-карты (`sdcard/`)

Общая часть карты — MOS2, FreeDOS, игры, учебные программы и тестовые файлы — лежит в каталоге `sdcard/` в корне репозитория и описана в [`../../SDCARD.md`](../../SDCARD.md). Карту готовят в два шага: сначала копируют общий `sdcard/`, затем поверх него — содержимое этого каталога.

| Путь на карте | Происхождение | Отличия от источника |
|---|---|---|
| `/emu/*.uf2`, `/emu/psram/*.uf2` | см. раздел 1, «Эмуляторы» | побайтово совпадают с файлами сборки |

## 3. Проверка целостности

Из каталога `hardware/olimex-pico-pc`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Здесь проверяются только файлы платы; общая часть карты проверяется по `SHA256SUMS` в корне репозитория (см. `SDCARD.md`, раздел 7).

## 4. Лицензии

MOS2, pico-launcher, PICO-BK и pico-nes распространяются под GPLv3, murm386 — под MIT. Исходники соответствуют коммитам, указанным в разделе 1. Лицензии содержимого общей части карты — в [`../../SDCARD.md`](../../SDCARD.md).
