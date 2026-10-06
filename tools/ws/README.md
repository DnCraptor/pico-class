# WonderSnake для pico-wonderswan

На карте `sdcard/WS/wondersnake.wsc` — «змейка» для WonderSwan Color Томаша Сланины (Tomasz Słanina, 2007), лицензия GPLv3. Исходники — [tslanina/Retro-WonderSwanColor-Wondersnake](https://github.com/tslanina/Retro-WonderSwanColor-Wondersnake) (проверено на `56309a6`): программа целиком на ассемблере процессора NEC V30MZ — того же семейства x86, что и 8086 в IBM PC.

Автор собирал её Borland Turbo Assembler и Turbo Link, это несвободные программы. `build.sh` собирает из тех же исходников свободным ассемблером [JWasm](https://github.com/Baron-von-Riedesel/JWasm) (Sybase Open Watcom Public License; проверено на `7f6f32e`). Изменения делаются во временной копии и только для сборки:

- директива TASM `JUMPS` закомментирована — JWasm сам удлиняет условные переходы, которые не достают до цели;
- `levels\` в путях включаемых файлов заменено на `levels/`.

Утилита `com2ws.c` из того же репозитория превращает `ws.com` в картридж на 4 Мбит.

## Сборка

```sh
WONDERSNAKE=путь/к/Retro-WonderSwanColor-Wondersnake JWASM_BIN=путь/к/jwasm ./build.sh
```

Переменная называется `JWASM_BIN`, а не `JWASM`: из переменной с таким именем JWasm читает свои параметры. Сборка повторяемая: тот же исходник даёт побайтово тот же файл.

Картридж проверен в эмуляторе Mednafen 1.29 (заставка, меню, уровень, конец игры), на pico-wonderswan — при приёмке комплекта.
