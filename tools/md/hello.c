/* hello.c - first Mega Drive program: text, colours, joypad, sound.
 * Part of the pico-class kit, see tools/md/README.md. */

#include <genesis.h>
#include "beep.h"

/* text colours to cycle through, RGB 0..7 each */
static const u16 rainbow[8] = {
    RGB3_3_3_TO_VDPCOLOR(7, 7, 7), RGB3_3_3_TO_VDPCOLOR(7, 7, 0),
    RGB3_3_3_TO_VDPCOLOR(0, 7, 7), RGB3_3_3_TO_VDPCOLOR(0, 7, 0),
    RGB3_3_3_TO_VDPCOLOR(7, 3, 0), RGB3_3_3_TO_VDPCOLOR(7, 0, 0),
    RGB3_3_3_TO_VDPCOLOR(7, 0, 7), RGB3_3_3_TO_VDPCOLOR(7, 3, 7)
};

/* show a word when the button is held, blanks when not */
static void show(u16 pad, u16 button, const char *name, u16 x, u16 y)
{
    if (pad & button) VDP_drawText(name, x, y);
    else VDP_clearText(x, y, strlen(name));
}

int main(bool hardReset)
{
    char buf[8];
    u16 frames = 0, seconds = 0, c = 0, pad, old = 0;

    PAL_setColor(0, RGB3_3_3_TO_VDPCOLOR(0, 0, 2));   /* background: dark blue */
    PAL_setColor(15, rainbow[0]);                     /* text: white */
    beep_init();

    VDP_drawText("HELLO, MEGA DRIVE!", 11, 3);
    VDP_drawText("This program is written in C", 2, 6);
    VDP_drawText("and compiled with SGDK.", 2, 7);
    VDP_drawText("Seconds:", 2, 10);
    VDP_drawText("Press the buttons:", 2, 13);
    VDP_drawText("A, B and C make a sound.", 2, 20);

    while (TRUE)
    {
        /* a clock: 60 frames = 1 second */
        frames++;
        if (frames == 60)
        {
            frames = 0;
            seconds++;
            uintToStr(seconds, buf, 5);
            VDP_drawText(buf, 11, 10);
            /* the text changes colour every second */
            c = (c + 1) & 7;
            PAL_setColor(15, rainbow[c]);
        }

        /* show which buttons are held */
        pad = JOY_readJoypad(JOY_1);
        show(pad, BUTTON_UP, "UP", 2, 15);
        show(pad, BUTTON_DOWN, "DOWN", 5, 15);
        show(pad, BUTTON_LEFT, "LEFT", 10, 15);
        show(pad, BUTTON_RIGHT, "RIGHT", 15, 15);
        show(pad, BUTTON_A, "A", 21, 15);
        show(pad, BUTTON_B, "B", 23, 15);
        show(pad, BUTTON_C, "C", 25, 15);
        show(pad, BUTTON_START, "START", 2, 17);

        /* a new press (not a hold) of A, B or C: a beep */
        if ((pad & ~old) & BUTTON_A) beep(440, 10);
        if ((pad & ~old) & BUTTON_B) beep(660, 10);
        if ((pad & ~old) & BUTTON_C) beep(880, 10);
        old = pad;

        beep_tick();
        SYS_doVBlankProcess();   /* wait for the next frame */
    }
    return 0;
}
