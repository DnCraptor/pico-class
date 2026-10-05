/* catch.c - "Catch the stars": a small complete game.
 * Move the basket with LEFT/RIGHT, catch the falling stars.
 * A missed star costs a life; after 3 misses the game is over.
 * Part of the pico-class kit, see tools/pce/README.md. */

#include "huc.h"
#include "beep.h"
#include "catch_gfx.h"    /* basket_spr, star_spr: made by mksprites.py */

#define BASKET_VRAM 0x5000
#define STAR_VRAM   0x5040
#define NSTARS      3     /* stars falling at the same time */

const int basket_pal[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0B0, 0x1B4, 0, 0, 0, 0
};
const int star_pal[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0x0F8, 0x1F8, 0, 0, 0, 0, 0, 0
};

int x;                    /* basket position */
int sx[NSTARS], sy[NSTARS], speed[NSTARS];
int score, lives, best, i, pad;

/* put star number n back to the top at a random place */
new_star(n)
int n;
{
	sx[n] = (rand() & 255) % 224 + 8;
	sy[n] = 16 - (rand() & 63);           /* a little above the screen */
	speed[n] = 1 + (score >> 3);          /* faster as the score grows */
	if (speed[n] > 4) speed[n] = 4;
}

show_status()
{
	put_string("Score", 1, 1);   put_number(score, 4, 7, 1);
	put_string("Lives", 13, 1);  put_number(lives, 1, 19, 1);
	put_string("Best", 23, 1);   put_number(best, 4, 27, 1);
}

draw_sprites()
{
	spr_set(0);
	spr_x(x); spr_y(200);
	spr_pattern(BASKET_VRAM);
	spr_ctrl(FLIP_MAS | SIZE_MAS, NO_FLIP | SZ_16x16);
	spr_pal(0); spr_pri(1);
	for (i = 0; i < NSTARS; i++) {
		spr_set(i + 1);
		spr_x(sx[i]); spr_y(sy[i]);
		spr_pattern(STAR_VRAM);
		spr_ctrl(FLIP_MAS | SIZE_MAS, NO_FLIP | SZ_16x16);
		spr_pal(1); spr_pri(1);
	}
}

play()
{
	score = 0; lives = 3; x = 120;
	for (i = 0; i < NSTARS; i++) {
		new_star(i);
		sy[i] -= i * 60;                  /* do not start all at once */
	}
	put_string("                              ", 1, 13);
	put_string("                              ", 1, 15);
	show_status();

	while (lives > 0) {
		pad = joy(0);
		if ((pad & JOY_LEFT)  && x > 0)   x -= 3;
		if ((pad & JOY_RIGHT) && x < 240) x += 3;

		for (i = 0; i < NSTARS; i++) {
			sy[i] += speed[i];
			/* caught: the star reached the basket and they overlap */
			if (sy[i] >= 190 && sy[i] <= 204 && abs(sx[i] - x) < 14) {
				score++;
				if (score > best) best = score;
				beep(100, 6);
				new_star(i);
				show_status();
			}
			/* missed: the star fell off the bottom */
			if (sy[i] > 224) {
				lives--;
				beep(700, 20);
				new_star(i);
				show_status();
			}
		}

		draw_sprites();
		vsync();
		satb_update();
		beep_tick();
	}
}

main()
{
	disp_off();
	cls();
	set_color(0, 0x001);          /* background: night */
	set_color(1, 0x1FF);          /* text: white */
	set_font_color(1, 0);
	load_default_font();
	beep_init();
	load_sprites(BASKET_VRAM, basket_spr, 1);
	load_sprites(STAR_VRAM, star_spr, 1);
	load_palette(16, basket_pal, 1);
	load_palette(17, star_pal, 1);
	init_satb();
	disp_on();

	best = 0;
	for (;;) {
		put_string("CATCH THE STARS", 8, 13);
		put_string("Press RUN to start", 6, 15);
		/* wait for RUN; calling rand() meanwhile makes every game new */
		while (!(joytrg(0) & JOY_RUN)) {
			rand();
			vsync();
		}
		play();
		put_string("GAME OVER", 11, 11);
		for (i = 0; i < 90; i++) vsync();   /* a short pause */
		put_string("         ", 11, 11);
		for (i = 0; i <= NSTARS; i++) spr_hide(i);
		satb_update();
	}
}
