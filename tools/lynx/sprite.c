/* sprite.c - hardware sprites: a robot you move with the joypad and a
 * ball that moves by itself and bounces off the screen edges. The Lynx
 * sprite engine can also stretch sprites: B makes the robot bigger.
 * Part of the pico-class kit, see tools/lynx/README.md. */

#include <lynx.h>
#include <tgi.h>
#include <joystick.h>
#include <6502.h>
#include <stdio.h>
#include <stdlib.h>
#include "beep.h"
#include "sprite_gfx.h"   /* robot_spr, ball_spr: made by mksprites.py */

/* The pen palette says which colour of the screen palette each pen of the
 * sprite shows (two pens a byte). Pen 0 stays 0: transparent. */
static const unsigned char robot_pens[8]  = { 0x01, 0x4F, 0xD1, 0xC0, 0, 0, 0, 0 };
static const unsigned char robot_pens2[8] = { 0x08, 0x7F, 0xA1, 0xC0, 0, 0, 0, 0 };

/* sprite control blocks: what to draw, where and how big (0x100 = 1.0) */
static SCB_REHV_PAL robot = {
    BPP_4 | TYPE_NORMAL, LITERAL | REHV, 0, 0, robot_spr, 72, 60, 0x100, 0x100,
    { 0x01, 0x4F, 0xD1, 0xC0, 0, 0, 0, 0 }
};
static SCB_REHV_PAL ball = {
    BPP_4 | TYPE_NORMAL, LITERAL | REHV, 0, 0, ball_spr, 10, 20, 0x100, 0x100,
    { 0x00, 0x00, 0x00, 0x0A, 0xF0, 0, 0, 0 }
};

static void text(const char *s, unsigned char col, unsigned char row)
{
    tgi_outtextxy(col * 8, row * 8, s);
}

void main(void)
{
    char buf[8];
    signed char dx = 1, dy = 1;       /* ball speed: how far it moves each frame */
    unsigned char pad, old = 0, colour = 0, size = 1, i;
    unsigned int score = 0;

    tgi_install(tgi_static_stddrv);
    tgi_init();
    joy_install(joy_static_stddrv);
    CLI();
    tgi_setframerate(60);
    beep_init();

    while (1)
    {
        /* the joypad moves the robot */
        pad = joy_read(JOY_1);
        if (JOY_LEFT(pad) && robot.hpos > 0) robot.hpos--;
        if (JOY_RIGHT(pad) && robot.hpos < 160 - 16 * size) robot.hpos++;
        if (JOY_UP(pad) && robot.vpos > 10) robot.vpos--;
        if (JOY_DOWN(pad) && robot.vpos < 102 - 16 * size) robot.vpos++;

        /* button A: the other pen palette */
        if ((pad & ~old) & JOY_BTN_A_MASK)
        {
            colour = 1 - colour;
            for (i = 0; i < 8; i++)
                robot.penpal[i] = colour ? robot_pens2[i] : robot_pens[i];
        }
        /* button B: bigger robot, 1x - 2x - 3x; the hardware stretches it */
        if ((pad & ~old) & JOY_BTN_B_MASK)
        {
            size = size % 3 + 1;
            robot.hsize = robot.vsize = size * 0x100;
            if (robot.hpos > 160 - 16 * size) robot.hpos = 160 - 16 * size;
            if (robot.vpos > 102 - 16 * size) robot.vpos = 102 - 16 * size;
        }
        old = pad;

        /* the ball moves by itself and bounces */
        ball.hpos += dx;
        ball.vpos += dy;
        if (ball.hpos <= 0 || ball.hpos >= 144) { dx = -dx; beep(1000, 3); }
        if (ball.vpos <= 10 || ball.vpos >= 86) { dy = -dy; beep(1000, 3); }

        /* do the robot and the ball touch? */
        if (ball.hpos + 14 > robot.hpos && ball.hpos < robot.hpos + 16 * size - 2 &&
            ball.vpos + 14 > robot.vpos && ball.vpos < robot.vpos + 16 * size - 2)
        {
            score++;
            beep(1500, 8);
            /* throw the ball back up */
            ball.vpos = 12;
            ball.hpos = (rand() & 63) + 40;
        }
        beep_tick();

        /* draw the whole screen again every frame */
        while (tgi_busy()) ;
        tgi_setcolor(COLOR_DARKBROWN);
        tgi_bar(0, 0, 159, 101);
        tgi_setbgcolor(COLOR_DARKBROWN);
        tgi_setcolor(COLOR_WHITE);
        text("A:colour B:size", 0, 0);
        sprintf(buf, "%u", score);
        text(buf, 17, 0);
        tgi_sprite(&ball);
        tgi_sprite(&robot);
        tgi_updatedisplay();
    }
}
