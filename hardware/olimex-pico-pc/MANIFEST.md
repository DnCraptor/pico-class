# Манифест комплекта: Olimex RP2040-PICO-PC + Raspberry Pi Pico 2 (PCp2)

Состояние на 2026-10-01. Файл фиксирует, **что именно** лежит в этом каталоге, из каких исходников оно собрано и как проверить целостность. При замене любого файла комплекта обновляются эта таблица, `SHA256SUMS` и статус в [`../COMPATIBILITY.md`](../COMPATIBILITY.md).

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
| `/freedos/bin/*`, `/freedos/configs/*`, `/freedos/nls/*`, `/freedos/version.fdi` | дистрибутив FreeDOS 1.4 (2025-04-02), выборка файлов | для загрузки murm386 прямо с SD-карты |
| `/fdconfig.sys`, `/FDAUTO.BAT` | конфигурация FreeDOS 1.4 | адаптированы под murm386: оболочка — Volkov Commander; концы строк CRLF |
| `/freedos/VC.COM` | Volkov Commander 4.05 | файловый менеджер, запускается из `FDAUTO.BAT` |
| `/freedos/SI.EXE` | Norton System Information | индекс производительности; полезен при проверке частоты murm386 |
| `/NES/*.nes` | свободные homebrew-игры для pico-nes, см. раздел 2.1 | см. раздел 2.1 |


### 2.1. Игры для pico-nes (`/NES`)

pico-nes ищет игры в каталоге `/NES` на SD-карте (расширение `.nes`). В подборку входят только игры, которые авторы разрешили свободно распространять. Коммерческие образы картриджей NES в комплект не входят.

| Файл | Игра | Жанр | Источник | Лицензия | Маппер, размер |
|---|---|---|---|---|---|
| `croom.nes` | Concentration Room | «Мемори», парные карточки | [pinobatch/croom-nes](https://github.com/pinobatch/croom-nes) `ed19c3c`, собрано из исходников | GPLv3; копии ROM разрешено распространять без исходников | 0, 24 КБ |
| `thwaite.nes` | Thwaite | защита городов от ракет | [pinobatch/thwaite-nes](https://github.com/pinobatch/thwaite-nes) `00e3674`, собрано из исходников | GPLv3 | 0, 40 КБ |
| `rfk.nes` | robotfindskitten | «дзен-симулятор», текст на английском | [pinobatch/rfk-nes](https://github.com/pinobatch/rfk-nes) `b9764ce`, собрано из исходников | zlib | 0, 32 КБ |
| `nova.nes` | Nova the Squirrel | платформер-головоломка, диалоги на английском | [NovaSquirrel/NovaTheSquirrel](https://github.com/NovaSquirrel/NovaTheSquirrel) `e9e79ae`, собрано из исходников | код — GPLv3, графика и уровни — CC BY-NC-SA 4.0 | 1 (MMC1), 256 КБ |
| `jetpaco.nes` | Jet Paco | платформер с реактивным ранцем | [mojontwins/MK1_NES](https://github.com/mojontwins/MK1_NES) `ec3fbcc`, `examples/` | игры из `examples` авторы разрешают свободно распространять | 0, 40 КБ |
| `lala.nes` | Lala the Magical: Prologue | платформер | там же | там же | 0, 40 КБ |
| `bootee.nes` | Bootèe | платформер | там же | там же | 0, 40 КБ |
| `cheril_bosque.nes` | Cheril of the Bosque | приключенческий платформер | там же | там же | 0, 40 КБ |
| `cheril_perils.nes` | Cheril Perils Classic | платформер | там же | там же | 0, 40 КБ |
| `cheril_goddess.nes` | Cheril the Goddess | платформер | там же | там же | 3 (CNROM), 64 КБ |
| `cheril_writer.nes` | Cheril the Writer | платформер | там же | там же | 3 (CNROM), 64 КБ |
| `espitene.nes` | Espitene | платформер | там же | там же | 3 (CNROM), 64 КБ |
| `cadaverion.nes` | Cadàveriön | головоломка в подземелье | там же | там же | 0, 40 КБ |
| `dveelng.nes` | D'Veel'Ng | экшен с видом сверху | там же | там же | 0, 40 КБ |

Все мапперы (0, 1, 3) поддерживаются pico-nes. Игры Damian Yerrick и Nova the Squirrel собраны из исходников указанных коммитов (cc65 2.19); игры Mojon Twins взяты готовыми из репозитория. Файлы переименованы в короткие имена без апострофов и диакритики.

Из примеров MK1_NES не включены Che-Man (пародия с политическим персонажем) и Sgt. Helmet: Training Day (военный шутер). Решение о включении остальных игр руководитель принимает после просмотра с учётом возраста группы.
## 3. Проверка целостности

Из каталога `hardware/olimex-pico-pc`:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Файлы `*.md` в `SHA256SUMS` не включены: git может менять в них концы строк при checkout, а на работу прошивок они не влияют.

## 4. Лицензии

MOS2, её приложения, pico-launcher, PICO-BK и pico-nes распространяются под GPLv3, murm386 — под MIT. Исходники соответствуют коммитам, указанным в разделе 1. Собственные файлы репозитория (документы, `/test/*`) распространяются на условиях [`LICENSE`](../../LICENSE).
