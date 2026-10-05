/* beep.h - a simple tone on sound channel 1 of the Game Boy (a square
 * wave with a volume envelope). The sound registers NR10..NR52 sit at
 * 0xFF10..0xFF26. Tone frequency = 131072 / (2048 - x), x in NR13/NR14. */

static void beep_init(void)
{
    NR52_REG = 0x80;   /* sound on */
    NR50_REG = 0x77;   /* full volume, both speakers */
    NR51_REG = 0x11;   /* channel 1 to both speakers */
}

/* a short tone of freq Hz; the envelope makes it fade by itself */
static void beep(unsigned int freq)
{
    unsigned int x = 2048 - (unsigned int)(131072UL / freq);
    NR10_REG = 0x00;           /* no sweep */
    NR11_REG = 0x80;           /* duty 50% */
    NR12_REG = 0xF3;           /* volume 15, fading */
    NR13_REG = x & 0xFF;
    NR14_REG = 0x80 | (x >> 8);/* start */
}
