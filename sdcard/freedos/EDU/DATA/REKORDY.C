/* Рекорды: таблица из пяти записей в файле REKORDY.TXT */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct rekord {
    char imja[16];          /* 15 букв + байт 0 в конце */
    int  ochki;
};

struct rekord t[6];         /* 5 рекордов + 1 место для нового */

/* прочитать строку, не больше razmer-1 букв; лишнее - пропустить */
void stroka(char *s, int razmer, FILE *f)
{
    int c;
    char *konec;
    if (fgets(s, razmer, f) == NULL) { s[0] = 0; return; }
    konec = strchr(s, '\n');
    if (konec != NULL)
        *konec = 0;                  /* строка влезла целиком */
    else                             /* не влезла: дочитать до конца строки */
        while ((c = fgetc(f)) != '\n' && c != EOF)
            ;
}

void pokazat(void)
{
    int i;
    printf("\n   Имя             Очки\n");
    for (i = 0; i < 5; i++)
        printf("%2d %-16s%d\n", i + 1, t[i].imja, t[i].ochki);
    printf("\n");
}

int main(void)
{
    FILE *f;
    char buf[40];
    int i, k;
    struct rekord r;

    for (i = 0; i < 5; i++) {
        strcpy(t[i].imja, "---");
        t[i].ochki = 0;
    }
    f = fopen("REKORDY.TXT", "r");
    if (f != NULL) {                 /* файла может и не быть */
        for (i = 0; i < 5; i++) {
            stroka(t[i].imja, sizeof t[i].imja, f);
            stroka(buf, sizeof buf, f);
            t[i].ochki = atoi(buf);
        }
        fclose(f);
    }
    pokazat();
    printf("Имя: ");   stroka(t[5].imja, sizeof t[5].imja, stdin);
    printf("Очки: ");  stroka(buf, sizeof buf, stdin);
    t[5].ochki = atoi(buf);
    /* сортировка обменом: большие очки - наверх */
    for (k = 1; k <= 5; k++)
        for (i = 0; i < 6 - k; i++)
            if (t[i].ochki < t[i + 1].ochki) {
                r = t[i]; t[i] = t[i + 1]; t[i + 1] = r;  /* структуру - целиком */
            }
    f = fopen("REKORDY.TXT", "w");
    for (i = 0; i < 5; i++)
        fprintf(f, "%s\n%d\n", t[i].imja, t[i].ochki);
    fclose(f);
    pokazat();
    return 0;
}
