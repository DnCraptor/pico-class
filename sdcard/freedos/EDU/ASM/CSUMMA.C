/* Функции на ассемблере в программе на Си: собирать tcc csumma.c summa.obj */
#include <stdio.h>

int pascal summa(int a, int b);   /* SUMMA: соглашение Паскаля */
int summa_c(int a, int b);        /* _summa_c: соглашение Си */

int main(void)
{
  printf("Си: summa(30, 12) = %d, summa_c(30, 12) = %d\n",
         summa(30, 12), summa_c(30, 12));
  return 0;
}
