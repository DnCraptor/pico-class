# Общее содержимое SD-карты

Состояние на 2026-10-03. Каталог `sdcard/` в корне репозитория — общая для всех плат часть учебной SD-карты: MOS2 и её приложения, FreeDOS для murm386, игры и учебные программы, файлы для приёмки рабочего места. Здесь описано, что в нём лежит, откуда взято и как проверить целостность. При изменении состава обновляются этот файл и [`SHA256SUMS`](SHA256SUMS).

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
| `/ZX/*` | свободные игры для pico-speccy, см. раздел 5 | см. раздел 5 |
| `/apple/*.DSK` | образы дисков для murmapple (Apple IIe), см. раздел 6 | см. раздел 6 |
| `/z26/*.bin` | свободные homebrew-игры для pico-z26 (Atari 2600), см. раздел 7 | см. раздел 7 |
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

Из примеров MK1_NES не включены Che-Man (пародия с политическим персонажем) и Sgt. Helmet: Training Day (военный шутер). Решение о включении остальных игр руководитель принимает после просмотра с учётом возраста группы.

## 4. Программы для murm386 (`/freedos/GAMES`, `/freedos/EDU`)

Подборка рассчитана на производительность murm386 без PSRAM: примерно 2–3 PC XT, то есть уровень Turbo XT или PC AT 8 МГц. Без PSRAM быстрее не получится из-за частого свопа. Уменьшить число промахов кэша помогают режимы EGA128 и VGA128 (дополнительно 128 КБ SRAM) и MCGA (ещё 64 КБ). Программы, которым нужна PSRAM, в комплект не входят — их можно добавить отдельно. На рабочих местах с PSRAM можно запускать сборки murm386 из `/emu/psram` (см. `hardware/olimex-pico-pc/MANIFEST.md`): гостевая память у них целиком в PSRAM, своп не нужен.

Отбор: скорость уровня XT/AT; видео текстовое, CGA, EGA или VGA/MCGA. murm386 поддерживает XMS, поэтому программы, использующие расширенную память, тоже подходят — ограничение задаёт скорость, а не объём памяти. Источник и условия распространения каждой программы указаны в таблице; условно-бесплатные (shareware) не включены. Запуск — из Volkov Commander: зайти в каталог, Enter на `.EXE`/`.COM`.

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

## 5. Игры для pico-speccy (`/ZX`)

pico-speccy открывает файлы из любого каталога SD-карты через файловый браузер (F5) и понимает почти все форматы Спектрума: `.tap`, `.tzx`, `.z80`, `.sna`, `.trd`, `.scl` и др. Подборка лежит в `/ZX`.

