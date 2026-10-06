/* hello.c - first Gamate program: text, shades, buttons, sound.
 * Part of the pico-class kit, see tools/gamate/README.md. */

#include <joystick.h>
#include <stdio.h>
#include "gm.h"

void main(void)
{
    char buf[8];
    unsigned char shade = 3, pad, old = 0, frames = 0, changed = 1, i;
    unsigned int seconds = 0;

    joy_install(joy_static_stddrv);
    scr_init();
    beep_init();

    /* four shades of the screen: 0 (background) .. 3 */
    for (i = 0; i < 4; i++)
    {
        scr_text(2 + i * 4, 17, "##", i);
        buf[0] = '0' + i; buf[1] = 0;
        scr_text(2 + i * 4, 18, buf, 3);
    }
    scr_text(0, 2, "Written in C,", 3);
    scr_text(0, 3, "compiled by cc65.", 3);
    scr_text(0, 5, "Seconds:", 3);
    scr_text(0, 7, "Buttons:", 3);
    scr_text(0, 11, "A and B: sound", 3);
    scr_text(0, 15, "Shades:", 3);

    while (1)
    {
        scr_wait();
        /* a clock: SCR_FPS frames a second */
        if (++frames == SCR_FPS)
        {
            frames = 0;
            seconds++;
            shade = shade == 1 ? 3 : shade - 1;  /* the title changes shade */
            changed = 1;
        }
        pad = joy_read(JOY_1);

        /* a new press (not a hold) of A or B: a beep */
        if ((pad & ~old) & JOY_BTN_1_MASK) beep(440, 10);
        if ((pad & ~old) & JOY_BTN_2_MASK) beep(880, 10);
        if (pad != old) changed = 1;
        old = pad;
        beep_tick();

        /* there is no time to redraw all the text every frame:
           only when something has changed */
        if (!changed) continue;
        changed = 0;
        scr_text(3, 0, "HELLO, GAMATE!", shade);
        sprintf(buf, "%u", seconds);
        scr_text(9, 5, buf, 3);
        scr_text(0, 8, JOY_UP(pad) ? "UP" : "  ", 3);
        scr_text(3, 8, JOY_DOWN(pad) ? "DOWN" : "    ", 3);
        scr_text(8, 8, JOY_LEFT(pad) ? "LEFT" : "    ", 3);
        scr_text(13, 8, JOY_RIGHT(pad) ? "RIGHT" : "     ", 3);
        scr_text(0, 9, JOY_BTN_1(pad) ? "A" : " ", 3);
        scr_text(2, 9, JOY_BTN_2(pad) ? "B" : " ", 3);
    }
}
