/* Память: читаем байты по адресу сегмент:смещение */
#include <stdio.h>
#include <dos.h>

int main(void)
{
  int i;
  printf("Дата BIOS (F000:FFF5): ");
  for (i = 0; i < 8; i++) putchar(peekb(0xF000, 0xFFF5 + i));
  printf("\n");
  printf("ОЗУ (0040:0013): %u КБ\n", peek(0x0040, 0x0013));
  printf("Порт COM1 (0040:0000): %04Xh\n", peek(0x0040, 0x0000));
  printf("Порт LPT1 (0040:0008): %04Xh\n", peek(0x0040, 0x0008));
  if (peekb(0x0040, 0x0017) & 0x40)
    printf("Caps Lock включен\n");
  else
    printf("Caps Lock выключен\n");
  pokeb(0xB800, 0, 'A');                 /* буква */
  pokeb(0xB800, 1, 0x1E);                /* цвет: желтый на синем */
  printf("В левом верхнем углу - буква A, записанная прямо в видеопамять\n");
  return 0;
}
