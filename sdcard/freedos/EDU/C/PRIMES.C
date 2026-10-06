/* Простые числа от 2 до M с замером времени - как PRIMES.BAS (модуль 19)
   и PRIMES.PAS (модуль 20) */
#include <stdio.h>
#include <time.h>

int main(void)
{
  int m, n, d, c = 0;
  clock_t t1, t2;

  printf("Сколько простых чисел от 2 до M?\n");
  printf("M (не больше 32000): ");
  scanf("%d", &m);
  t1 = clock();                 /* тики таймера: 18,2 в секунду */
  for (n = 2; n <= m; n++) {
    d = 2;
    while (d * d <= n && n % d != 0)
      d++;
    if (d * d > n)
      c++;
  }
  t2 = clock();
  printf("Найдено простых: %d\n", c);
  printf("Секунд: %.2f\n", (t2 - t1) / CLK_TCK);
  return 0;
}
