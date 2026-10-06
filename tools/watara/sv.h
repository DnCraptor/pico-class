/* sv.h - screen, text, sprites and sound for the Watara Supervision
 * examples. Part of the pico-class kit, see tools/watara/README.md.
 *
 * Screen: 160x160 pixels, 4 shades; screen memory at 0x4000, 48 bytes a
 * line (40 are shown), 4 pixels a byte, the left pixel in the lowest two
 * bits, shade 3 is the darkest. There are no sprites and no character
 * mode: text and pictures are copied into the screen memory by the
 * program. Sound: two square-wave channels and a noise channel. */


#include <supervision.h>
#include "font_gfx.h"

#define SV_LINE 48                 /* bytes a screen line */
#define SCR_H   160                /* screen height in pixels */

/* the cc65 start-up code counts the NMI that comes once a frame
 * (sv_nmi_counter, declared in supervision.h) */
#define SV_FRAMES (*(volatile unsigned char *)&sv_nmi_counter)
#define SCR_FPS 60                 /* frames a second */

void scr_wait(void)          /* wait for the next frame */
{
    unsigned char n = SV_FRAMES;
    while (SV_FRAMES == n) ;
}

void scr_clear(void)         /* the whole screen to shade 0 */
{
    unsigned char *p = SV_VIDEO;
    unsigned int i;
    for (i = 0; i < 160u * SV_LINE; i++) *p++ = 0;
}


/* spread[shade][n]: 4 font pixels (bits 3..0, the left one in bit 3)
 * as one screen byte in that shade; filled by scr_init() */
static unsigned char spread[4][16];

void scr_make_spread(void)
{
    unsigned char sh, n, i, v;
    for (sh = 0; sh < 4; sh++)
        for (n = 0; n < 16; n++)
        {
            v = 0;
            for (i = 0; i < 4; i++)
                if (n & (8 >> i)) v |= sh << (2 * i);
            spread[sh][n] = v;
        }
}

void scr_init(void)
{
    SV_LCD.width = 160;
    SV_LCD.height = 160;
    SV_LCD.xpos = 0;
    SV_LCD.ypos = 0;
    scr_make_spread();
    scr_clear();
}

/* text in a 20x20 grid of 8x8 characters; characters 32..95 only */
void scr_text(unsigned char col, unsigned char row, const char *s, unsigned char shade)
{
    unsigned char k, c;
    const unsigned char *g;
    const unsigned char *t = spread[shade & 3];
    unsigned char *p;
    while ((c = *s++) != 0 && col < 20)
    {
        if (c >= 'a' && c <= 'z') c -= 32;
        if (c < 32 || c > 95) c = '?';
        g = &font[(c - 32) * 8];
        p = SV_VIDEO + row * 8 * SV_LINE + col * 2;
        for (k = 0; k < 8; k++, p += SV_LINE)
        {
            p[0] = t[g[k] >> 4];
            p[1] = t[g[k] & 15];
        }
        col++;
    }
}

/* clear the rectangle x..x+w-1, y..y+h-1 (rounded out to whole bytes) */
void scr_erase(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    unsigned char *p = SV_VIDEO + y * SV_LINE + (x >> 2);
    unsigned char n = ((x + w - 1) >> 2) - (x >> 2) + 1, i;
    for (; h; h--, p += SV_LINE)
        for (i = 0; i < n; i++) p[i] = 0;
}

/* a w x h picture moves from (ox,oy) to (nx,ny): erase only the parts of
 * the old place that the new picture will not cover. Erasing all of it
 * and then drawing would make the picture flicker. */
void scr_move(unsigned char ox, unsigned char oy, unsigned char nx, unsigned char ny,
                    unsigned char w, unsigned char h)
{
    unsigned char top, bot;
    if (ox + w <= nx || nx + w <= ox || oy + h <= ny || ny + h <= oy)
    {
        scr_erase(ox, oy, w, h);         /* no overlap: erase it all */
        return;
    }
    if (oy < ny) scr_erase(ox, oy, w, ny - oy);                  /* strip above */
    if (oy > ny) scr_erase(ox, ny + h, w, oy - ny);              /* strip below */
    top = oy > ny ? oy : ny;
    bot = (oy > ny ? ny : oy) + h;                              /* overlap rows */
    if (ox < nx) scr_erase(ox, top, nx - ox, bot - top);         /* strip left */
    if (ox > nx) scr_erase(nx + w, top, ox - nx, bot - top);     /* strip right */
}

/* copy a sprite made by mkgfx.py: w x h pixels, stored four times
 * (shifted by 0..3 pixels), w/4 + 1 bytes a line */
void scr_sprite(unsigned char x, unsigned char y, const unsigned char *spr,
                       unsigned char w, unsigned char h)
{
    unsigned char wb = (w >> 2) + 1;
    unsigned char *p = SV_VIDEO + y * SV_LINE + (x >> 2);
    const unsigned char *s = spr + (x & 3) * (wb * h);
    unsigned char i;
    for (; h; h--, p += SV_LINE)
        for (i = 0; i < wb; i++) p[i] = *s++;
}

/* sound: a square wave on both channels. The tone is
 * 4 MHz / 32 / (period + 1); control: bit 6 on, bits 5..4 the shape
 * (2 = half high, half low), bits 3..0 the volume. */
unsigned char beep_left;    /* frames of sound left */

void beep_init(void)
{
    SV_RIGHT.control = 0;
    SV_LEFT.control = 0;
    beep_left = 0;
}

void beep(unsigned int freq, unsigned char len)
{
    unsigned int period = 125000u / freq - 1;
    SV_RIGHT.delay = period;
    SV_LEFT.delay = period;
    SV_RIGHT.control = 0x40 | 0x20 | 0x0C;
    SV_LEFT.control = 0x40 | 0x20 | 0x0C;
    SV_RIGHT.timer = 0;
    SV_LEFT.timer = 0;
    beep_left = len;
}

void beep_tick(void)        /* once a frame: stop the tone in time */
{
    if (beep_left && --beep_left == 0)
    {
        SV_RIGHT.control = 0;
        SV_LEFT.control = 0;
    }
}
