/* sprite.c - hardware sprites: a robot you move with the joypad and a
 * ball that moves by itself and bounces off the screen edges.
 * Part of the pico-class kit, see tools/pce/README.md. */

#include "huc.h"
#include "beep.h"
#include "sprite_gfx.h"   /* robot_spr, ball_spr: made by mksprites.py */

#define ROBOT_VRAM 0x5000 /* where the pictures go in video memory */
#define BALL_VRAM  0x5040 /* (each 16x16 picture takes 0x40 words)  */

/* sprite palettes, 9-bit GRB colours (G<<6 | R<<3 | B); colour 0 is
 * transparent */
const int robot_pal[16] = {
	0, 0x038, 0x16D, 0x1FF, 0x007, 0x038, 0x0B6, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const int robot_pal2[16] = {
	0, 0x1F8, 0x0F8, 0x1FF, 0x1C0, 0x038, 0x03F, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const int ball_pal[16] = {
	0, 0, 0, 0, 0, 0, 0, 0x1C0, 0x1FF, 0, 0, 0, 0, 0, 0, 0
};

int x, y;        /* robot position */
int bx, by;      /* ball position */
int dx, dy;      /* ball speed: how far it moves each frame */
int pad, colour, score;

main()
{
	disp_off();
	cls();
	set_color(0, 0x040);          /* background: dark green */
	set_color(1, 0x1FF);          /* text: white */
	set_font_color(1, 0);
	load_default_font();
	beep_init();

	load_sprites(ROBOT_VRAM, robot_spr, 1);
	load_sprites(BALL_VRAM, ball_spr, 1);
	load_palette(16, robot_pal, 1);  /* palettes 16..31 are for sprites */
	load_palette(17, ball_pal, 1);
	init_satb();

	put_string("Move the robot with the arrows", 1, 1);
	put_string("I: colour   Touch the ball.", 1, 2);
	put_string("Score:", 1, 26);
	disp_on();

	x = 120; y = 150;
	bx = 20; by = 40; dx = 2; dy = 1;
	colour = 0; score = 0;

	for (;;) {
		/* the joypad moves the robot */
		pad = joy(0);
		if ((pad & JOY_LEFT)  && x > 0)   x -= 2;
		if ((pad & JOY_RIGHT) && x < 240) x += 2;
		if ((pad & JOY_UP)    && y > 24)  y -= 2;
		if ((pad & JOY_DOWN)  && y < 192) y += 2;

		/* button I: the other palette */
		if (joytrg(0) & JOY_I) {
			colour = 1 - colour;
			if (colour) load_palette(16, robot_pal2, 1);
			else        load_palette(16, robot_pal, 1);
		}

		/* the ball moves by itself and bounces */
		bx += dx;
		by += dy;
		if (bx <= 0 || bx >= 240) { dx = -dx; beep(400, 3); }
		if (by <= 24 || by >= 192) { dy = -dy; beep(400, 3); }

		/* do the robot and the ball touch? (both are 16x16) */
		if (abs(bx - x) < 14 && abs(by - y) < 14) {
			score++;
			put_number(score, 4, 8, 26);
			beep(120, 8);
			/* throw the ball back up */
			by = 30;
			bx = (rand() & 127) + 50;
		}

		/* tell the video chip where the sprites are */
		spr_set(0);
		spr_x(x);
		spr_y(y);
		spr_pattern(ROBOT_VRAM);
		spr_ctrl(FLIP_MAS | SIZE_MAS, NO_FLIP | SZ_16x16);
		spr_pal(0);                 /* = palette 16 */
		spr_pri(1);

		spr_set(1);
		spr_x(bx);
		spr_y(by);
		spr_pattern(BALL_VRAM);
		spr_ctrl(FLIP_MAS | SIZE_MAS, NO_FLIP | SZ_16x16);
		spr_pal(1);                 /* = palette 17 */
		spr_pri(1);

		vsync();
		satb_update();
		beep_tick();
	}
}
