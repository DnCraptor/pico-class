/* Три способа: через DOS, через BIOS и прямо в оборудование */
#include <stdio.h>
#include <dos.h>

void dos_stroka(char *s)          /* DOS, прерывание 21h, функция 09h */
{
  union REGS r;                   /* регистры процессора - это union! */
  r.h.ah = 0x09;
  r.x.dx = (unsigned) s;          /* строка кончается знаком $ */
  intdos(&r, &r);
}

void bios_stroka(char *s)         /* BIOS, прерывание 10h, функция 0Eh */
{
  union REGS r;
  while (*s) {
    r.h.ah = 0x0E;
    r.h.al = *s++;
    r.h.bh = 0;
    int86(0x10, &r, &r);
  }
}

void ekran_stroka(char *s)        /* прямо в видеопамять B800h */
{
  int x = 0;
  int y = peekb(0x0040, 0x0051);  /* в какой строке курсор - помнит BIOS */
  while (*s) {
    pokeb(0xB800, (y * 80 + x) * 2, *s++);
    pokeb(0xB800, (y * 80 + x) * 2 + 1, 0x1E);   /* желтый на синем */
    x++;
  }
}

unsigned char chasy(unsigned char reg)
{
  outportb(0x70, reg);
  return inportb(0x71);
}

int main(void)
{
  union REGS r;

  dos_stroka("1. Это напечатала DOS\r\n$");
  bios_stroka("2. Это напечатал BIOS\r\n");
  ekran_stroka("3. Это записано прямо в видеопамять");
  bios_stroka("\r\n");             /* курсор на следующую строку */

  r.h.ah = 0x2C;                  /* DOS: который час? */
  intdos(&r, &r);
  printf("Время от DOS  (21h, 2Ch): %02d:%02d:%02d\n", r.h.ch, r.h.cl, r.h.dh);
  r.h.ah = 0x02;                  /* BIOS: который час? */
  int86(0x1A, &r, &r);
  printf("Время от BIOS (1Ah, 02h): %02X:%02X:%02X\n", r.h.ch, r.h.cl, r.h.dh);
  printf("Время из часов (70h/71h): %02X:%02X:%02X\n", chasy(4), chasy(2), chasy(0));
  return 0;
}
