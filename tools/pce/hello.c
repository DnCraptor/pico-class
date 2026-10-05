/* hello.c - first PC Engine program: text, colours, joypad, sound.
 * Part of the pico-class kit, see tools/pce/README.md. */

#include "huc.h"
#include "beep.h"

/* text colours to cycle through, 9-bit GRB: G<<6 | R<<3 | B, 0..7 each */
const int rainbow[8] = {
	0x1FF, 0x1F8, 0x1C7, 0x1C0, 0x0F8, 0x038, 0x03F, 0x0FF
};

int frames, seconds, pad, c;

main()
{
	disp_off();
	cls();
	set_color(0, 0x002);            /* colour 0, the background: dark blue */
	set_color(1, 0x1FF);            /* colour 1, the text: white */
	set_font_color(1, 0);
	set_font_pal(0);
	load_default_font();
	beep_init();

	put_string("HELLO, PC ENGINE", 7, 3);
	put_string("This program is written in C", 2, 6);
	put_string("and compiled with HuC.", 2, 7);
	put_string("Seconds:", 2, 10);
	put_string("Press the buttons:", 2, 13);
	put_string("I and II make a sound.", 2, 20);
	disp_on();

	frames = 0;
	seconds = 0;
	c = 0;
	for (;;) {
		vsync();
		beep_tick();

		/* a clock: 60 frames = 1 second */
		frames++;
		if (frames == 60) {
			frames = 0;
			seconds++;
			put_number(seconds, 5, 11, 10);
			/* the text changes colour every second */
			c = (c + 1) & 7;
			set_color(1, rainbow[c]);
		}

		/* show which buttons are held */
		pad = joy(0);
		put_string((pad & JOY_UP)    ? "UP"    : "  ",    2, 15);
		put_string((pad & JOY_DOWN)  ? "DOWN"  : "    ",  5, 15);
		put_string((pad & JOY_LEFT)  ? "LEFT"  : "    ", 10, 15);
		put_string((pad & JOY_RIGHT) ? "RIGHT" : "     ", 15, 15);
		put_string((pad & JOY_I)     ? "I"     : " ",    21, 15);
		put_string((pad & JOY_II)    ? "II"    : "  ",   23, 15);
		put_string((pad & JOY_SEL)   ? "SELECT": "      ", 2, 17);
		put_string((pad & JOY_RUN)   ? "RUN"   : "   ",   9, 17);

		/* a new press (not a hold) of I or II: a beep */
		pad = joytrg(0);
		if (pad & JOY_I)  beep(254, 10);   /* about 440 Hz */
		if (pad & JOY_II) beep(127, 10);   /* about 880 Hz */
	}
}
