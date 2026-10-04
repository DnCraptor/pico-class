# Исходники диска `CLASS.SSD` для frank-micro (BBC Micro)

Программы на BBC BASIC с диска [`sdcard/micro/disk/CLASS.SSD`](../../sdcard/micro/disk/CLASS.SSD) в виде текста. Написаны для этого репозитория и распространяются на условиях [`LICENSE`](../../LICENSE).

| Файл | Программа на диске | Что показывает |
|---|---|---|
| `MENU.bas` | `MENU` | меню выбора программ; запускается при загрузке диска (`!BOOT` — `boot.txt`) |
| `HELLO.bas` | `HELLO` | процедуры (`DEF PROC`), цикл `REPEAT…UNTIL`, `INPUT` |
| `TELETXT.bas` | `TELETXT` | цвета, двойная высота и мигание в режиме телетекста MODE 7 |
| `SHAPES.bas` | `SHAPES` | закрашенные треугольники в MODE 1 (`PLOT 85`) |
| `SPIRAL.bas` | `SPIRAL` | спираль из цветных линий в MODE 2 (`DRAW`, `COS`, `SIN`) |
| `MUSIC.bas` | `MUSIC` | мелодия командой `SOUND`, ноты в строках `DATA` |
| `GUESS.bas` | `GUESS` | игра «Угадай число» |

Сборка образа диска ассемблером [beebasm](https://github.com/stardot/beebasm) (GPL v3), в этом каталоге:

```sh
beebasm -i class.6502 -do CLASS.SSD -opt 3 -title PICOCLASS
```

`PUTBASIC` в `class.6502` переводит текст программ в токенизированный формат BBC BASIC, `-opt 3` включает автозапуск `!BOOT` (SHIFT+BREAK или загрузка через F11).
