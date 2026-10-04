# Общее содержимое SD-карты

Состояние на 2026-10-04. Каталог `sdcard/` в корне репозитория — общая для всех плат часть учебной SD-карты: MOS2 и её приложения, FreeDOS для murm386, игры и учебные программы, файлы для приёмки рабочего места. Здесь описано, что в нём лежит, откуда взято и как проверить целостность. При изменении состава обновляются этот файл и [`SHA256SUMS`](SHA256SUMS).

## 1. Порядок подготовки карты

1. Отформатировать карту в FAT32 с размером кластера 4 КБ — см. инструкцию к своей плате (для Olimex PICO-PC — [`hardware/olimex-pico-pc/README.md`](hardware/olimex-pico-pc/README.md), раздел 3).
2. Скопировать **содержимое** общего каталога [`sdcard/`](sdcard/) в корень карты.
3. Затем скопировать поверх **содержимое** каталога `sdcard/` своей платы — например, [`hardware/olimex-pico-pc/sdcard/`](hardware/olimex-pico-pc/sdcard/) для Olimex PICO-PC. Там лежат прошивки, собранные именно под эту плату (эмуляторы в `/emu`); при совпадении имён файлы платы заменяют общие.

## 2. Состав

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
| `/freedos/bin/*`, `/freedos/configs/*`, `/freedos/nls/*`, `/freedos/version.fdi` | FreeDOS 7.1, выборка файлов (в `version.fdi` указана 1.4 от 2025-04-02 — внутренняя версия исходного ядра) | для загрузки murm386 прямо с SD-карты |
| `/fdconfig.sys`, `/FDAUTO.BAT` | конфигурация FreeDOS 7.1 | адаптированы под murm386: оболочка — Volkov Commander, в `PATH` добавлен `\FREEDOS\EDU\TP55`; концы строк CRLF |
| `/freedos/VC.COM` | Volkov Commander 4.05 | файловый менеджер, запускается из `FDAUTO.BAT` |
| `/freedos/SI.EXE` | Norton System Information | индекс производительности; полезен при проверке частоты murm386 |
| `/freedos/LTEMM.EXE` | LTEMM r01 — драйвер EMS для платы Lo-tech 2MB EMS (A. Tsourikov, 1988; доработка Lo-tech), [lo-tech.co.uk](https://www.lo-tech.co.uk/wiki/LTEMM.EXE) | murm386 эмулирует эту плату (окно D000). В `/fdconfig.sys` строка подключения пока закомментирована (`REM`); чтобы включить EMS, убрать `REM` и перезагрузить. Исходный код — BSD 3-Clause, сборка Lo-tech — Standard Lo-tech License |
| `/386/bios.bin`, `/386/vgabios.bin` | SeaBIOS и SeaVGABIOS, сборка `rel-1.17.0-7-g106549a4-dirty` (2026-03-27); LGPL v3 | гостевой BIOS для сборок murm386 с ядром 386 (каталог данных `/386`; у сборки 286 — `/286`). По умолчанию murm386 работает со встроенным BIOS и грузит FreeDOS с SD-карты; SeaBIOS выбирается в Disk Manager (Win+F12), строка BIOS, нужна перезагрузка |
| `/386/fdd0free.img` | образ дискеты 1,44 МБ с FreeDOS 7.1 (та же выборка, что в `/freedos`: ядро, `COMMAND.COM`, `bin`, `nls`, `configs`; `FDCONFIG.SYS` и `FDAUTO.BAT` как на SD-карте), а также Volkov Commander, CuteMouse, LTEMM, CheckIt 1 | гостевая ОС для murm386 с ядром 386; дискета подключается в Disk Manager (Win+F12), строка FDD-0. FreeDOS — GPL; CheckIt — коммерческая программа |
| `/386/fdd0dr81.img` | образ дискеты 1,44 МБ с DR-DOS 8.1 (DrDOS, Inc., 2005): `COMMAND.COM`, `SYS.COM`, `MEM.EXE` и др., FDXXMS, UMBPCI, CuteMouse, LTEMM, Volkov Commander, Norton SI, CheckIt 1 и 3 | то же, другая гостевая ОС. DR-DOS 8.1 — проприетарная («All rights reserved»), CheckIt и Norton SI — коммерческие программы; образ будет удалён из репозитория при претензии правообладателя |
| `/NES/*.nes` | свободные homebrew-игры для pico-nes, см. раздел 3 | см. раздел 3 |
| `/ZX/*` | свободные программы и игры для pico-speccy, см. раздел 5 | см. раздел 5 |
| `/apple/*.DSK` | образы дисков для murmapple (Apple IIe), см. раздел 6 | см. раздел 6 |
| `/z26/*.bin` | свободные homebrew-игры для pico-z26 (Atari 2600), см. раздел 7 | см. раздел 7 |
| `/atari800/*` | свободные программы для atari800 (Atari 400/800, XL/XE, 5200), см. раздел 8 | см. раздел 8 |
| `/c64/*.PRG` | свободные программы для murmc64 (Commodore 64), см. раздел 9 | см. раздел 9 |
| `/freedos/GAMES/*`, `/freedos/EDU/*` | свободные программы для murm386, см. раздел 4 | см. раздел 4 |

Памятка `/mos2/README.md` написана для сборки MOS2 под Olimex PICO-PC (PCp2): видеорежимы, выводы звука и геймпада в ней указаны для этой платы.

## 3. Игры для pico-nes (`/NES`)

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

## 4. Программы для murm386 (`/freedos/GAMES`, `/freedos/EDU`)

Подборка рассчитана на производительность murm386 без PSRAM: примерно 2–3 PC XT, то есть уровень Turbo XT или PC AT 8 МГц. Без PSRAM быстрее не получится из-за частого свопа. Уменьшить число промахов кэша помогают режимы EGA128 и VGA128 (дополнительно 128 КБ SRAM) и MCGA (ещё 64 КБ). Программы, которым нужна PSRAM, в комплект не входят — их можно добавить отдельно. На рабочих местах с PSRAM можно запускать сборки murm386 из `/emu/psram` (см. `hardware/olimex-pico-pc/MANIFEST.md`): гостевая память у них целиком в PSRAM, своп не нужен.

Отбор: скорость уровня XT/AT; видео текстовое, CGA, EGA или VGA/MCGA. murm386 поддерживает XMS, поэтому программы, использующие расширенную память, тоже подходят — ограничение задаёт скорость, а не объём памяти. Источник и условия распространения каждой программы указаны в таблице. Запуск — из Volkov Commander: зайти в каталог, Enter на `.EXE`/`.COM`.

| Каталог | Программа | Что это | Видео | Источник | Условия распространения |
|---|---|---|---|---|---|
| `/freedos/GAMES/KROZ/KINGDOM` | Kingdom of Kroz (1987) | первая игра Apogee, текстовый аркадный лабиринт | текст | [tangentforks/kroz](https://github.com/tangentforks/kroz) `5d080fb` — официальный freeware-релиз Apogee 2009 г. | freeware: бесплатно играть и распространять, продавать и использовать материалы нельзя (`/freedos/GAMES/KROZ/KROZ.TXT`) |
| `/freedos/GAMES/KROZ/KINGDOM2`, `CAVERNS2`, `DUNGEON2`, `RETURN`, `TEMPLE`, `CRUSADE`, `LOST` | остальные эпизоды Kroz (1990) | то же | текст | там же | там же |
| `/freedos/GAMES/TYPEFAST` | TypeFast | клавиатурный тренажёр в виде игры: успеть набрать падающие слова | текст | [clasqm/freedos-repo](https://github.com/clasqm/freedos-repo) `5254279` | авторы не заявляли авторских прав, просят сохранять комментарии в исходниках |
| `/freedos/GAMES/DRMIND` | Dr. Mind Lite | «Быки и коровы» (Mastermind) в цвете | VGA/MCGA | там же | CC BY-ND 4.0 |
| `/freedos/GAMES/DROBOS` | Drobos | «Далеки»: увести героя от роботов, сталкивая их между собой | EGA | там же | свободное распространение, без продажи |
| `/freedos/GAMES/SHUFFLE` | Shuffle V | «Пятнашки» | текст | там же | freeware для некоммерческого использования (J. R. Ferguson). **Известная проблема:** в murm386 v1.19 зависает при запуске во всех трёх сборках (Turbo Pascal, интерфейс в стиле Turbo Vision; файл сжат LZEXE 0.91) |
| `/freedos/GAMES/HANOI` | Towers of Hanoi | «Ханойская башня» — классическая рекурсивная задача | текст | там же | то же |
| `/freedos/GAMES/LINES` | Color Lines (Gamos, 1992) | «Линии»: собирать ряды из пяти шариков одного цвета; правила — `LINES.TXT` | EGA | загружено вручную | не указаны |
| `/freedos/GAMES/LIFE` | Life2 | «Жизнь» Конвея | текст | там же | freeware по данным freedos-repo |
| `/freedos/GAMES/NERO5` | Nero 5 | шахматы | текст | там же | то же |
| `/freedos/EDU/ELAN` | Elan-1 1.5 | учебный язык программирования (Университет Неймегена), с черепашьей графикой и роботом Karel | текст, графика | там же | копировать и распространять бесплатно, целиком и с уведомлением об авторских правах (`READ.1ST`). Нужен драйвер `ANSI.SYS` |
| `/freedos/EDU/robotlnd` | Роботландия (Ю. Первин) | курс информатики для младших школьников: клавиатура, исполнители, игры-головоломки. Запуск — `MENU.EXE` | графика | загружено вручную | не указаны |
| `/freedos/EDU/ALGORITM` | Алгоритмика (ИНТ) | программирование для детей через исполнителей. Ученик — `INF.EXE`, учитель («Учительская»: список учеников, результаты) — `TI.EXE`; описание — `ALGOR.DOC`, `ALGOR_U.DOC` | графика | загружено вручную | не указаны |
| `/freedos/EDU/LOGO`, `/freedos/EDU/LOGO_DOC` | LogoWriter 3, русская версия ИНТ | Лого с черепашьей графикой. Запуск — `LOGOWR.COM`; документация — `LOGO_DOC` | графика | загружено вручную | не указаны |
| `/freedos/EDU/TP55` | Turbo Pascal 5.5 (Borland, 1989) | среда и компилятор Pascal с примерами (`*.PAS`), BGI-графикой и документацией (`DOC`, `README`); в каталоге `TURBO3` — средства совместимости с TP 3.0. Запуск — `TURBO.EXE`, каталог включён в `PATH` | текст, графика BGI | Borland Museum / Embarcadero Antique Software | Embarcadero: бесплатно, «as is», только для личного использования; распространение через интернет и на носителях не разрешено. Будет удалено из репозитория при претензии правообладателя |
| `/freedos/EDU/ELEMENTS` | Elements | периодическая таблица | текст | там же | freeware по данным freedos-repo |
| `/freedos/EDU/GRAPHXY` | GraphXY | построение графиков функций | EGA/VGA | там же | freeware. Режим VGA 640×480, 16 цветов, драйвер `EGAVGA.BGI` и шрифты `*.CHR` загружаются из текущего каталога — запускать из `\FREEDOS\EDU\GRAPHXY`. **Известная проблема:** в murm386 v1.19 работает только с ядром 386 и видеоадаптером VGA256: в сборке 386 с PSRAM, а в сборке без PSRAM — при выборе VGA256 в меню Win+F11 (Video adapter), медленно. С VGA128 и более простыми адаптерами, а также в сборке 286 не запускается |

Файлы взяты из пакетов freedos-repo без изменений, кроме имён: в нескольких случаях упрощены до 8.3 (`hanoie.exe` → `HANOI.EXE`, `Kingdom.com` → `KINGDOM.COM`). На murm386 v1.19 Shuffle V не работает ни в одной сборке, GraphXY работает только с ядром 386 и видеоадаптером VGA256 (см. таблицу); остальные программы поштучно ещё не проверялись.

## 5. Программы и игры для pico-speccy (`/ZX`)

pico-speccy открывает файлы из любого каталога SD-карты через файловый браузер (F5) и понимает почти все форматы Спектрума: `.tap`, `.tzx`, `.z80`, `.sna`, `.trd`, `.scl` и др. Файлы ZX81 (`.p`) открываются клавишей F2; эмулятор сам переключается в режим ZX81+. Подборка лежит в `/ZX`.

Управление: **F1** — меню эмулятора (модель компьютера — Machine → Spectrum: 48K, 128K, +2, +3, ZX81+ и др.); **F5** — открыть файл, Enter — загрузить и запустить; **Shift** — CAPS SHIFT; **Ctrl** — SYMBOL SHIFT; **Esc** — BREAK; **F11** — сброс Спектрума; **F9/F10** — громкость.

Рядом с каждым файлом лежит файл `.TXT` с описанием, авторами и текстом лицензии. Файлы переименованы в короткие имена, содержимое не изменено.

Игры The Mojon Twins. Свою работу (игры, графику, музыку, исходники) авторы распространяют по лицензии [CC BY-NC-SA 3.0](https://creativecommons.org/licenses/by-nc-sa/3.0/): свободное некоммерческое распространение с указанием авторства. Источник файлов — [retrobrews/zxspectrum-games](https://github.com/retrobrews/zxspectrum-games), коммит `b3cb92e`.

| Файл | Игра | Жанр |
|---|---|---|
| `COLUMNS.TZX` | Columns | «Колонны» — головоломка: собирать ряды из трёх одинаковых камней |
| `CHERILB.TZX` | Cheril of the Bosque | приключенческий платформер |
| `CHERILP.TZX` | Cheril Perils | платформер |
| `CHERILG.TAP` | Cheril the Goddess | платформер |
| `LALA.TZX` | Lala the Magical: Prologue | платформер |
| `CADAVER.TAP` | Cadàveriön | головоломка в подземелье |
| `MARIANO.TZX` | Mariano the Dragon: Capers in Cityland | аркада |
| `ABABOL.TZX` | Sir Ababol | платформер |
| `ABABOL2.TAP` | Sir Ababol 2: The Ice Palace | платформер |

Cheril the Goddess, Cheril Perils, Cheril of the Bosque, Lala Prologue и Cadàveriön есть и в подборке для NES (раздел 3) — одну и ту же игру можно сравнить на двух платформах: графику, цвет, звук, управление.

Во вступлении к Sir Ababol — шуточная история про крестоносца.

Другие программы:

| Файл | Программа | Что делать | Машина | Источник | Лицензия |
|---|---|---|---|---|---|
| `MAZE3D.TAP` | ZXMAZE3D (Andrej Oresnik) | трёхмерный лабиринт: W — вперёд, S — назад, A и D — повороты | 48K и выше | [aoresnik/zxmaze3d](https://github.com/aoresnik/zxmaze3d), ветка `gh-pages` (`79a7f9e`), `files/zxmaze3d.tap` | BSD (3 пункта) |

## 6. Образы дисков для Apple IIe (`/apple`)

murmapple ищет образы дисков в каталоге `/apple` (форматы `.dsk`, `.nib`, `.woz`, `.bdsk`); диск выбирается в экранном меню эмулятора. Подборка рассчитана в первую очередь на проверку картинки и звука и на знакомство с Apple II; все образы — DOS 3.3, 140 КБ.

| Файл | Что это | Что проверяет | Автор, источник | Лицензия |
|---|---|---|---|---|
| `SHORTPRG.DSK` | Short Programs — сборник коротких программ на Applesoft BASIC в одну-две строки: графика, анимация, звук | графика HGR/LORES, BASIC; хороший материал для модуля программирования — программы короткие и их можно разбирать | Lee Fastenau, [thelbane/Apple-II-Programs](https://github.com/thelbane/Apple-II-Programs) `3bc2568` | MIT |
| `FIRE.DSK` | Fire — небольшие эффекты «огня» | графика LORES | Vince Weaver, [deater/dos33fsprogs](https://github.com/deater/dos33fsprogs) `3a02734` | GPL v2 |
| `TB6502.DSK` | Tom Bombem — аркадная «стрелялка» | игровой процесс, звук | там же | GPL v2 |
| `TFV.DSK` | Talbot Fantasy 7 — шуточная ролевая игра в режиме LORES | игровой процесс, клавиатура | там же | GPL v2 |

Образы взяты из репозиториев без изменений, переименованы в короткие имена. Проверка 2026-10-02 на PCp2: образы подборки работают в обеих сборках murmapple. Образов для проверки Mockingboard в подборке нет.

Как запустить диск: F11 — экранное меню дисков, выбрать образ, затем Boot. Если программа не стартовала сама, в Applesoft BASIC: `CATALOG` — список файлов на диске, `RUN имя` — запустить BASIC-программу (тип `A`), `BRUN имя` — машинный код (тип `B`); `PR#6` — перезагрузка с дисковода.

## 7. Игры для pico-z26 (`/z26`)

pico-z26 ищет игры в каталоге `/z26` на SD-карте (расширения `.bin`, `.a26`, `.rom`). В подборку входят только свободные homebrew-игры с открытой лицензией; коммерческие образы картриджей Atari 2600 в комплект не входят. Управление — см. строку pico-z26 в [`hardware/COMPATIBILITY.md`](hardware/COMPATIBILITY.md): стрелки — джойстик, Z — кнопка, Enter — RESET (старт), Esc — SELECT.

| Файл | Игра | Что делать | Источник | Лицензия | Размер, схема банков |
|---|---|---|---|---|---|
| `2048.bin` | 2048 2600 (Carlos Duarte do Nascimento, 2014) | головоломка «2048»: сдвигать плитки джойстиком и складывать одинаковые числа; старт — кнопкой | [chesterbr/2048-2600](https://github.com/chesterbr/2048-2600) `35de3c1` | MIT | 2 КБ |
| `berta.bin` | Berta and Butterflies (vandalton, 2024) | слонёнок Берта ловит бабочек с четырёх сторон — по мотивам «Ну, погоди!» / Game & Watch «Egg» | [vandalton/BertaAndButterflies](https://github.com/vandalton/BertaAndButterflies), релиз v1.00 (`7f81d40`), `berta-and-butterflies.v1.00.ntsc.en.bin` | MIT | 4 КБ |

Образы взяты из репозиториев без изменений (у Berta — из релиза на GitHub), переименованы в короткие имена. Исходники — в тех же репозиториях. Проверка 2026-10-03 на PCp2 с pico-z26 4.0.8 (`9689fc5`): обе игры работают. Игры 8 КБ и больше с переключением банков в этой версии pico-z26 не запускаются, см. [`hardware/COMPATIBILITY.md`](hardware/COMPATIBILITY.md).

## 8. Программы для atari800 (`/atari800`)

atari800 хранит файлы в каталоге `/atari800` на SD-карте и открывает в нём выбор файлов. Отдельные образы ПЗУ не нужны: в эмулятор встроены AltirraOS для 400/800 и XL/XE, Altirra 5200 OS и Altirra BASIC, а также оригинальные Atari OS Rev. B (400/800), XL OS BB01 Rev. 2 и Atari BASIC Rev. C. По умолчанию эмулируется 130XE. Подборка охватывает разные машины линейки: компьютеры 400/800 и XL/XE и игровую приставку 5200.

Управление: **F1** — меню эмулятора; **Alt+R** — запустить программу (`.XEX`, `.ATR`); **Alt+C** — картридж (`.CAR`); **Alt+D** — дисководы; **Alt+Y** — настройки системы (тип машины, объём памяти, ОС); **F2/F3/F4** — консольные клавиши Option/Select/Start; **Pause/Break** — клавиша BREAK (остановить программу на BASIC); **F9/F10** — громкость. Джойстик эмулируется клавиатурой: стрелки (или цифровой блок, с диагоналями на 7, 9, 1, 3) — направления, Ctrl — кнопка. Геймпад NES DPAD можно подключить вместо клавиатуры.

В XL/XE по умолчанию включён встроенный BASIC; он занимает 8 КБ памяти, и часть программ без него не запускается. Перед запуском `.XEX` его стоит выключить: Alt+Y → BASIC.

| Файл | Программа | Что делать | Машина | Источник | Лицензия |
|---|---|---|---|---|---|
| `FASTBAS.ATR` | FastBasic 4.7 (Daniel Serpell) | современный BASIC с редактором прямо на Atari; на диске примеры, в том числе игры Joyas и Carrera 3D | 400/800 48 КБ и выше | [dmsc/fastbasic](https://github.com/dmsc/fastbasic), релиз v4.7 (`55dc52f`), `fastbasic-v4.7.atr` | GPL v2+ с исключением для скомпилированных программ; на диске BW-DOS (freeware) |
| `ACTION.CAR` | Action! 3.6 (Clinton Parker, OSS, 1983) | язык программирования с редактором и компилятором в картридже: набрать программу в редакторе, Shift+Ctrl+M — монитор, `C` — компилировать, `R` — запустить; Shift+Ctrl+E — обратно в редактор | 400/800, XL/XE | образ картриджа из [jhusak/atari_action_compiler](https://github.com/jhusak/atari_action_compiler) `7de960e`, `inc/action_36.c`; исходники — [pjones1063/Atari_OSS_Action](https://github.com/pjones1063/Atari_OSS_Action) `e35661c` | GPL v3 (исходники открыты автором в 2015 году) |
| `FIREFITE.XEX` | Firefighter (Bill Kendrick, 2025) | тушить пожар и выводить людей к выходу; можно вдвоём. F4 (Start) — начать; стрелки — идти; Ctrl + стрелка — струя воды в эту сторону | 48 КБ, BASIC выключен | [billkendrick/firefighter](https://github.com/billkendrick/firefighter), релиз 0.1-beta10 (`546c7a7`) | GPL v3 |
| `GEMDROP.XEX` | Gem Drop Deluxe (Bill Kendrick) | головоломка: ловить и бросать цветные камни, собирая тройки | 48 КБ | [billkendrick/gemdrop_deluxe](https://github.com/billkendrick/gemdrop_deluxe) `e6a3dc4`, собрано из исходников (cc65) | GPL v2 |
| `SCORCH.XEX` | Scorch (Pecus, pirx) | артиллерийская дуэль на 2–6 игроков. В меню — стрелки, Return — дальше; в бою ←/→ — угол ствола, ↑/↓ — сила, пробел или Ctrl — выстрел | 48 КБ | [pkali/scorch_src](https://github.com/pkali/scorch_src) `f1e2c46`, `scorch.xex` | Unlicense |
| `SCORCH52.CAR` | Scorch для приставки Atari 5200 | то же на приставке 5200: стрелки и Ctrl, как выше | Atari 5200 (тип машины 5200 в Alt+Y) | там же, `scorch.bin` с заголовком CAR (тип 4, 5200 32 КБ) | Unlicense |
| `DLI1.XEX` | пример прерываний списка отображения (Seban/Slight, 2016) | демонстрация цветового приёма: строки разного цвета на одном экране и бегущая строка | 48 КБ | [seban-slt/atari8_6502_code_examples](https://github.com/seban-slt/atari8_6502_code_examples) `da91a1a`, собрано из исходников (xasm) | общественное достояние (`README.md`) |

Образы взяты из репозиториев и релизов без изменений, кроме отмеченных: собраны из исходников `GEMDROP.XEX` и `DLI1.XEX`, к образу Scorch для 5200 добавлен 16-байтный заголовок CAR. Файлы переименованы в имена 8.3. Проверка 2026-10-04 на PCp2 (130XE, AltirraOS, PAL): FastBasic, Action!, Gem Drop, Scorch, Scorch 5200 и DLI1 запускаются; Firefighter — с выключенным BASIC (проверено на ПК).

## 9. Программы для murmc64 (`/c64`)

murmc64 (эмулятор Commodore 64) открывает файлы из каталога `/c64` по клавише **F10**. Файл `.PRG` загружается в память и запускается сам (эмулятор набирает `RUN`); образ диска `.D64` или `.D81` подключается как дисковод 8 и загружается командой `LOAD"*",8,1`. Отдельные образы ПЗУ не нужны.

Управление: клавиатура — как у C64 (Tab или левый Ctrl — CTRL, левый Alt — C=, Home — CLR/HOME); **F1–F8** — функциональные клавиши C64; **F10** — выбор файла; **F11** — RESTORE; **Ctrl+Alt+Del** — сброс C64; **F9** — переключить порт джойстика. Джойстик (по умолчанию в порту 2) эмулируется клавиатурой: стрелки — направления, правый Ctrl или правый Alt — кнопка. Геймпад NES DPAD можно подключить вместо клавиатуры.

Все программы — примеры из компилятора Си для C64 [drmortalwombat/oscar64](https://github.com/drmortalwombat/oscar64) (Dr. Mortal Wombat), лицензия GPL v3; исходники лежат в каталоге `samples` того же репозитория. Собраны из исходников коммита `3e2ffb6` с ключами из `samples/*/build.sh` самим oscar64 этого коммита; файлы переименованы в имена 8.3. Все игры управляются джойстиком в порту 2.

| Файл | Программа | Что делать | Исходник |
|---|---|---|---|
| `SNAKE.PRG` | «Змейка» | стрелки — направление; не врезаться в стены и в себя | `samples/games/snake.c` |
| `LANDER.PRG` | посадка на Луну | ↑ — двигатель, ←/→ — сдвиг в сторону; сесть мягко | `samples/games/lander.c` |
| `MAZE3D.PRG` | трёхмерный лабиринт | ↑/↓ — шаг вперёд и назад, ←/→ — поворот | `samples/games/maze3d.c` |
| `MISSILE.PRG` | ракетная оборона (по мотивам Missile Command) | стрелки — прицел, кнопка — выстрел; защитить города | `samples/games/missile.c` |
| `BREAKOUT.PRG` | «Арканоид» (Breakout) | ←/→ — ракетка, кнопка — запустить мяч | `samples/games/breakout.c` |
| `CONNECT4.PRG` | «Четыре в ряд» против компьютера | ←/→ и кнопка или клавиши 1–7 — бросить фишку в столбец | `samples/games/connectfour.c` |
| `SHMUP.PRG` | космическая стрелялка с прокруткой | стрелки — корабль, кнопка — огонь | `samples/games/hscrollshmup.c` |
| `MANDEL3D.PRG` | множество Мандельброта в объёме | смотреть, как строится картинка; любая клавиша — выход | `samples/fractals/mbmulti3d.c` |
| `FUNC3D.PRG` | трёхмерный график функции | то же | `samples/hires/func3d.c` |
| `FRACTREE.PRG` | фрактальное дерево | то же | `samples/hires/fractaltree.c` |
| `CUBE3D.PRG` | вращающийся каркасный куб | смотреть | `samples/hires/cube3d.c` |

## 10. Проверка целостности

Из корня репозитория:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Файлы `*.md` в `SHA256SUMS` не включены: git может менять в них концы строк при checkout. Файлы под `sdcard/` git хранит байт в байт (`.gitattributes`), поэтому контрольные суммы совпадают в любой ОС.

## 11. Лицензии

MOS2 и её приложения распространяются под GPLv3, исходники — murmulator-os2, тег `v.2.3.2`. Условия для FreeDOS, SeaBIOS, образа DR-DOS, игр и программ указаны в разделах 2–9. Собственные файлы репозитория (документы, `/test/*`) распространяются на условиях [`LICENSE`](LICENSE).
