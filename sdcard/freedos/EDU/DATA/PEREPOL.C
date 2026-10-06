/* Переполнение: 32767 + 1 в 16-битном int */
#include <stdio.h>

int main(void)
{
  int a = 32767;       /* 16 бит: от -32768 до 32767 */
  long b = 32767;      /* 32 бита: до 2147483647 */
  printf("a = %d\n", a);
  a = a + 1;           /* проверки нет: число "перекручивается" */
  printf("a + 1 = %d\n", a);
  b = b + 1;
  printf("long: %ld\n", b);
  return 0;
}
