/* hello.c - first Game Boy program: text, shades, buttons, sound.
 * Part of the pico-class kit, see tools/gb/README.md. */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdio.h>
#include "beep.h"

/* the four shades of the screen, darkest last; BGP = background palette */
static const unsigned char shades[4] = { 0xE4, 0x1B, 0x39, 0x93 };

/* show a word when the button is held, blanks when not */
static void show(unsigned char pad, unsigned char button, const char *name, const char *blank,
                 unsigned char x, unsigned char y)
{
    gotoxy(x, y);
    printf((pad & button) ? name : blank);
}

void main(void)
{
    unsigned char frames = 0, c = 0, pad, old = 0;
    unsigned int seconds = 0;

    beep_init();
    gotoxy(2, 1);  printf("HELLO, GAME BOY!");
    gotoxy(0, 3);  printf("This program is");
    gotoxy(0, 4);  printf("written in C and");
    gotoxy(0, 5);  printf("compiled with GBDK.");
    gotoxy(0, 7);  printf("Seconds:");
    gotoxy(0, 9);  printf("Press the buttons:");
    gotoxy(0, 15); printf("A and B: sound");

    while (1)
    {
        /* a clock: 60 frames = 1 second */
        frames++;
        if (frames == 60)
        {
            frames = 0;
            seconds++;
            gotoxy(9, 7);
            printf("%u", seconds);
            /* the screen changes its shades every second */
            c = (c + 1) & 3;
            BGP_REG = shades[c];
        }

        /* show which buttons are held */
        pad = joypad();
        show(pad, J_UP, "UP", "  ", 0, 11);
        show(pad, J_DOWN, "DOWN", "    ", 3, 11);
        show(pad, J_LEFT, "LEFT", "    ", 8, 11);
        show(pad, J_RIGHT, "RIGHT", "     ", 13, 11);
        show(pad, J_A, "A", " ", 0, 12);
        show(pad, J_B, "B", " ", 2, 12);
        show(pad, J_SELECT, "SELECT", "      ", 4, 12);
        show(pad, J_START, "START", "     ", 11, 12);

        /* a new press (not a hold) of A or B: a beep */
        if ((pad & ~old) & J_A) beep(440);
        if ((pad & ~old) & J_B) beep(880);
        old = pad;

        vsync();   /* wait for the next frame */
    }
}
