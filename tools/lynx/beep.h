/* beep.h - a simple tone on audio channel A of Mikey, the Lynx sound chip.
 * Each channel is a timer that clocks a shift register; with only bit 0
 * fed back the output bit flips on every tick, which gives a square wave.
 * Ticks come every 8 microseconds (clock select 3), so the tone frequency
 * is 1000000 / 8 / 2 / (reload + 1). */

static unsigned char beep_left;   /* frames of sound left */

static void beep_init(void)
{
    MIKEY.mstereo = 0;               /* all channels on both speakers */
    MIKEY.channel_a.volume = 0;
    MIKEY.channel_a.control = 0;
    beep_left = 0;
}

/* start a tone of freq Hz for len frames */
static void beep(unsigned int freq, unsigned char len)
{
    MIKEY.channel_a.control = 0;
    MIKEY.channel_a.feedback = 0x01;                        /* tap bit 0 */
    MIKEY.channel_a.shiftlo = 0x01;
    MIKEY.channel_a.other = 0;
    MIKEY.channel_a.reload = (unsigned char)(62500u / freq - 1);
    MIKEY.channel_a.volume = 0x40;
    MIKEY.channel_a.control = 0x18 | 3;   /* reload on, count on, 8 us ticks */
    beep_left = len;
}

/* call once a frame: turns the tone off when its time is up */
static void beep_tick(void)
{
    if (beep_left)
    {
        beep_left--;
        if (beep_left == 0)
        {
            MIKEY.channel_a.volume = 0;
            MIKEY.channel_a.control = 0;
        }
    }
}
