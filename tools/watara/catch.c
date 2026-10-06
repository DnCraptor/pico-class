/* catch.c - "Catch the stars": a small complete game.
 * Move the basket with LEFT/RIGHT, catch the falling stars.
 * A missed star costs a life; after 3 misses the game is over.
 * Part of the pico-class kit, see tools/watara/README.md. */

#include <joystick.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sv.h"
#include "catch_gfx.h"    /* basket, star: made by mkgfx.py */

#define NSTARS   3        /* stars falling at the same time */
#define BASKET_Y (SCR_H - 20)
#define TOP      10       /* stars appear below the score line */

static unsigned char sx[NSTARS], sy[NSTARS], speed[NSTARS], visible[NSTARS];
static unsigned char bx = 72;
static unsigned int score, best;
static unsigned char lives;

static void status(void)
{
    char buf[24];
    sprintf(buf, "S%u L%u B%u   ", score, lives, best);
    scr_text(0, 0, buf, 3);
}

static void message(const char *msg1, const char *msg2)
{
    scr_text(0, 8, "                    ", 0);
    scr_text(0, 10, "                    ", 0);
    if (msg1) scr_text((20 - strlen(msg1)) / 2, 8, msg1, 3);
    if (msg2) scr_text((20 - strlen(msg2)) / 2, 10, msg2, 3);
}

/* put star number n back to the top at a random place; it waits above
   the screen for a while (wait frames) before it shows up */
static int wait[NSTARS];
static void new_star(unsigned char n)
{
    sx[n] = (rand() % 140) + 2;
    sy[n] = TOP;
    wait[n] = rand() & 31;
    speed[n] = 1 + (score >> 3);            /* faster as the score grows */
    if (speed[n] > 3) speed[n] = 3;
}

static void play(void)
{
    unsigned char i, pad, obx;
    score = 0; lives = 3; bx = 72;
    scr_clear();
    status();
    for (i = 0; i < NSTARS; i++)
    {
        new_star(i);
        wait[i] += i * 40;                  /* do not start all at once */
        visible[i] = 0;
    }
    while (lives > 0)
    {
        pad = joy_read(JOY_1);
        obx = bx;
        if (JOY_LEFT(pad) && bx > 1) bx -= 2;
        if (JOY_RIGHT(pad) && bx < 143) bx += 2;

        scr_wait();
        for (i = 0; i < NSTARS; i++)
        {
            if (wait[i]) { wait[i]--; continue; }
            if (visible[i]) scr_move(sx[i], sy[i], sx[i], sy[i] + speed[i], 16, 16);
            sy[i] += speed[i];
            visible[i] = 1;
            /* caught: the star reached the basket and they overlap */
            if (sy[i] >= BASKET_Y - 10 && sy[i] <= BASKET_Y + 4 &&
                abs(sx[i] - bx) < 14)
            {
                score++;
                if (score > best) best = score;
                beep(1200, 6);
                scr_erase(sx[i], sy[i], 16, 16);
                new_star(i);
                visible[i] = 0;
                status();
            }
            /* missed: the star fell off the bottom */
            else if (sy[i] > SCR_H - 16)
            {
                lives--;
                beep(200, 20);
                scr_erase(sx[i], sy[i], 16, 16);
                new_star(i);
                visible[i] = 0;
                status();
            }
        }
        if (obx != bx) scr_move(obx, BASKET_Y, bx, BASKET_Y, 16, 16);
        for (i = 0; i < NSTARS; i++)
            if (visible[i]) scr_sprite(sx[i], sy[i], star, 16, 16);
        scr_sprite(bx, BASKET_Y, basket, 16, 16);
        beep_tick();
    }
}

void main(void)
{
    unsigned char i;
    joy_install(joy_static_stddrv);
    scr_init();
    beep_init();

    best = 0;
    while (1)
    {
        /* wait for A; calling rand() meanwhile makes every game new */
        scr_clear();
        status();
        message("CATCH THE STARS", "Press A");
        while (!JOY_BTN_1(joy_read(JOY_1)))
        {
            rand();
            scr_wait();
            beep_tick();
        }
        play();
        message("GAME OVER", 0);
        for (i = 0; i < 90; i++)            /* a short pause */
        {
            scr_wait();
            beep_tick();
        }
    }
}
