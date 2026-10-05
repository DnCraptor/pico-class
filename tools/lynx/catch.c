/* catch.c - "Catch the stars": a small complete game.
 * Move the basket with LEFT/RIGHT, catch the falling stars.
 * A missed star costs a life; after 3 misses the game is over.
 * Part of the pico-class kit, see tools/lynx/README.md. */

#include <lynx.h>
#include <tgi.h>
#include <joystick.h>
#include <6502.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beep.h"
#include "catch_gfx.h"    /* basket_spr, star_spr: made by mksprites.py */

#define NSTARS   3        /* stars falling at the same time */
#define BASKET_Y 84

static SCB_REHV_PAL basket = {
    BPP_4 | TYPE_NORMAL, LITERAL | REHV, 0, 0, basket_spr, 72, BASKET_Y, 0x100, 0x100,
    { 0, 0, 0, 0, 0, 0xB6, 0, 0 }
};
static SCB_REHV_PAL star[NSTARS];
static unsigned char speed[NSTARS];
static unsigned int score, best;
static unsigned char lives;

static void text(const char *s, unsigned char col, unsigned char row)
{
    tgi_outtextxy(col * 8, row * 8, s);
}

/* put star number n back to the top at a random place */
static void new_star(unsigned char n)
{
    star[n].hpos = (rand() % 140) + 2;
    star[n].vpos = 10 - (rand() & 31);      /* a little above the screen */
    speed[n] = 1 + (score >> 3);            /* faster as the score grows */
    if (speed[n] > 3) speed[n] = 3;
}

/* draw everything; status: score line on top, message in the middle */
static void draw(const char *msg1, const char *msg2)
{
    char buf[24];
    unsigned char i;
    while (tgi_busy()) ;
    tgi_setcolor(COLOR_BLACK);
    tgi_bar(0, 0, 159, 101);
    tgi_setbgcolor(COLOR_BLACK);
    tgi_setcolor(COLOR_WHITE);
    sprintf(buf, "S%u L%u B%u", score, lives, best);
    text(buf, 0, 0);
    if (msg1) text(msg1, (20 - strlen(msg1)) / 2, 5);
    if (msg2) text(msg2, (20 - strlen(msg2)) / 2, 7);
    if (lives)
    {
        for (i = 0; i < NSTARS; i++)
            tgi_sprite(&star[i]);
        tgi_sprite(&basket);
    }
    tgi_updatedisplay();
}

static void play(void)
{
    unsigned char i, pad;
    score = 0; lives = 3; basket.hpos = 72;
    for (i = 0; i < NSTARS; i++)
    {
        new_star(i);
        star[i].vpos -= i * 40;             /* do not start all at once */
    }
    while (lives > 0)
    {
        pad = joy_read(JOY_1);
        if (JOY_LEFT(pad) && basket.hpos > 1) basket.hpos -= 2;
        if (JOY_RIGHT(pad) && basket.hpos < 143) basket.hpos += 2;

        for (i = 0; i < NSTARS; i++)
        {
            star[i].vpos += speed[i];
            /* caught: the star reached the basket and they overlap */
            if (star[i].vpos >= BASKET_Y - 10 && star[i].vpos <= BASKET_Y + 4 &&
                abs(star[i].hpos - basket.hpos) < 14)
            {
                score++;
                if (score > best) best = score;
                beep(1200, 6);
                new_star(i);
            }
            /* missed: the star fell off the bottom */
            if (star[i].vpos > 102)
            {
                lives--;
                beep(200, 20);
                new_star(i);
            }
        }
        beep_tick();
        draw(0, 0);
    }
}

void main(void)
{
    unsigned char i;
    tgi_install(tgi_static_stddrv);
    tgi_init();
    joy_install(joy_static_stddrv);
    CLI();
    tgi_setframerate(60);
    beep_init();
    for (i = 0; i < NSTARS; i++)
    {
        star[i].sprctl0 = BPP_4 | TYPE_NORMAL;
        star[i].sprctl1 = LITERAL | REHV;
        star[i].data = star_spr;
        star[i].hsize = star[i].vsize = 0x100;
        star[i].penpal[4] = 0x78;          /* pen 8: peach, pen 9: yellow */
    }

    best = 0;
    while (1)
    {
        /* wait for A; calling rand() meanwhile makes every game new */
        while (!JOY_BTN_A(joy_read(JOY_1)))
        {
            rand();
            beep_tick();
            draw("CATCH THE STARS", "Press A");
        }
        play();
        for (i = 0; i < 90; i++)            /* a short pause */
        {
            beep_tick();
            draw("GAME OVER", 0);
        }
    }
}
