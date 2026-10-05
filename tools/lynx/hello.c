/* hello.c - first Atari Lynx program: text, colours, buttons, sound.
 * Part of the pico-class kit, see tools/lynx/README.md. */

#include <lynx.h>
#include <tgi.h>
#include <joystick.h>
#include <6502.h>
#include <stdio.h>
#include <time.h>
#include "beep.h"

/* text colours to cycle through, from the standard Lynx palette */
static const unsigned char rainbow[8] = {
    COLOR_WHITE, COLOR_YELLOW, COLOR_LIGHTBLUE, COLOR_LIGHTGREEN,
    COLOR_PEACH, COLOR_RED, COLOR_PINK, COLOR_LIGHTGREY
};

static void text(const char *s, unsigned char col, unsigned char row)
{
    tgi_outtextxy(col * 8, row * 8, s);
}

void main(void)
{
    char buf[8];
    unsigned char c = 0, pad, old = 0;
    unsigned int seconds = 0, s;

    tgi_install(tgi_static_stddrv);
    tgi_init();
    joy_install(joy_static_stddrv);
    CLI();
    tgi_setframerate(60);
    beep_init();

    while (1)
    {
        /* a clock: clock() counts frames, CLOCKS_PER_SEC of them a second */
        s = clock() / CLOCKS_PER_SEC;
        if (s != seconds)
        {
            seconds = s;
            c = (c + 1) & 7;      /* the text changes colour every second */
        }
        pad = joy_read(JOY_1);

        /* a new press (not a hold) of A or B: a beep */
        if ((pad & ~old) & JOY_BTN_A_MASK) beep(440, 10);
        if ((pad & ~old) & JOY_BTN_B_MASK) beep(880, 10);
        old = pad;
        beep_tick();

        /* draw the whole screen again every frame */
        while (tgi_busy()) ;
        tgi_setcolor(COLOR_BLUE);
        tgi_bar(0, 0, 159, 101);
        tgi_setbgcolor(COLOR_BLUE);
        tgi_setcolor(rainbow[c]);
        text("HELLO, LYNX!", 4, 0);
        text("Written in C,", 0, 2);
        text("compiled by cc65.", 0, 3);
        sprintf(buf, "%u", seconds);
        text("Seconds:", 0, 5);
        text(buf, 9, 5);
        text("Buttons:", 0, 7);
        if (JOY_UP(pad)) text("UP", 0, 8);
        if (JOY_DOWN(pad)) text("DOWN", 3, 8);
        if (JOY_LEFT(pad)) text("LEFT", 8, 8);
        if (JOY_RIGHT(pad)) text("RIGHT", 13, 8);
        if (JOY_BTN_A(pad)) text("A", 0, 9);
        if (JOY_BTN_B(pad)) text("B", 2, 9);
        text("A and B: sound", 0, 11);
        tgi_updatedisplay();  /* show it at the next frame */
    }
}
