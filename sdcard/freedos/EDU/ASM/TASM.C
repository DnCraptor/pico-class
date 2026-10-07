/* TASM.C - переходник: Turbo C вызывает TASM, а мы запускаем JWasm.
   Turbo C пишет, например:  tasm prog/D__SMALL__ /D__CDECL__ /e/ml,prog ;
   а JWasm нужно:            jwasmr -D__SMALL__ -D__CDECL__ -Cp -Foprog.obj prog.asm */
#include <stdio.h>
#include <string.h>
#include <process.h>

char vse[256];                  /* вся командная строка */
char istochnik[80], obekt[80];  /* имена файлов */
char opcii[10][40];             /* ключи для JWasm */
char fo[90];
int nopc = 0;

void slovo(char *s, int dlina, int pole, int kluch)
{
  char w[80];
  if (dlina <= 0 || dlina >= 80) return;
  strncpy(w, s, dlina); w[dlina] = 0;
  if (kluch) {                                  /* /Dимя, /ml, /mx */
    if ((w[0] == 'D' || w[0] == 'd') && nopc < 10) { strcpy(opcii[nopc], "-D"); strcat(opcii[nopc++], w + 1); }
    else if (!stricmp(w, "ml") && nopc < 10) strcpy(opcii[nopc++], "-Cp");
    else if (!stricmp(w, "mx") && nopc < 10) strcpy(opcii[nopc++], "-Cx");
  }                                             /* остальные ключи TASM пропускаем */
  else if (pole == 0) strcpy(istochnik, w);
  else if (pole == 1) strcpy(obekt, w);
}

int main(int argc, char *argv[])
{
  char *p = vse, *nachalo;
  char *args[16];
  int i, k = 0, pole = 0, kluch;

  for (i = 1; i < argc; i++) { strcat(vse, argv[i]); strcat(vse, " "); }
  while (*p) {
    if (*p == ' ' || *p == ';') { p++; continue; }
    if (*p == ',') { pole++; p++; continue; }
    kluch = (*p == '/');
    if (kluch) p++;
    nachalo = p;
    while (*p && *p != ' ' && *p != '/' && *p != ',' && *p != ';') p++;
    slovo(nachalo, p - nachalo, pole, kluch);
  }
  if (!istochnik[0]) { printf("TASM (переходник к JWasm): нет имени файла\n"); return 1; }
  if (!strchr(istochnik, '.')) strcat(istochnik, ".asm");
  if (!obekt[0]) { strcpy(obekt, istochnik); *strchr(obekt, '.') = 0; }
  if (!strchr(obekt, '.')) strcat(obekt, ".obj");

  args[k++] = "JWASMR";
  args[k++] = "-nologo";
  args[k++] = "-W0";            /* Turbo C пишет служебный макрос ?debug, о котором JWasm ворчит */
  for (i = 0; i < nopc; i++) args[k++] = opcii[i];
  strcpy(fo, "-Fo"); strcat(fo, obekt);
  args[k++] = fo;
  args[k++] = istochnik;
  args[k] = NULL;
  return spawnvp(P_WAIT, "JWASMR.EXE", args);
}
