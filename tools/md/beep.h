/* beep.h - a simple tone on PSG channel 0 (the SN76489 sound chip of the
 * Mega Drive, the same one as in the Master System). The FM chip YM2612
 * makes the real music of Mega Drive games; for a beep the PSG is enough.
 * PSG_setEnvelope: 0 = loudest, 15 = silent. */

static u16 beep_left;  /* frames of sound left */

static void beep_init(void)
{
    PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
    beep_left = 0;
}

/* start a tone of freq Hz for len frames */
static void beep(u16 freq, u16 len)
{
    PSG_setFrequency(0, freq);
    PSG_setEnvelope(0, PSG_ENVELOPE_MAX);
    beep_left = len;
}

/* call once a frame: turns the tone off when its time is up */
static void beep_tick(void)
{
    if (beep_left)
    {
        beep_left--;
        if (beep_left == 0)
            PSG_setEnvelope(0, PSG_ENVELOPE_MIN);
    }
}
