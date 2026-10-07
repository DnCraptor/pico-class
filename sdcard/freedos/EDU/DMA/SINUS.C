/* Синусоида через DMA: Sound Blaster играет блок памяти сам */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dos.h>
#include <conio.h>

#define RATE  8000              /* отсчетов в секунду */
#define DLINA 8000              /* блок - ровно 1 секунда */

unsigned char stranica[4] = { 0x87, 0x83, 0x81, 0x82 };  /* страничные регистры DMA */
unsigned char buf[2 * DLINA];   /* вдвое больше: внутри ищем окно для DMA */
unsigned baza = 0x220;          /* порт Sound Blaster */
int irq = 5, dma = 1;           /* номер IRQ и канал DMA */
volatile unsigned schet = 0;    /* сколько блоков отыграно - считает обработчик */
void interrupt (*staryj)(void); /* прежний обработчик прерывания */

void razobrat(char *s)          /* BLASTER=A220 I5 D1 ... */
{
  char *p;
  if (s == NULL) return;
  if ((p = strchr(s, 'A')) != NULL) baza = (unsigned) strtol(p + 1, NULL, 16);
  if ((p = strchr(s, 'I')) != NULL) irq = atoi(p + 1);
  if ((p = strchr(s, 'D')) != NULL) dma = atoi(p + 1);
}

void dsp_pishi(unsigned char b) /* подождать, пока DSP готов, и записать */
{
  while (inportb(baza + 0xC) & 0x80)
    ;
  outportb(baza + 0xC, b);
}

int dsp_sbros(void)
{
  unsigned i;
  outportb(baza + 6, 1);
  for (i = 0; i < 100; i++) ;   /* небольшая пауза */
  outportb(baza + 6, 0);
  for (i = 0; i < 30000 && !(inportb(baza + 0xE) & 0x80); i++) ;
  return inportb(baza + 0xA) == 0xAA;
}

void interrupt obrabotchik(void) /* вызывается по IRQ: блок отыгран */
{
  schet++;
  inportb(baza + 0xE);          /* сказать карте: прерывание принято */
  outportb(0x20, 0x20);         /* сказать контроллеру прерываний: обработка окончена */
}

int main(void)
{
  char *s = getenv("BLASTER");
  unsigned char maska;
  unsigned ofs, i, ostalos;
  unsigned long phys;
  float f;
  unsigned char far *p = buf;

  razobrat(s);
  printf("BLASTER=%s  ->  порт %Xh, IRQ %d, DMA %d\n", s ? s : "(нет)", baza, irq, dma);
  if (!dsp_sbros()) { printf("Sound Blaster не отвечает\n"); return 1; }
  printf("Частота, Гц (например, 440): ");
  scanf("%f", &f);

  /* окно в buf, которое не пересекает границу 64 КБ физической памяти */
  phys = (unsigned long) FP_SEG(p) * 16 + FP_OFF(p);
  if ((phys & 0xFFFF) + DLINA > 0x10000L) ofs = (unsigned) (0x10000L - (phys & 0xFFFF));
  else ofs = 0;
  phys += ofs;
  for (i = 0; i < DLINA; i++)
    buf[ofs + i] = (unsigned char) (128 + 120 * sin(2 * M_PI * f * i / RATE));

  /* свой обработчик IRQ */
  staryj = getvect(8 + irq);
  setvect(8 + irq, obrabotchik);
  maska = inportb(0x21);
  outportb(0x21, maska & ~(1 << irq));         /* разрешить этот IRQ */

  /* контроллер DMA: из памяти в устройство, с автоповтором */
  outportb(0x0A, 4 + dma);                     /* закрыть канал */
  outportb(0x0C, 0);                           /* сброс: дальше сначала младший байт */
  outportb(0x0B, 0x58 + dma);                  /* режим: чтение памяти, автоповтор */
  outportb(dma * 2, phys & 0xFF);              /* адрес внутри страницы 64 КБ */
  outportb(dma * 2, (phys >> 8) & 0xFF);
  outportb(stranica[dma], phys >> 16);         /* номер страницы 64 КБ */
  outportb(dma * 2 + 1, (DLINA - 1) & 0xFF);   /* длина - 1 */
  outportb(dma * 2 + 1, (DLINA - 1) >> 8);
  outportb(0x0A, dma);                         /* открыть канал */

  /* Sound Blaster: частота, размер блока, пуск с автоповтором */
  dsp_pishi(0xD1);                             /* включить выход */
  dsp_pishi(0x40); dsp_pishi(256 - 1000000L / RATE);
  dsp_pishi(0x48); dsp_pishi((DLINA - 1) & 0xFF); dsp_pishi((DLINA - 1) >> 8);
  dsp_pishi(0x1C);

  printf("Играет. Любая клавиша - стоп.\n");
  while (!kbhit()) {
    outportb(0x0C, 0);                         /* сколько осталось в блоке - спросим DMA */
    ostalos = inportb(dma * 2 + 1);
    ostalos += inportb(dma * 2 + 1) << 8;
    printf("\rБлоков отыграно: %u   DMA: осталось %5u байт ", schet, ostalos);
  }
  if (getch() == 0) getch();
  printf("\n");

  dsp_pishi(0xD0);                             /* остановить DMA карты */
  dsp_pishi(0xD3);                             /* выключить выход */
  outportb(0x0A, 4 + dma);                     /* закрыть канал DMA */
  outportb(0x21, maska);                       /* вернуть маску IRQ */
  setvect(8 + irq, staryj);                    /* вернуть прежний обработчик! */
  dsp_sbros();
  return 0;
}
