/* Рисунок из функций - как RISUNOK.PAS (модуль 20) */
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

/* квадрат: левый верхний угол (x, y), сторона a, цвет c */
void kvadrat(int x, int y, int a, int c)
{
  setcolor(c);
  rectangle(x, y, x + a, y + a);
}

/* дом: левый нижний угол (x, y), ширина w */
void dom(int x, int y, int w)
{
  setfillstyle(SOLID_FILL, BROWN);
  bar(x, y - w, x + w, y);                          /* стены */
  setcolor(RED);
  line(x, y - w, x + w / 2, y - w - w / 2);         /* крыша */
  line(x + w / 2, y - w - w / 2, x + w, y - w);
  setfillstyle(SOLID_FILL, LIGHTCYAN);
  bar(x + w / 3, y - 2 * w / 3, x + 2 * w / 3, y - w / 3);  /* окно */
}

void solnce(int x, int y, int r)
{
  setcolor(YELLOW);
  setfillstyle(SOLID_FILL, YELLOW);
  fillellipse(x, y, r, r);
}

int main(void)
{
  int gd = EGA, gm = EGAHI, i;   /* 640 x 350, 16 цветов: есть у EGA и VGA */

  initgraph(&gd, &gm, "\\FREEDOS\\EDU\\TC201");
  if (graphresult() != grOk) {
    printf("Графика не включилась: нужен видеоадаптер EGA или VGA\n");
    return 1;
  }
  for (i = 1; i <= 8; i++)
    kvadrat(20 + i * 6, 20 + i * 6, 120 - i * 12, i);
  dom(250, 300, 160);
  dom(450, 300, 100);
  solnce(560, 70, 40);
  outtextxy(20, getmaxy() - 20, "Press any key");
  getch();
  closegraph();
  return 0;
}
