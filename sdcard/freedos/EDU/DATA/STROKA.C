/* Строка в Си: массив символов, в конце - байт 0 */
#include <stdio.h>
#include <string.h>

/* две строки лежат в памяти одна за другой */
char a[8] = "Привет";        /* 6 букв + 0 в конце: 7 байт из 8 */
char b[8] = "мир";

int main(void)
{
  printf("%s - длина %d, a[6] = %d\n", a, strlen(a), a[6]);
  printf("b = %s\n", b);
  strcpy(a, "Очень длинная строка");  /* 20 букв в 8 байт! */
  printf("a = %s\n", a);
  printf("b = %s  <- b испортилась\n", b);
  return 0;
}
