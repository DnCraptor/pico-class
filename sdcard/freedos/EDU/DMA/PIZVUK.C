/* Число пи и звук: кто отдает отсчеты - процессор или DMA? */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dos.h>
#include <conio.h>

#define CIFR  500               /* сколько цифр пи считать */
#define LEN   (CIFR * 10 / 3 + 1)
#define RATE  8000
#define DLINA 8000
#define COVOX 0x278             /* Covox Speech Thing на втором порту принтера */

long a[LEN];                    /* рабочий массив для цифр пи */
unsigned char tab[256];         /* одна волна синусоиды */
unsigned char stranica[4] = { 0x87, 0x83, 0x81, 0x82 };
unsigned char buf[2 * DLINA];
unsigned baza = 0x220;
int irq = 5, dma = 1;
volatile unsigned schet = 0;
void interrupt (*staryj)(void);
unsigned char maska;
int rezhim;
unsigned char faza = 0;         /* где мы на волне - для Covox */

/* ---------- Sound Blaster: как в SINUS.C ---------- */
void razobrat(char *s)
{
  char *p;
  if (s == NULL) return;
  if ((p = strchr(s, 'A')) != NULL) baza = (unsigned) strtol(p + 1, NULL, 16);
  if ((p = strchr(s, 'I')) != NULL) irq = atoi(p + 1);
  if ((p = strchr(s, 'D')) != NULL) dma = atoi(p + 1);
}

void dsp_pishi(unsigned char b)
{
  while (inportb(baza + 0xC) & 0x80) ;
  outportb(baza + 0xC, b);
}

int dsp_sbros(void)
{
  unsigned i;
  outportb(baza + 6, 1);
  for (i = 0; i < 100; i++) ;
  outportb(baza + 6, 0);
  for (i = 0; i < 30000 && !(inportb(baza + 0xE) & 0x80); i++) ;
  return inportb(baza + 0xA) == 0xAA;
}

void interrupt obrabotchik(void)
{
  schet++;
  inportb(baza + 0xE);
  outportb(0x20, 0x20);
}

int sb_pusk(void)               /* запустить синусоиду 440 Гц по кругу */
{
  unsigned ofs, i;
  unsigned long phys;
  unsigned char far *p = buf;
  razobrat(getenv("BLASTER"));
  if (!dsp_sbros()) return 0;
  phys = (unsigned long) FP_SEG(p) * 16 + FP_OFF(p);
  ofs = ((phys & 0xFFFF) + DLINA > 0x10000L) ? (unsigned) (0x10000L - (phys & 0xFFFF)) : 0;
  phys += ofs;
  for (i = 0; i < DLINA; i++)
    buf[ofs + i] = tab[(unsigned char) ((unsigned long) i * 440 * 256 / RATE)];
  staryj = getvect(8 + irq);
  setvect(8 + irq, obrabotchik);
  maska = inportb(0x21);
  outportb(0x21, maska & ~(1 << irq));
  outportb(0x0A, 4 + dma); outportb(0x0C, 0); outportb(0x0B, 0x58 + dma);
  outportb(dma * 2, phys & 0xFF); outportb(dma * 2, (phys >> 8) & 0xFF);
  outportb(stranica[dma], phys >> 16);
  outportb(dma * 2 + 1, (DLINA - 1) & 0xFF); outportb(dma * 2 + 1, (DLINA - 1) >> 8);
  outportb(0x0A, dma);
  dsp_pishi(0xD1);
  dsp_pishi(0x40); dsp_pishi(256 - 1000000L / RATE);
  dsp_pishi(0x48); dsp_pishi((DLINA - 1) & 0xFF); dsp_pishi((DLINA - 1) >> 8);
  dsp_pishi(0x1C);
  return 1;
}

void sb_stop(void)
{
  dsp_pishi(0xD0); dsp_pishi(0xD3);
  outportb(0x0A, 4 + dma);
  outportb(0x21, maska);
  setvect(8 + irq, staryj);
  dsp_sbros();
}

/* ---------- число пи: алгоритм "краника" (spigot) ---------- */
void cifra(int c)               /* напечатать цифру через DOS */
{
  putchar('0' + c);
  fflush(stdout);
}

void schitat_pi(void)
{
  int i, j, nines = 0, predigit = 0;
  long x, q;
  for (i = 0; i < LEN; i++) a[i] = 2;
  for (j = 1; j <= CIFR; j++) {
    q = 0;
    for (i = LEN; i > 0; i--) {
      x = 10L * a[i - 1] + q * i;
      a[i - 1] = x % (2L * i - 1);
      q = x / (2L * i - 1);
      if (rezhim == 1) {        /* без DMA: процессор сам отдает каждый отсчет */
        outportb(COVOX, tab[faza]);
        faza += 14;             /* шаг по волне: чем быстрее считаем, тем выше звук */
      }
    }
    a[0] = q % 10; q /= 10;
    if (q == 9) nines++;
    else if (q == 10) {
      cifra(predigit + 1);
      for (; nines > 0; nines--) cifra(0);
      predigit = 0;
    } else {
      if (j > 1) cifra(predigit);
      predigit = (int) q;
      for (; nines > 0; nines--) cifra(9);
    }
  }
  cifra(predigit);
}

int main(void)
{
  int i;
  unsigned t1, t2;
  for (i = 0; i < 256; i++)
    tab[i] = (unsigned char) (128 + 120 * sin(i * 2 * M_PI / 256));
  printf("Число пи, %d цифр.\n", CIFR);
  printf("0 - без звука, 1 - звук через Covox (каждый отсчет отдает процессор),\n");
  printf("2 - звук через Sound Blaster (DMA и IRQ): ");
  scanf("%d", &rezhim);
  if (rezhim == 2 && !sb_pusk()) { printf("Sound Blaster не отвечает\n"); return 1; }
  t1 = peek(0x0040, 0x006C);    /* тики таймера BIOS: 18,2 в секунду */
  schitat_pi();
  t2 = peek(0x0040, 0x006C);
  if (rezhim == 2) sb_stop();
  if (rezhim == 1) outportb(COVOX, 128);
  printf("\nВремя: %.1f с", (t2 - t1) / 18.2);
  if (rezhim == 2) printf(", блоков звука отыграно: %u", schet);
  printf("\n");
  return 0;
}
