/* Порты: часы и динамик без BIOS и DOS */
#include <stdio.h>
#include <dos.h>

unsigned char chasy(unsigned char reg)   /* прочитать регистр микросхемы часов */
{
  outportb(0x70, reg);                   /* какой регистр нужен */
  return inportb(0x71);                  /* его значение */
}

int main(void)
{
  unsigned t;
  printf("Часы (порты 70h, 71h): %02X:%02X:%02X\n", chasy(4), chasy(2), chasy(0));
  /* динамик: таймер (порты 42h, 43h) и порт 61h */
  outportb(0x43, 0xB6);                  /* канал 2 таймера, режим "меандр" */
  outportb(0x42, 0xA9);                  /* 1193182 / 1000 = 1193 = 0x04A9: */
  outportb(0x42, 0x04);                  /* сначала младший байт, потом старший */
  outportb(0x61, inportb(0x61) | 3);     /* включить динамик */
  t = peek(0x0040, 0x006C);              /* счетчик тиков таймера: 18,2 в секунду */
  while (peek(0x0040, 0x006C) - t < 6)   /* ждем 6 тиков - треть секунды */
    ;
  outportb(0x61, inportb(0x61) & 0xFC);  /* выключить */
  printf("Звук 1000 Гц - через порты\n");
  return 0;
}
