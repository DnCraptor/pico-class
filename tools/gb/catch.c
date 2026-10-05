/* catch.c - "Catch the stars": a small complete game.
 * Move the basket with LEFT/RIGHT, catch the falling stars.
 * A missed star costs a life; after 3 misses the game is over.
 * Part of the pico-class kit, see tools/gb/README.md. */

#include <gb/gb.h>
#include <gbdk/console.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>
#include "beep.h"
#include "catch_gfx.h"    /* basket_spr, star_spr: made by mksprites.py */

#define NSTARS   3        /* stars falling at the same time */
#define BASKET_Y 120

static unsigned char x;                              /* basket position */
static signed int sx[NSTARS], sy[NSTARS];
static unsigned char speed[NSTARS];
static unsigned int score, best;
static unsigned char lives;

static void put16(unsigned char spr, unsigned char tile, signed int x, signed int y)
{
    set_sprite_tile(spr, tile);
    set_sprite_tile(spr + 1, tile + 2);
    if (y < -16) y = -16;                  /* above the screen: hidden */
    move_sprite(spr, x + 8, y + 16);
    move_sprite(spr + 1, x + 16, y + 16);
}

/* put star number n back to the top at a random place */
static void new_star(unsigned char n)
{
    sx[n] = (rand() % 136) + 4;
    sy[n] = 16 - (rand() & 63);            /* a little above the screen */
    speed[n] = 1 + (score >> 3);           /* faster as the score grows */
    if (speed[n] > 3) speed[n] = 3;
}

static void show_status(void)
{
    /* GBDK's printf has no field widths: clear the line first */
    gotoxy(0, 0);
    printf("                    ");
    gotoxy(0, 0);
    printf("Score %u Lives %u", score, lives);
    gotoxy(0, 1);
    printf("Best %u", best);
}

static void hide_sprites(void)
{
    unsigned char i;
    for (i = 0; i < 2 + 2 * NSTARS; i++) move_sprite(i, 0, 0);
}

static void play(void)
{
    unsigned char i, pad;
    score = 0; lives = 3; x = 72;
    for (i = 0; i < NSTARS; i++)
    {
        new_star(i);
        sy[i] -= i * 50;                   /* do not start all at once */
    }
    gotoxy(0, 8);  printf("                    ");
    gotoxy(0, 10); printf("                    ");
    show_status();

    while (lives > 0)
    {
        pad = joypad();
        if ((pad & J_LEFT) && x > 1) x -= 2;
        if ((pad & J_RIGHT) && x < 143) x += 2;

        for (i = 0; i < NSTARS; i++)
        {
            sy[i] += speed[i];
            /* caught: the star reached the basket and they overlap */
            if (sy[i] >= BASKET_Y - 10 && sy[i] <= BASKET_Y + 4 && abs(sx[i] - x) < 14)
            {
                score++;
                if (score > best) best = score;
                beep(1200);
                new_star(i);
                show_status();
            }
            /* missed: the star fell off the bottom */
            if (sy[i] > 144)
            {
                lives--;
                beep(200);
                new_star(i);
                show_status();
            }
            put16(2 + 2 * i, 4, sx[i], sy[i]);
        }
        put16(0, 0, x, BASKET_Y);
        vsync();
    }
}

void main(void)
{
    unsigned char i;
    beep_init();
    SPRITES_8x16;
    set_sprite_data(0, 4, basket_spr);
    set_sprite_data(4, 4, star_spr);
    OBP0_REG = 0xE4;
    SHOW_SPRITES;

    best = 0;
    while (1)
    {
        gotoxy(2, 8);  printf("CATCH THE STARS");
        gotoxy(1, 10); printf("Press START");
        /* wait for START; counting frames meanwhile makes every game new */
        i = 0;
        while (!(joypad() & J_START))
        {
            i++;
            vsync();
        }
        initrand(i | (DIV_REG << 8));
        play();
        gotoxy(5, 6); printf("GAME OVER");
        for (i = 0; i < 90; i++) vsync();  /* a short pause */
        gotoxy(5, 6); printf("         ");
        hide_sprites();
    }
}
