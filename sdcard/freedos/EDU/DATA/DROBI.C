/* Дроби: десять раз по 0.1 - это ровно 1? */
#include <stdio.h>
#include <math.h>

int main(void)
{
  double s = 0;
  int i;
  for (i = 1; i <= 10; i++)
    s = s + 0.1;
  printf("s = %.10f\n", s);
  if (s == 1)
    printf("s равно 1\n");
  else
    printf("s НЕ равно 1\n");
  printf("s - 1 = %g\n", s - 1);
  /* сравнивать дроби надо "с допуском" */
  if (fabs(s - 1) < 0.000001)
    printf("с допуском: равно\n");
  return 0;
}
