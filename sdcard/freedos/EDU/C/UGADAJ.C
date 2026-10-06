/* Угадай число: та же игра, что на Бейсике (модуль 19) и Паскале (модуль 20) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
  int x, g = 0, n = 0, r, ch;

  randomize();                  /* перемешать случайные числа по часам */
  x = random(100) + 1;          /* random(100) - от 0 до 99 */
  printf("Угадай число от 1 до 100\n");
  do {
    printf("Твой ответ: ");
    r = scanf("%d", &g);
    if (r == EOF)               /* ввод закончился (Ctrl+Z) */
      return 0;
    if (r != 1) {               /* ввели не число */
      do
        ch = getchar();         /* выбросить строку целиком */
      while (ch != '\n' && ch != EOF);
      printf("Это не число\n");
      continue;
    }
    n++;
    if (g < x)
      printf("Больше\n");
    else if (g > x)
      printf("Меньше\n");
  } while (g != x);
  printf("Угадал! Ходов: %d\n", n);
  return 0;
}
