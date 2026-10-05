/* catch.c - "Catch the stars": a small complete game.
 * Move the basket with LEFT/RIGHT, catch the falling stars.
 * A missed star costs a life; after 3 misses the game is over.
 * Part of the pico-class kit, see tools/md/README.md. */

#include <genesis.h>
#include "beep.h"
#include "catch_gfx.h"    /* basket_spr, star_spr: made by mksprites.py */

#define BASKET_TILE TILE_USER_INDEX
#define STAR_TILE   (TILE_USER_INDEX + 4)
#define NSTARS      3     /* stars falling at the same time */
#define BASKET_Y    200

static const u16 basket_pal[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, RGB3_3_3_TO_VDPCOLOR(4, 2, 0), RGB3_3_3_TO_VDPCOLOR(6, 4, 2)
};
static const u16 star_pal[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, RGB3_3_3_TO_VDPCOLOR(7, 3, 0), RGB3_3_3_TO_VDPCOLOR(7, 7, 0)
};

static s16 x;                                      /* basket position */
static s16 sx[NSTARS], sy[NSTARS], speed[NSTARS];
static u16 score, lives, best;

static void number(u16 n, u16 width, u16 col, u16 row)
{
    char buf[8];
    uintToStr(n, buf, width);
    VDP_drawText(buf, col, row);
}

/* put star number n back to the top at a random place */
static void new_star(u16 n)
{
    sx[n] = (random() % 288) + 8;
    sy[n] = 16 - (random() & 63);           /* a little above the screen */
    speed[n] = 1 + (score >> 3);            /* faster as the score grows */
    if (speed[n] > 4) speed[n] = 4;
}

static void show_status(void)
{
    VDP_drawText("Score", 1, 1);   number(score, 4, 7, 1);
    VDP_drawText("Lives", 15, 1);  number(lives, 1, 21, 1);
    VDP_drawText("Best", 29, 1);   number(best, 4, 34, 1);
}

static void draw_sprites(void)
{
    VDP_setSpriteFull(0, x, BASKET_Y, SPRITE_SIZE(2, 2),
                      TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, BASKET_TILE), 1);
    for (u16 i = 0; i < NSTARS; i++)
        VDP_setSpriteFull(i + 1, sx[i], sy[i], SPRITE_SIZE(2, 2),
                          TILE_ATTR_FULL(PAL2, 1, FALSE, FALSE, STAR_TILE),
                          i + 1 < NSTARS ? i + 2 : 0);
    VDP_updateSprites(NSTARS + 1, DMA_QUEUE);
}

static void hide_sprites(void)
{
    /* sprites below the screen are not visible */
    VDP_setSpriteFull(0, 0, 240, SPRITE_SIZE(1, 1), 0, 0);
    VDP_updateSprites(1, DMA_QUEUE);
}

static void play(void)
{
    u16 pad;
    score = 0; lives = 3; x = 152;
    for (u16 i = 0; i < NSTARS; i++)
    {
        new_star(i);
        sy[i] -= i * 60;                    /* do not start all at once */
    }
    VDP_clearTextLine(13);
    VDP_clearTextLine(15);
    show_status();

    while (lives > 0)
    {
        pad = JOY_readJoypad(JOY_1);
        if ((pad & BUTTON_LEFT) && x > 0) x -= 3;
        if ((pad & BUTTON_RIGHT) && x < 304) x += 3;

        for (u16 i = 0; i < NSTARS; i++)
        {
            sy[i] += speed[i];
            /* caught: the star reached the basket and they overlap */
            if (sy[i] >= BASKET_Y - 10 && sy[i] <= BASKET_Y + 4 && abs(sx[i] - x) < 14)
            {
                score++;
                if (score > best) best = score;
                beep(1200, 6);
                new_star(i);
                show_status();
            }
            /* missed: the star fell off the bottom */
            if (sy[i] > 224)
            {
                lives--;
                beep(200, 20);
                new_star(i);
                show_status();
            }
        }

        draw_sprites();
        beep_tick();
        SYS_doVBlankProcess();
    }
}

int main(bool hardReset)
{
    PAL_setColor(0, RGB3_3_3_TO_VDPCOLOR(0, 0, 1));   /* background: night */
    PAL_setColor(15, RGB3_3_3_TO_VDPCOLOR(7, 7, 7));  /* text: white */
    beep_init();
    VDP_loadTileData(basket_spr, BASKET_TILE, 4, DMA);
    VDP_loadTileData(star_spr, STAR_TILE, 4, DMA);
    PAL_setColors(16, basket_pal, 16, DMA);
    PAL_setColors(32, star_pal, 16, DMA);

    best = 0;
    while (TRUE)
    {
        VDP_drawText("CATCH THE STARS", 12, 13);
        VDP_drawText("Press START to begin", 10, 15);
        /* wait for START; calling random() meanwhile makes every game new */
        while (!(JOY_readJoypad(JOY_1) & BUTTON_START))
        {
            random();
            SYS_doVBlankProcess();
        }
        play();
        VDP_drawText("GAME OVER", 15, 11);
        for (u16 i = 0; i < 90; i++) SYS_doVBlankProcess();   /* a short pause */
        VDP_clearTextLine(11);
        hide_sprites();
    }
    return 0;
}
