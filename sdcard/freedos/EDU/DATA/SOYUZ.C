/* Объединение (union): одна и та же память под разные типы */
#include <stdio.h>

union chislo
{
  int i;                     /* целое: 2 байта */
  unsigned char bajt[2];     /* те же 2 байта по одному */
  long l;                    /* длинное: 4 байта */
};

int main(void)
{
  union chislo c;
  printf("размер union: %d байта (как самый большой тип)\n", sizeof(c));
  c.l = 0;
  c.i = 258;                 /* 258 = 1 * 256 + 2 */
  printf("i = %d, bajt[0] = %d, bajt[1] = %d\n", c.i, c.bajt[0], c.bajt[1]);
  c.bajt[1] = 3;             /* меняем старший байт */
  printf("i = %d\n", c.i);   /* 3 * 256 + 2 = 770 */
  return 0;
}
