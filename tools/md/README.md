# Исходники программ для pico-genesisPlus (Sega Mega Drive / Genesis)

Программы на C с карты `sdcard/MD/*.bin`. Написаны для этого репозитория и распространяются на условиях [`LICENSE`](../../LICENSE). Это те же три программы, что для PC Engine в [`tools/pce`](../pce/README.md), с теми же спрайтами: на двух приставках одного поколения одну и ту же задачу можно решить и сравнить. Картриджи по 128 КБ.

| Файл | Картридж | Что показывает |
|---|---|---|
| `hello.c` | [`hello.bin`](../../sdcard/MD/hello.bin) | текст на экране (`VDP_drawText`), цвета палитры (`PAL_setColor`), часы на счётчике кадров (`SYS_doVBlankProcess`), нажатые кнопки геймпада (`JOY_readJoypad`), звук |
| `sprite.c` | [`sprite.bin`](../../sdcard/MD/sprite.bin) | аппаратные спрайты (`VDP_setSpriteFull`): робот на геймпаде, мяч летает сам и отскакивает от краёв, касание считается; кнопка A меняет палитру робота |
| `catch.c` | [`catch.bin`](../../sdcard/MD/catch.bin) | законченная игра «Лови звёзды»: корзина внизу, три звезды падают всё быстрее, три промаха — конец игры, лучший счёт запоминается |
| `beep.h` | — | звук: тон на канале 0 чипа PSG (SN76489, тот же, что в Master System); музыку игр Mega Drive играет второй чип — FM-синтезатор YM2612 |
| `mksprites.py` | — | спрайты нарисованы в нём текстом (цифра — номер цвета палитры) и превращаются в тайлы 8×8 `sprite_gfx.h`, `catch_gfx.h` |

Чем отличаются от версий для PC Engine: экран шире (320 точек вместо 256), спрайт 16×16 собирается из четырёх тайлов 8×8, а не хранится одним куском; у геймпада три кнопки A, B, C и START (у PC Engine — I, II, SELECT, RUN). Клавиши pico-genesisPlus — в [`../../SDCARD.md`](../../SDCARD.md), раздел 15.

## Сборка

Библиотека — [SGDK](https://github.com/Stephane-D/SGDK) (MIT, проверено на `ee6870a`, 2026-10-01), компилятор — gcc для 68000: `m68k-elf-gcc` или пакет `gcc-m68k-linux-gnu` из Ubuntu/Debian; нужна ещё Java (утилиты SGDK — jar-файлы). SGDK сначала собирается под Linux из своего корня:

```sh
PREFIX=m68k-linux-gnu- cmake -B build . && make -C build
```

Это собирает `lib/libmd.a` и утилиты `bin/bintos`, `bin/sjasm`, `bin/xgmtool`. Затем из этого каталога:

```sh
GDK=/путь/к/SGDK PREFIX=m68k-linux-gnu- ./build.sh
```

`build.sh` запускает `mksprites.py`, собирает каждую программу как проект SGDK (`makefile.gen`, цель `release`) и кладёт картриджи в `../../sdcard/MD`. Сборка повторяемая: те же исходники дают побайтово те же файлы. С `m68k-linux-gnu-gcc` компоновщику нужен ключ `--build-id=none`, `build.sh` его добавляет. Расширение у картриджей `.bin`, а не привычное `.md`: файлы `*.md` в этом репозитории — документы, git считает их текстом и меняет в них концы строк (`.gitattributes`). pico-genesisPlus показывает оба расширения.

Картриджи проверены на ядре gwenesis из [DnCraptor/pico-genesisPlus](https://github.com/DnCraptor/pico-genesisPlus) (`1a7d23c`), собранном для ПК (`hosttest`): экран, отклик на кнопки, звук, счёт в `sprite.bin` и `catch.bin` (для проверки корзину и робота водила программа).