Все игры — The Mojon Twins. Свою работу (игры, графику, музыку, исходники) авторы распространяют по лицензии [CC BY-NC-SA 3.0](https://creativecommons.org/licenses/by-nc-sa/3.0/): свободное некоммерческое распространение с указанием авторства. Рядом с каждой игрой лежит файл `.TXT` с описанием, авторами и текстом лицензии. Источник файлов — [retrobrews/zxspectrum-games](https://github.com/retrobrews/zxspectrum-games), коммит `b3cb92e`; файлы переименованы в короткие имена, содержимое не изменено.

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

Не включены игры Mojon Twins с отсылками к чужим персонажам (El Hobbit, Goku Mal, Horace Goes to the Tower) и с сюжетами, неуместными для детской аудитории (Ninjajar!, Judge Morry vs. Baldo Midget). Во вступлении к Sir Ababol — шуточная история про крестоносца; решение о включении остальных игр руководитель принимает после просмотра с учётом возраста группы. Игры не проверялись на рабочем месте.

## 6. Образы дисков для Apple IIe (`/apple`)

murmapple ищет образы дисков в каталоге `/apple` (форматы `.dsk`, `.nib`, `.woz`, `.bdsk`); диск выбирается в экранном меню эмулятора. Подборка рассчитана в первую очередь на проверку картинки и звука и на знакомство с Apple II; все образы — DOS 3.3, 140 КБ.

| Файл | Что это | Что проверяет | Автор, источник | Лицензия |
|---|---|---|---|---|
| `SHORTPRG.DSK` | Short Programs — сборник коротких программ на Applesoft BASIC в одну-две строки: графика, анимация, звук | графика HGR/LORES, BASIC; хороший материал для модуля программирования — программы короткие и их можно разбирать | Lee Fastenau, [thelbane/Apple-II-Programs](https://github.com/thelbane/Apple-II-Programs) `3bc2568` | MIT |
| `FIRE.DSK` | Fire — небольшие эффекты «огня» | графика LORES | Vince Weaver, [deater/dos33fsprogs](https://github.com/deater/dos33fsprogs) `3a02734` | GPL v2 |
| `TB6502.DSK` | Tom Bombem — аркадная «стрелялка» | игровой процесс, звук | там же | GPL v2 |
| `TFV.DSK` | Talbot Fantasy 7 — шуточная ролевая игра в режиме LORES | игровой процесс, клавиатура | там же | GPL v2 |

Образы взяты из репозиториев без изменений, переименованы в короткие имена. Из dos33fsprogs не включены игры, основанные на чужих сюжетах и персонажах (Myst, Riven, Commander Keen, Duke, Monkey Island, Peasant's Quest и др.), из Apple-II-Programs — диск `fun-stuff.dsk` с рисунком чужого логотипа. Проверка 2026-10-02 на PCp2: образы подборки работают в обеих сборках murmapple. Демо Fireworks, Xmas 2018 и Xmas 2019 из того же репозитория после загрузки выводили повторяющиеся текстовые символы и зависали, поэтому из подборки исключены; проверки Mockingboard в комплекте пока нет.

Как запустить диск: F11 — экранное меню дисков, выбрать образ, затем Boot. Если программа не стартовала сама, в Applesoft BASIC: `CATALOG` — список файлов на диске, `RUN имя` — запустить BASIC-программу (тип `A`), `BRUN имя` — машинный код (тип `B`); `PR#6` — перезагрузка с дисковода.

## 7. Игры для pico-z26 (`/z26`)

pico-z26 ищет игры в каталоге `/z26` на SD-карте (расширения `.bin`, `.a26`, `.rom`). В подборку входят только свободные homebrew-игры с открытой лицензией; коммерческие образы картриджей Atari 2600 в комплект не входят. Управление — см. строку pico-z26 в [`hardware/COMPATIBILITY.md`](hardware/COMPATIBILITY.md): стрелки — джойстик, Z — кнопка, Enter — RESET (старт), Esc — SELECT.

| Файл | Игра | Что делать | Источник | Лицензия | Размер, схема банков |
|---|---|---|---|---|---|
| `pipes.bin` | Pipes 2600 (albf, 2014) | головоломка: проложить трубы от старта до финиша, пока не потекла вода (в духе Pipe Dream) | [albf/pipes-2600](https://github.com/albf/pipes-2600) `ea0e196`, `pipe2600.bin` | MIT | 8 КБ, F8 + SuperChip |
| `2048.bin` | 2048 2600 (Carlos Duarte do Nascimento, 2014) | головоломка «2048»: сдвигать плитки джойстиком и складывать одинаковые числа; старт — кнопкой | [chesterbr/2048-2600](https://github.com/chesterbr/2048-2600) `35de3c1` | MIT | 2 КБ |
| `berta.bin` | Berta and Butterflies (vandalton, 2024) | слонёнок Берта ловит бабочек с четырёх сторон — по мотивам «Ну, погоди!» / Game & Watch «Egg» | [vandalton/BertaAndButterflies](https://github.com/vandalton/BertaAndButterflies), релиз v1.00 (`7f81d40`), `berta-and-butterflies.v1.00.ntsc.en.bin` | MIT | 4 КБ |
| `mssnake.bin` | Ms Snake! (D. Olmisani, L. Olmisani, M. Segnalini, 2017) | «Змейка» с яркой графикой и музыкой | [mad4j/atari-mssnake](https://github.com/mad4j/atari-mssnake) `8d495ba`, `game/ntsc-pal60/ms-snake!-NTSC-PAL60.bas.bin` | GPLv3 | 32 КБ, F4 + SuperChip |

Образы взяты из репозиториев без изменений (у Berta — из релиза на GitHub), переименованы в короткие имена. Исходники — в тех же репозиториях. Проверка 2026-10-03 на PCp2 с pico-z26 4.0.8 (`b794190`): `2048.bin` и `berta.bin` работают. `pipes.bin` и `mssnake.bin` (8 и 32 КБ с переключением банков) дают чёрный экран: эмулятор переключает банк при любом чтении верхних адресов картриджа, включая вектор сброса; исправление предложено в pico-z26.

Не включены: Snake (careyes17, 2021) — в pico-z26 змейка не реагирует на джойстик, причина не найдена; Plane (gonzalorf, 2022) — в игре нет звука, управление на рабочем месте оказалось непонятным; Hellway (свободная, но с «Hell» в названии — руководитель может добавить сам, [opbokel/hellway](https://github.com/opbokel/hellway)), незаконченные проекты, игры без лицензии и игры по чужим персонажам.

## 8. Проверка целостности

Из корня репозитория:

```sh
sha256sum -c SHA256SUMS          # Linux, Git Bash в Windows
shasum -a 256 -c SHA256SUMS      # macOS
```

Файлы `*.md` в `SHA256SUMS` не включены: git может менять в них концы строк при checkout. Файлы под `sdcard/` git хранит байт в байт (`.gitattributes`), поэтому контрольные суммы совпадают в любой ОС.

## 9. Лицензии

MOS2 и её приложения распространяются под GPLv3, исходники — murmulator-os2, тег `v.2.3.2`. Условия для FreeDOS, SeaBIOS, образа DR-DOS, игр и программ указаны в разделах 2–7. Собственные файлы репозитория (документы, `/test/*`) распространяются на условиях [`LICENSE`](LICENSE).
