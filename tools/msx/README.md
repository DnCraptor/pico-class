# Исходники кассет для frank-msx (MSX)

Программы на MSX BASIC с кассет `sdcard/MSX/*.CAS` в виде текста. Написаны для этого репозитория (учебный модуль 10) и распространяются на условиях [`LICENSE`](../../LICENSE). Все работают на MSX первого поколения (MSX BASIC 1.0).

| Файл | Кассета | Что показывает |
|---|---|---|
| `HELLO.bas` | [`HELLO.CAS`](../../sdcard/MSX/HELLO.CAS) | `INPUT`, цикл `FOR…NEXT`, `COLOR` |
| `MUSIC.bas` | [`MUSIC.CAS`](../../sdcard/MSX/MUSIC.CAS) | трёхголосная музыка `PLAY`, которая играет сама, пока программа идёт дальше (`PLAY(0)`); ноты с клавиатуры (`INKEY$`) |
| `DRAW.bas` | [`DRAW.CAS`](../../sdcard/MSX/DRAW.CAS) | графика SCREEN 2: `LINE`, `CIRCLE`, `PAINT`, язык перемещений `DRAW` с переменными (`"R=L;"`), текст на графическом экране (`OPEN "GRP:"`) |
| `SPRITE.bas` | [`SPRITE.CAS`](../../sdcard/MSX/SPRITE.CAS) | аппаратные спрайты: `SPRITE$`, `PUT SPRITE`, стрелки (`STICK`), пробел (`STRIG`), столкновение спрайтов (`ON SPRITE GOSUB`) |
| `GUESS.bas` | [`GUESS.CAS`](../../sdcard/MSX/GUESS.CAS) | игра «Угадай число» |

Каждая кассета — один файл в текстовом (ASCII) формате, как после `SAVE"CAS:ИМЯ",A` на настоящем MSX. Загрузка: **F11** → Cassette tape → Insert tape (или Change tape) → файл, затем `RUN"CAS:"`.

Сборка кассет (Python 3), в этом каталоге:

```sh
python3 mkcas.py ../../sdcard/MSX *.bas
```

`mkcas.py` записывает блок заголовка (10 байт `EA` и имя из шести знаков) и текст программы блоками по 256 байт; концы строк — CRLF, конец файла — байты `1A`. Кассеты проверены в openMSX (MSX1 с ПЗУ `sdcard/MSX/MSX.ROM`): загрузка `RUN"CAS:"` и работа каждой программы.
