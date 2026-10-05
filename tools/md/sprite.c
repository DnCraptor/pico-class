/* sprite.c - hardware sprites: a robot you move with the joypad and a
 * ball that moves by itself and bounces off the screen edges.
 * Part of the pico-class kit, see tools/md/README.md. */

#include <genesis.h>
#include "beep.h"
#include "sprite_gfx.h"   /* robot_spr, ball_spr: made by mksprites.py */

#define ROBOT_TILE TILE_USER_INDEX        /* where the pictures go in video memory */
#define BALL_TILE  (TILE_USER_INDEX + 4)  /* (each 16x16 picture takes 4 tiles)   */

/* sprite palettes, RGB 0..7 each; colour 0 is transparent */
static const u16 robot_pal[16] = {
    0, RGB3_3_3_TO_VDPCOLOR(7, 0, 0), RGB3_3_3_TO_VDPCOLOR(5, 5, 5), RGB3_3_3_TO_VDPCOLOR(7, 7, 7),
    RGB3_3_3_TO_VDPCOLOR(0, 0, 7), RGB3_3_3_TO_VDPCOLOR(7, 0, 0), RGB3_3_3_TO_VDPCOLOR(6, 2, 6)
};
static const u16 robot_pal2[16] = {
    0, RGB3_3_3_TO_VDPCOLOR(7, 7, 0), RGB3_3_3_TO_VDPCOLOR(7, 3, 0), RGB3_3_3_TO_VDPCOLOR(7, 7, 7),
    RGB3_3_3_TO_VDPCOLOR(0, 7, 0), RGB3_3_3_TO_VDPCOLOR(7, 0, 0), RGB3_3_3_TO_VDPCOLOR(7, 0, 7)
};
static const u16 ball_pal[16] = {
    0, 0, 0, 0, 0, 0, 0, RGB3_3_3_TO_VDPCOLOR(0, 7, 0), RGB3_3_3_TO_VDPCOLOR(7, 7, 7)
};

int main(bool hardReset)
{
    char buf[8];
    s16 x = 152, y = 150;        /* robot position */
    s16 bx = 20, by = 40;        /* ball position */
    s16 dx = 2, dy = 1;          /* ball speed: how far it moves each frame */
    u16 pad, old = 0, colour = 0, score = 0;

    PAL_setColor(0, RGB3_3_3_TO_VDPCOLOR(0, 2, 0));   /* background: dark green */
    PAL_setColor(15, RGB3_3_3_TO_VDPCOLOR(7, 7, 7));  /* text: white */
    beep_init();

    VDP_loadTileData(robot_spr, ROBOT_TILE, 4, DMA);
    VDP_loadTileData(ball_spr, BALL_TILE, 4, DMA);
    PAL_setColors(16, robot_pal, 16, DMA);            /* palette 1: the robot */
    PAL_setColors(32, ball_pal, 16, DMA);             /* palette 2: the ball */

    VDP_drawText("Move the robot with the arrows", 1, 1);
    VDP_drawText("A: colour   Touch the ball!", 1, 2);
    VDP_drawText("Score:", 1, 26);

    while (TRUE)
    {
        /* the joypad moves the robot */
        pad = JOY_readJoypad(JOY_1);
        if ((pad & BUTTON_LEFT) && x > 0) x -= 2;
        if ((pad & BUTTON_RIGHT) && x < 304) x += 2;
        if ((pad & BUTTON_UP) && y > 24) y -= 2;
        if ((pad & BUTTON_DOWN) && y < 192) y += 2;

        /* button A: the other palette */
        if ((pad & ~old) & BUTTON_A)
        {
            colour = 1 - colour;
            PAL_setColors(16, colour ? robot_pal2 : robot_pal, 16, DMA);
        }
        old = pad;

        /* the ball moves by itself and bounces */
        bx += dx;
        by += dy;
        if (bx <= 0 || bx >= 304) { dx = -dx; beep(1000, 3); }
        if (by <= 24 || by >= 192) { dy = -dy; beep(1000, 3); }

        /* do the robot and the ball touch? (both are 16x16) */
        if (abs(bx - x) < 14 && abs(by - y) < 14)
        {
            score++;
            uintToStr(score, buf, 4);
            VDP_drawText(buf, 8, 26);
            beep(1500, 8);
            /* throw the ball back up */
            by = 30;
            bx = (random() & 127) + 80;
        }

        /* tell the video chip where the sprites are: sprite 0 links to 1 */
        VDP_setSpriteFull(0, x, y, SPRITE_SIZE(2, 2),
                          TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, ROBOT_TILE), 1);
        VDP_setSpriteFull(1, bx, by, SPRITE_SIZE(2, 2),
                          TILE_ATTR_FULL(PAL2, 1, FALSE, FALSE, BALL_TILE), 0);
        VDP_updateSprites(2, DMA_QUEUE);

        beep_tick();
        SYS_doVBlankProcess();
    }
    return 0;
}
