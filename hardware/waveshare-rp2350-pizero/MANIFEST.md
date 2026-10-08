# Манифест комплекта: Waveshare RP2350-PiZero (z0p2)

Состояние на 2026-10-07: есть pico-launcher, MOS2, murm386, PICO-BK, pico-nes и pico-speccy. Файл фиксирует, **что именно** лежит в каталоге платы, из каких исходников оно собрано и как проверить целостность — по образцу [`../olimex-pico-pc/MANIFEST.md`](../olimex-pico-pc/MANIFEST.md). При добавлении или замене любого файла обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

## 1. Менеджеры прошивок (UF2)

| Файл | Исходники | Версия / коммит | Параметры сборки | SHA-256 |
|---|---|---|---|---|
| `pico-launcher/z0p2-uf2-launcher-HDMI-HID.uf2` | [DnCraptor/pico-launcher](https://github.com/DnCraptor/pico-launcher) | `main` = `5d871d7` (2026-10-01), версия 4 — тот же коммит, что для PCp2 | профиль платы z0p2, `HID=ON` (по имени файла; HDMI) | `48d1643e4f05f778c24a38032d5cfaf047c6d87491120fc707e387c0c750eb21` |
| `murmulator-os2/z0p2-murmulator-os-VGA-HDMI-HID-2.3.2-252MHz-8.uf2` | [DnCraptor/murmulator-os2](https://github.com/DnCraptor/murmulator-os2) | 2.3.2 build 8, `bd11190` (2026-10-07): `v.2.3.2` (`aad5813`) плюс правки для z0p2 — видеодрайвер HDMI по умолчанию и звук через I2S-модуль (`I2S_SOUND`, вывод I2S на `pio2`, постоянная частота 44100 Гц) | `ZERO2=ON`, `VGAHDMI=ON`, `HID=ON`, `CPU_MHZ=252` | `729952752ac4989c0d19e3028f37cdcf8e776beb7890976b47bb6855031e69f2` |

### Эмуляторы (`sdcard/emu/`)

| Файл | Исходники | Версия / коммит | Частота / напряжение | SHA-256 |
|---|---|---|---|---|
| `sdcard/emu/z0p2-386-RUNTIME-504MHz-1.6V-P66-v1.19.uf2` | [DnCraptor/murm386](https://github.com/DnCraptor/murm386) | v1.19, `main` = `fc80323` (2026-10-06) — тот же коммит, что для PCp2; профиль платы `BOARD_Z2`; сборка без PSRAM (`VIDEO_MODE=RUNTIME`, подкачка гостевой памяти) | 504 МГц / 1.6 В | `a2de24ba1f24304796dc3116e6c634453edfc8c7be4a3ab0c616ebaec4afd720` |
| `sdcard/emu/psram/z0p2-386-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, `BOARD_Z2`; сборка с PSRAM: ядро 386, VGA256, гостевая память прямо в PSRAM 66 МГц (`NO_PAGING`) | 504 МГц / 1.6 В | `140828daf4994c2a3b856214f4457c0667d702e298e12a67fdbbf97f7fa8239c` |
| `sdcard/emu/psram/z0p2-286-VGA256-504MHz-1.6V-P66-v1.19.uf2` | там же | v1.19, `fc80323`, `BOARD_Z2`; сборка с PSRAM: ядро 8086/186/286 (`CPU_TARGET=286`), остальное как у сборки 386 | 504 МГц / 1.6 В | `4faa210a991986c71dcf0427ae3300f5ef35ee495d8c5dcaf07d80aadb89841a` |
| `sdcard/emu/z0p2-bk-VGA-HDMI-400MHz-I2S-1.4.9.uf2` | [DnCraptor/PICO-BK](https://github.com/DnCraptor/PICO-BK) | 1.4.9, `1dc08e9` (2026-10-07): `8c0e735` (тот же, что для PCp2) плюс вывод I2S на `pio2` для RP2350; `ZERO2=ON`, `AY_TYPE=I2S` — звук через модуль I2S на GP10–GP12 и параллельно по HDMI | как у PCp2: частота растёт вместе с видеорежимом HDMI (меню Home); на z0p2 проверено 400 МГц / 1.5 В | `5b665a339fbdbc008c0cf5c882e90654df1286b40d5a017a0c9ef1219ab5e3e5` |
| `sdcard/emu/z0p2-nes-VGA-HDMI-I2S-TDA1387-305.uf2` | [DnCraptor/pico-nes](https://github.com/DnCraptor/pico-nes) | 305, `0d6dbbe` (2026-10-08): `fa24bc7` (тот же, что для PCp2) плюс правки для z0p2 — профиль платы `rp2350pizero` без `pico2.h` (иначе SDK считает чип RP2350A и HDMI на GPIO32–39 не запускается), I2S на `pio2`, геймпад NES на GP4/GP5/GP7 (второй — GP8) и PS/2 на GP2/GP3 как в MOS2; `PICO_BOARD=rp2350pizero`, `I2S=ON`, HDMI | 252 МГц / 1.6 В | `b443c72c3a907867cfad1b3e74e96e2c2eafdd5b063cd6d492306e3acb2f962c` |
| `sdcard/emu/z0p2-speccy-VGA-HDMI-1.0.9.uf2` | [drewpo28/pico-speccy](https://github.com/drewpo28/pico-speccy) | релиз [v1.0.9](https://github.com/drewpo28/pico-speccy/releases/tag/v1.0.9), тег `v1.0.9` = `f5f57d4` (2026-10-07), готовая сборка из релиза; `ZERO2=ON`, HDMI через PIO; звук I2S на GP10–GP12 (как у murm386) или плата Waveshare PCM5122 Audio Board | по умолчанию сборки, меняется в меню | `f5631b5c74e07b4a691806908b82e3431138cf1ebc301ea37f829e9f856c115d` |
| `sdcard/emu/z0p2-speccy-VGA-HDMI-PIOUSB-1.0.9.uf2` | там же | v1.0.9, `f5f57d4`; то же плюс `ZERO2_PIO_USB`: второй разъём Type-C (J2, GP28/GP29) работает как ещё один USB-хост через PIO — клавиатуру и геймпад можно подключить без хаба | по умолчанию сборки, меняется в меню | `b94859db4d3a044fcc187e5c4a8887fa6504cc812c75ca472a93ac0c5a8c3c0b` |

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

pico-launcher, MOS2, PICO-BK, pico-nes и pico-speccy распространяются под GPLv3, murm386 — под MIT. Остальное будет дописано вместе с файлами — по образцу манифеста Olimex.
