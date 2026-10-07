/* Ассемблер прямо в программе на Си: asm (собирать только tcc!) */
#include <stdio.h>
#include <dos.h>

int main(void)
{
  int a = 30, b = 12, s;

  asm mov ax, a          /* переменные Си видны прямо по имени */
  asm add ax, b
  asm mov s, ax
  printf("asm: %d + %d = %d\n", a, b, s);

  printf("asm: ");
  asm mov ah, 2          /* служба DOS 02h - вывести букву */
  asm mov dl, 'A'
  asm int 21h
  printf("\n");

  printf("без ассемблера: ");
  _AH = 2;               /* псевдорегистры Turbo C */
  _DL = 'B';
  geninterrupt(0x21);
  printf("\n");
  return 0;
}
