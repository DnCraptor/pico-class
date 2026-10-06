/* sprite.c - pictures that move: a robot you move with the joypad and a
 * ball that moves by itself and bounces off the screen edges. The
 * Gamate has no sprite hardware, so the program itself erases each
 * picture at its old place and copies it to the new one, every frame.
 * A: other shades for the robot, B: a robot twice as big.
 * Part of the pico-class kit, see tools/gamate/README.md. */

#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include "gm.h"
#include "sprite_gfx.h"   /* robot1, robot1b, robot2, robot2b, ball: made by mkgfx.py */

#define TOP 10            /* the score line is above */

void main(void)
{
    char buf[8];
    unsigned char rx = 72, ry = 70, bx = 10, by = 20, size = 1, colour = 0;
    unsigned char orx = 72, ory = 70, osize = 1, obx = 10, oby = 20;
    signed char dx = 1, dy = 1;       /* ball speed: how far it moves each frame */
    unsigned char pad, old = 0, rs;
    unsigned int score = 0;
    const unsigned char *rspr;

    joy_install(joy_static_stddrv);
    scr_init();
    beep_init();
    scr_text(0, 0, "A:shade B:size", 3);

    while (1)
    {
        rs = 16 * size;               /* robot size in pixels */

        /* the joypad moves the robot */
        pad = joy_read(JOY_1);
        if (JOY_LEFT(pad) && rx > 0) rx--;
        if (JOY_RIGHT(pad) && rx < 160 - rs) rx++;
        if (JOY_UP(pad) && ry > TOP) ry--;
        if (JOY_DOWN(pad) && ry < SCR_H - rs) ry++;

        /* button A: other shades */
        if ((pad & ~old) & JOY_BTN_1_MASK) colour = 1 - colour;
        /* button B: 1x - 2x */
        if ((pad & ~old) & JOY_BTN_2_MASK)
        {
            size = 3 - size;
            rs = 16 * size;
            if (rx > 160 - rs) rx = 160 - rs;
            if (ry > SCR_H - rs) ry = SCR_H - rs;
        }
        old = pad;

        /* the ball moves by itself and bounces */
        bx += dx;
        by += dy;
        if (bx == 0 || bx >= 144) { dx = -dx; beep(1000, 3); }
        if (by <= TOP || by >= SCR_H - 16) { dy = -dy; beep(1000, 3); }

        /* do the robot and the ball touch? */
        if (bx + 14 > rx && bx < rx + rs - 2 && by + 14 > ry && by < ry + rs - 2)
        {
            score++;
            beep(1500, 8);
            by = TOP + 2;             /* throw the ball back up */
            bx = (rand() & 63) + 40;
            sprintf(buf, "%u", score);
            scr_text(16, 0, buf, 3);
        }
        beep_tick();

        /* wait for the frame, then erase the old pictures and draw the new */
        scr_wait();
        scr_move(obx, oby, bx, by, 16, 16);
        if (osize != size) scr_erase(orx, ory, 16 * osize, 16 * osize);
        else scr_move(orx, ory, rx, ry, rs, rs);
        scr_sprite(bx, by, ball, 16, 16);
        if (size == 1) rspr = colour ? robot1b : robot1;
        else rspr = colour ? robot2b : robot2;
        scr_sprite(rx, ry, rspr, rs, rs);
        obx = bx; oby = by; orx = rx; ory = ry; osize = size;
    }
}
