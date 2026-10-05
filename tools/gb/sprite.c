/* sprite.c - hardware sprites: a robot you move with the joypad and a
 * ball that moves by itself and bounces off the screen edges.
 * Part of the pico-class kit, see tools/gb/README.md. */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>
#include "beep.h"
#include "sprite_gfx.h"   /* robot_spr, ball_spr: made by mksprites.py */

/* a 16x16 picture is two 8x16 sprites: tiles n, n+1 left, n+2, n+3 right */
static void put16(unsigned char spr, unsigned char tile, unsigned char x, unsigned char y)
{
    set_sprite_tile(spr, tile);
    set_sprite_tile(spr + 1, tile + 2);
    move_sprite(spr, x + 8, y + 16);       /* the hardware counts from (-8, -16) */
    move_sprite(spr + 1, x + 16, y + 16);
}

void main(void)
{
    unsigned char x = 72, y = 100;        /* robot position */
    unsigned char bx = 10, by = 30;       /* ball position */
    signed char dx = 1, dy = 1;           /* ball speed */
    unsigned char pad, old = 0;
    unsigned int score = 0;

    beep_init();
    gotoxy(0, 0);  printf("Move the robot");
    gotoxy(0, 1);  printf("A: colours");
    gotoxy(0, 17); printf("Score: 0");

    SPRITES_8x16;
    set_sprite_data(0, 4, robot_spr);
    set_sprite_data(4, 4, ball_spr);
    OBP0_REG = 0xE4;                       /* sprite palette 0: normal shades */
    OBP1_REG = 0x1B;                       /* sprite palette 1: inverted */
    SHOW_SPRITES;

    while (1)
    {
        /* the joypad moves the robot */
        pad = joypad();
        if ((pad & J_LEFT) && x > 0) x--;
        if ((pad & J_RIGHT) && x < 144) x++;
        if ((pad & J_UP) && y > 16) y--;
        if ((pad & J_DOWN) && y < 120) y++;

        /* button A: the other sprite palette */
        if ((pad & ~old) & J_A)
        {
            set_sprite_prop(0, get_sprite_prop(0) ^ S_PALETTE);
            set_sprite_prop(1, get_sprite_prop(1) ^ S_PALETTE);
        }
        old = pad;

        /* the ball moves by itself and bounces */
        bx += dx;
        by += dy;
        if (bx == 0 || bx >= 144) { dx = -dx; beep(1000); }
        if (by <= 16 || by >= 120) { dy = -dy; beep(1000); }

        /* do the robot and the ball touch? (both are 16x16) */
        if (abs((int)bx - x) < 14 && abs((int)by - y) < 14)
        {
            score++;
            gotoxy(7, 17);
            printf("%u", score);
            beep(1500);
            /* throw the ball back up */
            by = 20;
            bx = (rand() & 63) + 40;
        }

        put16(0, 0, x, y);
        put16(2, 4, bx, by);
        vsync();
    }
}
