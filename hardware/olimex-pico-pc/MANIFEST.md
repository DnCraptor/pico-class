# Манифест комплекта: Olimex RP2040-PICO-PC + Raspberry Pi Pico 2 (PCp2)

Состояние на 2026-10-01. Файл фиксирует, **что именно** лежит в этом каталоге, из каких исходников оно собрано и как проверить целостность. При замене любого файла комплекта обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `murmulator-os2/PCp2-murmulator-os-VGA-HDMI-HID-2.3.2-252MHz-8.uf2` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | тег `v.2.3.2` = `aad5813` (2026-07-15), build 8 | `PICO_PC=ON`, `VGAHDMI=ON`, `HID=ON`, `CPU_MHZ=252` | `9cdb1263981fdb7df9e3e2a887afb73e24679a9f3e2ff30751399f50bf442c73` |
| `pico-launcher/PCp2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 | `PICO_PC=ON`, `HID=ON`, `MinSizeRel` (HDMI включается автоматически) | `2cde325b9a32b3a6a569bdb28421338559cfb660445232f7f1031926741e0334` |

Параметры сборки MOS2 восстановлены по имени файла согласно правилам формирования имени в `CMakeLists.txt` upstream. Оба образа собраны для RP2350 в режиме ARM Secure (UF2 family `0xe48bff59`).

### Эмуляторы (`sdcard/emu/`)

Запускаются из pico-launcher или MOS2. Сами выставляют частоту и напряжение ядра при старте.

| Файл | Исходники | Версия / коммит | Частота / напряжение | SHA-256 |
|---|---|---|---|---|
| `sdcard/emu/PCp2-bk-VGA-HDMI-400MHz-PWM-1.4.9.uf2` | [DnCraptor/PICO-BK](https://github.com/DnCraptor/PICO-BK) | 1.4.9, `master` = `e1d08ab` (2026-10-01) | 400 МГц / 1.5 В | `efb2ba0ca3927760370cc5e41a06d1aaf069cedbf9c42bf98b4af38407ebd318` |
| `sdcard/emu/PCp2-nes-VGA-HDMI-PWM-305.uf2` | [DnCraptor/pico-nes](https://github.com/DnCraptor/pico-nes) | 305, `main` = `fa24bc7` (2026-10-01) | 378 МГц / 1.6 В | `3557d29136b2d88de534c984149bd7978379f588d75189faced817f1dd398d74` |
| `sdcard/emu/PCp2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `6d87963` (2026-10-01) | 504 МГц / 1.6 В | `43dd2c1a5a5334cd1affa2f880fb20065ef19b830334535e5c81b8c40de1e8c7` |

Частота и напряжение взяты из имени файла и исходников сборки (`boards/olimex-pico-pc.h` у pico-nes, таблица видеорежимов у PICO-BK). Образ pico-nes не содержит дополнительного блока UF2 для errata RP2350-E10; на запуск из launcher и MOS2 это не влияет.

## 2. Содержимое SD-карты (`sdcard/`)

Каталог `sdcard/` — зеркало корня учебной SD-карты: его **содержимое** копируется в корень карты как есть.

| Путь на карте | Происхождение | Отличия от источника |
|---|---|---|
| `/mos2/*` (54 файла приложений) | `apps/compiled` из murmulator-os2, тег `v.2.3.2` (`aad5813`) | побайтово совпадают |
| `/mos2/config.sys` | там же | пути приведены к единому регистру (`/mos2`), концы строк CRLF |
| `/mos2/UPSTREAM-README.md` | там же, `README.md` | только переименован |
| `/mos2/README.md` | этот репозиторий | памятка по MOS2 на PCp2; кодировка CP866, чтобы файл читался в MOS2 (`type`, `mcview`) |
| `/test/kbdtest.bas` | этот репозиторий | утилита проверки клавиатуры для MOS2 (Stefan's BASIC) |
| `/test/edit.txt` | этот репозиторий | файл для проверки просмотра, редактора и записи на SD |
| `/test/snd_mono.wav` | этот репозиторий | 8000 Гц, моно, 16 бит: три сигнала 440/660/880 Гц, уровень −12 dBFS |
| `/test/snd_lr.wav` | этот репозиторий | 8000 Гц, стерео, 16 бит: 440 Гц слева, 660 Гц справа, 880 Гц в обоих каналах, −12 dBFS |
| `/emu/*.uf2` | см. раздел 1, «Эмуляторы» | побайтово совпадают с файлами сборки |

## 3. Проверка целостности

Из каталога `hardware/olimex-pico-pc`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Файлы `*.md` в `SHA256SUMS` не включены: git может менять в них концы строк при checkout, а на работу прошивок они не влияют.

## 4. Лицензии

MOS2, её приложения, pico-launcher, PICO-BK и pico-nes распространяются под GPLv3, murm386 — под MIT. Исходники соответствуют коммитам, указанным в разделе 1. Собственные файлы репозитория (документы, `/test/*`) распространяются на условиях [`LICENSE`](../../LICENSE).
