# Манифест комплекта: Olimex RP2040-PICO-PC + Raspberry Pi Pico 2 (PCp2)

Состояние на 2026-10-01. Файл фиксирует, **что именно** лежит в этом каталоге, из каких исходников оно собрано и как проверить целостность. При замене любого файла комплекта обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `murmulator-os2/PCp2-murmulator-os-VGA-HDMI-HID-2.3.2-252MHz-8.uf2` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | тег `v.2.3.2` = `aad5813` (2026-07-15), build 8 | `PICO_PC=ON`, `VGAHDMI=ON`, `HID=ON`, `CPU_MHZ=252` | `9cdb1263981fdb7df9e3e2a887afb73e24679a9f3e2ff30751399f50bf442c73` |
| `pico-launcher/PCp2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 | `PICO_PC=ON`, `HID=ON`, `MinSizeRel` (HDMI включается автоматически) | `2cde325b9a32b3a6a569bdb28421338559cfb660445232f7f1031926741e0334` |

Параметры сборки MOS2 восстановлены по имени файла согласно правилам формирования имени в `CMakeLists.txt` upstream. Оба образа собраны для RP2350 в режиме ARM Secure (UF2 family `0xe48bff59`).

## 2. Содержимое SD-карты (`sdcard/`)

Каталог `sdcard/` — зеркало корня учебной SD-карты: его **содержимое** копируется в корень карты как есть.

| Путь на карте | Происхождение | Отличия от источника |
|---|---|---|
| `/mos2/*` (54 файла приложений) | `apps/compiled` из murmulator-os2, тег `v.2.3.2` (`aad5813`) | побайтово совпадают |
| `/mos2/config.sys` | там же | пути приведены к единому регистру (`/mos2`) |
| `/mos2/UPSTREAM-README.md` | там же, `README.md` | только переименован |
| `/mos2/README.md` | этот репозиторий | памятка по MOS2 на PCp2; кодировка CP866, чтобы файл читался в MOS2 (`type`, `mcview`) |
| `/test/kbdtest.bas` | этот репозиторий | утилита проверки клавиатуры (Stefan's BASIC) |
| `/test/snd_mono.wav` | этот репозиторий | 8000 Гц, моно, 16 бит: три сигнала 440/660/880 Гц, уровень −12 dBFS |
| `/test/snd_lr.wav` | этот репозиторий | 8000 Гц, стерео, 16 бит: 440 Гц слева, 660 Гц справа, 880 Гц в обоих каналах, −12 dBFS |

## 3. Проверка целостности

Из каталога `hardware/olimex-pico-pc`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Файлы `*.md` в `SHA256SUMS` не включены: git может менять в них концы строк при checkout, а на работу прошивок они не влияют.

## 4. Лицензии

MOS2, её приложения и pico-launcher распространяются под GPLv3. Исходники соответствуют коммитам, указанным в разделе 1. Собственные файлы репозитория (документы, `/test/*`) распространяются на условиях [`LICENSE`](../../LICENSE).
