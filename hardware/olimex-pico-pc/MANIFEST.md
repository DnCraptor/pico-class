# Манифест комплекта: Olimex RP2040-PICO-PC + Raspberry Pi Pico 2 (PCp2)

Состояние на 2026-10-05. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность. При замене любого файла комплекта обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

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
| `sdcard/emu/PCp2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `f81f5ed` (2026-10-03), сборка без PSRAM (`VIDEO_MODE=RUNTIME`, подкачка гостевой памяти) | 504 МГц / 1.6 В | `4a8c657c106c2989f14acc46e370316278277fc591433ce82a011e6b5a1cac01` |
| `sdcard/emu/psram/PCp2-386-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `210e92f` (2026-10-03), сборка с PSRAM: ядро 386, VGA256, гостевая память прямо в PSRAM 66 МГц (`NO_PAGING`) | 504 МГц / 1.6 В | `77fcb677222f4852e1093c87e892a593695e4dacd1a00956b53a4848d2b5f415` |
| `sdcard/emu/psram/PCp2-286-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `210e92f` (2026-10-03), сборка с PSRAM: ядро 8086/186/286 (`CPU_TARGET=286`), остальное как у сборки 386 | 504 МГц / 1.6 В | `38c1c02510df2656ddb5c89e390e1c9e074f1a481c05d4d2a1820a5354d73685` |
| `sdcard/emu/PCp2-speccy-VGA-HDMI-HSTX-1.0.8.uf2` | [drewpo28/pico-speccy](https://github.com/drewpo28/pico-speccy) | релиз [v1.0.8](https://github.com/drewpo28/pico-speccy/releases/tag/v1.0.8), тег `v1.0.8` = `119f372` (2026-09-29) | 378 МГц / 1.5 В (по умолчанию; 252–504 МГц и 1.15–1.80 В — в меню) | `9f577078666fc52e566ffde7736a0eb3132ce7676303c0f6cbce0a8e88f06155` |
| `sdcard/emu/PCp2-z26-HDMI-PWM-4.0.8.uf2` | [DnCraptor/pico-z26](https://github.com/DnCraptor/pico-z26) | 4.0.8, `9689fc5` (2026-10-03) | 378 МГц / 1.6 В (по умолчанию; до 524 МГц — в меню) | `e63befc68d57a448532a6504c29b9977b8bdd81efde853d7181cf341a659995e` |
| `sdcard/emu/PCp2-atari800-HDMI-PWM-378-3.3.1.uf2` | [DnCraptor/atari800](https://github.com/DnCraptor/atari800) | 3.3.1, ветка `hdmi` = `6866352` (2026-10-04) | 378 МГц / 1.6 В | `5910211fc75a3ff462d5b60f8eefe7ebc344bd9d8901f94f7f0f0988eb8ee409` |
| `sdcard/emu/PCp2-gnw-HDMI-0.1.18.uf2` | [DnCraptor/murmulator_game_n_watch](https://github.com/DnCraptor/murmulator_game_n_watch) | 0.1.18, `master` = `6d06e88` (2026-10-04) | 252 МГц / 1.3 В | `a7a34684f5b76b1ae0ee958bab777e87b385697e6df2c44c3e95f14beae5893b` |
| `sdcard/emu/PCp2-frank-c64-HDMI-378MHz-F66-PWM-v1.08.uf2` | [DnCraptor/murmc64](https://github.com/DnCraptor/murmc64) | v1.08, `main` = `2a398e1` (2026-10-04), сборка без PSRAM | 378 МГц / 1.6 В | `bb8790edbfcb69c1702bebeca5e16c4d425d0a56a3e2694a7328376169ad1b66` |
| `sdcard/emu/PCp2-frank-micro-HDMI-252MHz-1.00.uf2` | [DnCraptor/frank-micro](https://github.com/DnCraptor/frank-micro) | 1.00, `main` = `8a9cbba` (2026-10-04), `PLATFORM=pc`, видеодрайвер со звуком по HDMI | 252 МГц | `6fd9a993746b87dd34c90af7e7522ef0c6458bd8939782d4e8c3134648e8c17c` |
| `sdcard/emu/PCp2-frank-msx-HDMI-VGA-252MHz-0.01.uf2` | [DnCraptor/frank-msx](https://github.com/DnCraptor/frank-msx) | 0.01, `main` = `14d8d4d` (2026-10-04), `PLATFORM=pc`, USB HID включён; PSRAM необязательна: без неё — только MSX1, картриджи записываются во флеш-память | 252 МГц | `eae531dcfacfc4724b43b9b11455dbd7b102b3d8cb13f137c1b2f2cd7de9484c` |
| `sdcard/emu/PCp2-korvet-400-PWM-HDMI-DVI-0.7.1.uf2` | [DnCraptor/emu80v4](https://github.com/DnCraptor/emu80v4), ветка `korvet` (порт Emu80 v4) | 0.7.1, `korvet` = `730ea0f` (2026-10-04), `PICO_BOARD=olimex-pico-pc`, HDMI через libdvi (800×600, такт от PIO), звук PWM; PSRAM необязательна | 400 МГц / 1.5 В | `e956f8a0f4bca7229370a6e4814c681b9b1dc544ce53b8bdd30c9bc57b0732b5` |
| `sdcard/emu/PCp2-v06c-400-PWM-HDMI-DVI-0.7.6.uf2` | там же, ветка `vector06c` | 0.7.6, `vector06c` = `f55ad6b` (2026-10-05), `PICO_BOARD=olimex-pico-pc`, HDMI через libdvi (800×600, такт от PIO), звук PWM; PSRAM необязательна | 400 МГц / 1.5 В | `f79b88a05740682a8c83dc411d85a39e47a528b2947874a9225d8e60679c79dd` |
| `sdcard/emu/PCp2-pce-HDMI-PWM-1.1.1.uf2` | [DnCraptor/pico-pce](https://github.com/DnCraptor/pico-pce) (ядро pce-go) | 1.1.1, `main` = `a790a67` (2026-10-05), `PICO_BOARD=olimex-pico-pc`, `PICO_PLATFORM=rp2350`, HDMI через PIO, звук PWM (стерео на джек), USB-клавиатура (TinyUSB host); картридж записывается во флеш-память сразу за прошивкой | 378 МГц / 1.6 В | `6b98c8a79ed69533150e0669009907a76c2c79008595020150d06f3ebc42749a` |
| `sdcard/emu/PCp2-frank_apple-HDMI-378MHz-1.05.uf2` | [DnCraptor/murmapple](https://github.com/DnCraptor/murmapple) | 1.05, `b95b7c7` (2026-10-02), сборка без PSRAM | 378 МГц / 1.6 В | `088957d8cbb4b1b82de98bbef82766eb0a4e593dd7c198842a0521ef2ac32a0f` |
| `sdcard/emu/psram/PCp2-frank_apple-HDMI-378MHz-P84-1.05.uf2` | там же | 1.05, `9ca41fc` (2026-10-03), сборка с PSRAM 84 МГц | 378 МГц / 1.6 В | `4bfdf99cd9a92afa8ccaacc501819c923a7493232b2d319a4da1b7a50e2746b4` |

Частота и напряжение взяты из имени файла и исходников сборки (`boards/olimex-pico-pc.h` у pico-nes, таблица видеорежимов у PICO-BK). Образ pico-nes не содержит дополнительного блока UF2 для errata RP2350-E10; на запуск из launcher и MOS2 это не влияет.

В подкаталоге `emu/psram` лежат сборки, которым нужна внешняя PSRAM; на рабочих местах без PSRAM они не работают или работают не полностью. Сейчас там murmapple с PSRAM (держит в ней образы дисков и без PSRAM диски не загружает) и две «быстрые» сборки murm386: гостевая память у них целиком в PSRAM, без подкачки на SD-карту.

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

Здесь проверяются только файлы платы; общая часть карты проверяется по `SHA256SUMS` в корне репозитория (см. `SDCARD.md`, раздел 15).

## 4. Лицензии

MOS2, pico-launcher, PICO-BK и pico-nes распространяются под GPLv3, murm386 — под MIT, pico-z26 и atari800 — под GPLv2, эмулятор Game & Watch — под Apache 2.0, murmc64 — под GPLv2 или более поздней, frank-micro — под GPLv3 или более поздней, frank-msx — под GPLv3, кроме ядра fMSX, EMULib и эмулятора Z80 (Marat Fayzullin): у них собственная лицензия fMSX, порт для RP2350 сделан с разрешения автора (`LICENSE.fMSX` в репозитории frank-msx), emu80v4 — под GPLv3, pico-pce — под GPLv2 (лицензия ядра pce-go). Исходники соответствуют коммитам, указанным в разделе 1. Лицензии содержимого общей части карты — в [`../../SDCARD.md`](../../SDCARD.md).
