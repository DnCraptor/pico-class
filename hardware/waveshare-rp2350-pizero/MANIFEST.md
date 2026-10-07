# Манифест комплекта: Waveshare RP2350-PiZero (z0p2)

Состояние на 2026-10-07: есть pico-launcher и murm386; MOS2 ещё не собран. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность — по образцу [`../olimex-pico-pc/MANIFEST.md`](../olimex-pico-pc/MANIFEST.md). При добавлении или замене любого файла обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `pico-launcher/z0p2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 — тот же коммит, что для PCp2 | профиль платы z0p2, `HID=ON` (по имени файла; HDMI) | `48d1643e4f05f778c24a38032d5cfaf047c6d87491120fc707e387c0c750eb21` |
| `murmulator-os2/…` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | — | — | — |

### Эмуляторы (`sdcard/emu/`)

| Файл | Исходники | Версия / коммит | Частота / напряжение | SHA-256 |
|---|---|---|---|---|
| `sdcard/emu/z0p2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `fc80323` (2026-10-06) — тот же коммит, что для PCp2; профиль платы `BOARD_Z2`; сборка без PSRAM (`VIDEO_MODE=RUNTIME`, подкачка гостевой памяти) | 504 МГц / 1.6 В | `a2de24ba1f24304796dc3116e6c634453edfc8c7be4a3ab0c616ebaec4afd720` |
| `sdcard/emu/psram/z0p2-386-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, `BOARD_Z2`; сборка с PSRAM: ядро 386, VGA256, гостевая память прямо в PSRAM 66 МГц (`NO_PAGING`) | 504 МГц / 1.6 В | `140828daf4994c2a3b856214f4457c0667d702e298e12a67fdbbf97f7fa8239c` |
| `sdcard/emu/psram/z0p2-286-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, `BOARD_Z2`; сборка с PSRAM: ядро 8086/186/286 (`CPU_TARGET=286`), остальное как у сборки 386 | 504 МГц / 1.6 В | `4faa210a991986c71dcf0427ae3300f5ef35ee495d8c5dcaf07d80aadb89841a` |

Звук у murm386 на этой плате — I2S или PWM на GP10–GP12, выбор в **Win+F11** (Audio output: Autodetect / PWM / I2S). Свои настройки сборки для z0p2 хранят в `/.config/386/Z2/` и `/.config/286/Z2/`, отдельно от настроек PCp2: если карту переставить с платы на плату, настройки одной не испортят другую. В подкаталоге `emu/psram` — сборки, которым нужна внешняя PSRAM (как у Olimex, см. [`../olimex-pico-pc/MANIFEST.md`](../olimex-pico-pc/MANIFEST.md)).

## 2. Платформенная часть SD-карты (`sdcard/`)

Карту готовят как для Olimex: сначала общий `sdcard/` из корня репозитория, затем поверх — `sdcard/` этой платы.

| Путь на карте | Происхождение | Отличия от источника |
|---|---|---|
| `/emu/*.uf2`, `/emu/psram/*.uf2` | см. раздел 1, «Эмуляторы» | побайтово совпадают с файлами сборки |

## 3. Проверка целостности

Из каталога `hardware/waveshare-rp2350-pizero`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Здесь проверяются только файлы платы; общая часть карты — по `SHA256SUMS` в корне репозитория (см. `SDCARD.md`, раздел 22).

## 4. Лицензии

murm386 распространяется под MIT. Остальное будет дописано вместе с файлами — по образцу манифеста Olimex.
