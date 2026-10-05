/* beep.h - a simple tone on PSG channel 0 (PC Engine sound chip).
 * The PSG registers live at 0x0800..0x0809:
 *   0x0800 channel select, 0x0801 main volume, 0x0802/0x0803 period,
 *   0x0804 on/volume, 0x0805 balance, 0x0806 waveform (32 samples).
 * Tone frequency = 3579545 / 32 / period. */

int beep_left;  /* frames of sound left */

beep_init()
{
	int i;
	poke(0x0801, 0xFF);      /* main volume: left 15, right 15 */
	poke(0x0800, 0);         /* channel 0 */
	poke(0x0804, 0x00);      /* off, reset waveform index */
	for (i = 0; i < 32; i++) /* square wave: 16 high, 16 low */
		poke(0x0806, (i < 16) ? 31 : 0);
	poke(0x0805, 0xFF);      /* channel balance */
	beep_left = 0;
}

/* start a tone: period (bigger = lower) for len frames */
beep(period, len)
int period; int len;
{
	poke(0x0800, 0);
	poke(0x0802, period & 0xFF);
	poke(0x0803, (period >> 8) & 0x0F);
	poke(0x0804, 0x9F);      /* on, volume 31 */
	beep_left = len;
}

/* call once a frame: turns the tone off when its time is up */
beep_tick()
{
	if (beep_left) {
		beep_left--;
		if (beep_left == 0) {
			poke(0x0800, 0);
			poke(0x0804, 0x00);
		}
	}
}
