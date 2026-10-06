/* gm.h - screen, text, sprites and sound for the Gamate examples.
 * The functions are not static: each program is one file and uses only
 * some of them, and cc65 warns about unused static functions.
 * Part of the pico-class kit, see tools/gamate/README.md.
 *
 * Screen: 160x152 pixels, 4 shades, two bit planes of 8 pixels a byte
 * (the left pixel in bit 7); shade 3 is the darkest. The screen memory
 * is not in the address space of the processor: the program puts a byte
 * column (LCD_X, plane 2 has bit 7 set) and a line (LCD_Y) into the video
 * chip, then writes bytes to LCD_DATA, and the chip steps down a line
 * after each one. There are no sprites and no character mode.
 * Sound: a chip like the AY-3-8910 (three square-wave channels). */


#include <gamate.h>
#include <6502.h>
#include "font_gfx.h"

#define SCR_H 152                  /* screen height in pixels */
#define REG(a) (*(volatile unsigned char *)(a))

/* waitvsync() waits for the next timer interrupt of the BIOS, which comes
 * about 128 times a second; two of them make a frame of about 64 Hz */
#define SCR_FPS 64

void scr_wait(void)         /* wait for the next frame */
{
    waitvsync();
    waitvsync();
}

/* bytes of a w-byte, h-line block in plane 1 and plane 2: v1 and v2 */
void scr_fill(unsigned char xb, unsigned char y, unsigned char wb, unsigned char h,
                     unsigned char v1, unsigned char v2)
{
    unsigned char i, k;
    for (i = 0; i < wb; i++)
    {
        REG(LCD_X) = LCD_XPOS_PLANE1 + xb + i;
        REG(LCD_Y) = y;
        for (k = 0; k < h; k++) REG(LCD_DATA) = v1;
        REG(LCD_X) = LCD_XPOS_PLANE2 + xb + i;
        REG(LCD_Y) = y;
        for (k = 0; k < h; k++) REG(LCD_DATA) = v2;
    }
}

void scr_clear(void)        /* the whole screen to shade 0 */
{
    scr_fill(0, 0, 20, SCR_H, 0, 0);
}

void scr_init(void)
{
    REG(LCD_XPOS) = 0;
    REG(LCD_YPOS) = 0;
    REG(LCD_MODE) = LCD_MODE_INC_Y; /* step down a line after each byte */
    scr_clear();
}

/* text in a 20x19 grid of 8x8 characters; characters 32..95 only */
void scr_text(unsigned char col, unsigned char row, const char *s, unsigned char shade)
{
    unsigned char k, c, m1, m2;
    const unsigned char *g;
    m1 = (shade & 1) ? 0xFF : 0;
    m2 = (shade & 2) ? 0xFF : 0;
    while ((c = *s++) != 0 && col < 20)
    {
        if (c >= 'a' && c <= 'z') c -= 32;
        if (c < 32 || c > 95) c = '?';
        g = &font[(c - 32) * 8];
        REG(LCD_X) = LCD_XPOS_PLANE1 + col;
        REG(LCD_Y) = row * 8;
        for (k = 0; k < 8; k++) REG(LCD_DATA) = g[k] & m1;
        REG(LCD_X) = LCD_XPOS_PLANE2 + col;
        REG(LCD_Y) = row * 8;
        for (k = 0; k < 8; k++) REG(LCD_DATA) = g[k] & m2;
        col++;
    }
}

/* clear the rectangle x..x+w-1, y..y+h-1 (rounded out to whole bytes) */
void scr_erase(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    if (h) scr_fill(x >> 3, y, ((x + w - 1) >> 3) - (x >> 3) + 1, h, 0, 0);
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
        scr_erase(ox, oy, w, h);        /* no overlap: erase it all */
        return;
    }
    if (oy < ny) scr_erase(ox, oy, w, ny - oy);                 /* strip above */
    if (oy > ny) scr_erase(ox, ny + h, w, oy - ny);             /* strip below */
    top = oy > ny ? oy : ny;
    bot = (oy > ny ? ny : oy) + h;                              /* overlap rows */
    if (ox < nx) scr_erase(ox, top, nx - ox, bot - top);        /* strip left */
    if (ox > nx) scr_erase(nx + w, top, ox - nx, bot - top);    /* strip right */
}

/* copy a sprite made by mkgfx.py: w x h pixels, stored eight times
 * (shifted by 0..7 pixels), plane 1 then plane 2, w/8 + 1 columns each */
void scr_sprite(unsigned char x, unsigned char y, const unsigned char *spr,
                       unsigned char w, unsigned char h)
{
    unsigned char wb = (w >> 3) + 1, xb = x >> 3, i, k;
    const unsigned char *s = spr + (x & 7) * (2 * wb * h);
    for (i = 0; i < wb; i++)
    {
        REG(LCD_X) = LCD_XPOS_PLANE1 + xb + i;
        REG(LCD_Y) = y;
        for (k = 0; k < h; k++) REG(LCD_DATA) = *s++;
    }
    for (i = 0; i < wb; i++)
    {
        REG(LCD_X) = LCD_XPOS_PLANE2 + xb + i;
        REG(LCD_Y) = y;
        for (k = 0; k < h; k++) REG(LCD_DATA) = *s++;
    }
}

/* sound: a square wave on channel C, which goes to both speakers.
 * The tone is 1.1 MHz / 16 / period; register 7 switches channels on
 * (a 0 bit = on), register 10 is the volume of channel C (0..15). */
unsigned char beep_left;    /* frames of sound left */

void beep_init(void)
{
    REG(AUDIO_BASE + 7) = 0x3F;    /* all channels off */
    REG(AUDIO_BASE + 10) = 0;
    beep_left = 0;
}

void beep(unsigned int freq, unsigned char len)
{
    unsigned int period = 69230u / freq;
    REG(AUDIO_BASE + 4) = period & 0xFF;
    REG(AUDIO_BASE + 5) = period >> 8;
    REG(AUDIO_BASE + 10) = 12;
    REG(AUDIO_BASE + 7) = 0x3B;    /* tone C on, the rest off */
    beep_left = len;
}

void beep_tick(void)        /* once a frame: stop the tone in time */
{
    if (beep_left && --beep_left == 0)
    {
        REG(AUDIO_BASE + 10) = 0;
        REG(AUDIO_BASE + 7) = 0x3F;
    }
}
