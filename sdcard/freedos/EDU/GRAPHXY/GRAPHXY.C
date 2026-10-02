//Last updated 1e/9/99
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <time.h>
#include <alloc.h>
#include "c:\BorlandC\myfiles\graphio.h"

#define GRID 180

#define FORMULA(vals, x) (vals.a * pow(x,vals.pow) + vals.b * (x) + vals.c)
#define QUADRATIC_POS(vals) ((-vals.b) + sqrt((vals.b*vals.b) - (4 * vals.a * vals.c))) / (2 * vals.a)
#define QUADRATIC_NEG(vals) ((-vals.b) - sqrt((vals.b*vals.b) - (4 * vals.a * vals.c))) / (2 * vals.a)
//for draw_equation() function
#define IMAGINARY 12345
#define HIGH_POWER 54321   //power higher than 2
#define NO_EQUATION 12346
//for options()
#define MAXSTR 12
#define ROWMAX 2
#define COLMAX 1
#define SPACE 1.7

#define F1 ';'
#define F5 '?'

struct coord
{
	float x, y;
};

struct box
{
	struct coord top;
	struct coord bot;
};

struct values
{
	float a, pow, b, c;
};
void main(void);
void start(void);
void options(int zoom, struct coord mid);
struct box draw_options(char *com[COLMAX+1][ROWMAX+1]);
struct coord draw_grid(float zoom, struct coord center);
struct coord options_select(struct box box, char *opt[COLMAX+1][ROWMAX+1]);
void options_quick_help(char *opt);
void number_grid(struct coord center, int zoom);
struct values get_values(char *put);
void draw_graph(struct values vals, struct coord center, float zoom, int color);
void draw_dots(struct values vals, struct coord center, float zoom, int color);
int zoom_procedure(int zoom, struct coord mid);
int zoom_graph(int zoom);
void *draw_zoom(void);
void zoom_number(int zoom);
void zoom_sample(int zoom, struct box box);
void get_equation(char *word, int bgcolor);
void change_font_size(char *word, char *buffer);
struct textsettingstype defsize(void);
struct values add_graph(struct coord mid, float zoom);
int select_equations(void);
void drawYs(void);
char *equations(int eqnum);
void *get_draw_equations(int eqnum, char *equation);
void graphing_procedure(int zoom, struct coord mid);
void clear_graph(struct coord mid);
void graph_all(struct coord mid, int zoom);
void root_procedure(void);
float *get_roots(struct values vals);
void draw_roots(float root[], int eqnum);
void print_values_procedure(void);
struct box print_values_draw(struct box box, int eqnum);
void print_values(struct box box, struct values vals, int eqnum);
void print_xy(struct coord xloc, float x, struct coord yloc, float y, int color);
void about(void);
void quick_help_draw(void);
void quick_help(char *string);
void print_all_equations(void);
void help_procedure(void);
void draw_help(void);
char *load_file(char *filename);
int filesize(FILE *stream);
int help_select(void);
void help_branch(int opt);
void help_topic_display(char *title, char *filename, int txtsize);
void wordwrap(char *str, int min, int max, int ycoord);
void stars(int num);
void exit_all(void);

void main()
{
	int GraphDriver = VGA, GraphMode = VGAHI;

	initgraph(&GraphDriver, &GraphMode, "");

	start();

	getch();
	closegraph();
}

void start()
{
	float zoom = 20;
	struct coord mid = {440, 210};

	cleardevice();
	setbkcolor(0);
	draw_grid(zoom, mid);  //it returns the center point
	drawYs();
	about();
	quick_help_draw();
	print_all_equations();
	graph_all(mid, zoom);
	options(zoom, mid);
}

void options(int zoom, struct coord mid)
{
	char option;
	struct box box;
	struct coord pos;
	int col, row;
	char *opt[COLMAX+1][ROWMAX+1] = {{"New Graph", "Graph All", "Clear Graph"},
												 {"Roots", "Zoom", "Values"}};

	settextstyle(2, 0, 6);

	box = draw_options(opt);

	while(1) //the loop of options...
	{
		settextstyle(2, 0, 6);
		pos = options_select(box, opt);
		row = pos.x;
		col = pos.y;

		if(strcmp(opt[col][row], "New Graph") == 0)
		{
			graphing_procedure(zoom, mid);
		}
		if(strcmp(opt[col][row], "Graph All") == 0)
		{
			graph_all(mid, zoom);
		}
		if(strcmp(opt[col][row], "Clear Graph") == 0)
		{
			clear_graph(mid);
			draw_grid(zoom, mid);
			number_grid(mid, zoom);
		}
		if(strcmp(opt[col][row], "Roots") == 0)
		{
			root_procedure();
		}
		if(strcmp(opt[col][row], "Zoom") == 0)
		{
			zoom = zoom_procedure(zoom, mid);
		}
		if(strcmp(opt[col][row], "Values") == 0)
		{
			print_values_procedure();
		}
	}
}

struct box draw_options(char *com[COLMAX+1][ROWMAX+1])
{
	struct box box, cell;
	int col, row, bgcolor = 4, color = 14;
	struct coord txtloc;

	setlinestyle(SOLID_LINE, 0, 1);
	setfillstyle(SOLID_FILL, bgcolor);
	setcolor(15);

	box.top.x = 10;
	box.top.y = 250;
	box.bot.x = box.top.x + MAXSTR * 2 * textwidth("X");
	box.bot.y = box.top.y + 3 * SPACE * textheight("X");

	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);
	setcolor(8);
	rectangle(box.top.x+1, box.top.y+1, box.bot.x+1, box.bot.y+1);
	setcolor(15);
	rectangle(box.top.x, box.top.y, box.bot.x, box.bot.y);
	for(col = 0; col <= COLMAX; col++)
	{
		for(row = 0; row <= ROWMAX; row++)
		{
			cell.top.x = box.top.x + col * MAXSTR * textwidth("X");
			cell.bot.x = box.top.x + (col + 1) * MAXSTR * textwidth("X");
			cell.top.y = box.top.y + (textheight("X") * SPACE * row);
			cell.bot.y = box.top.y + (textheight("X") * SPACE * (row+1));
			setcolor(color);
			rectangle(cell.top.x, cell.top.y, cell.bot.x, cell.bot.y);

			txtloc.x = cell.top.x + abs(cell.bot.x - cell.top.x)/2 - textwidth(com[col][row])/2 + 1;
			txtloc.y = cell.top.y + 1;
			setcolor(0);                                       //shadows of text
			outtextxy(txtloc.x + 1, txtloc.y + 1, com[col][row]);
			setcolor(6);
			outtextxy(txtloc.x, txtloc.y, com[col][row]);
			setcolor(color);
			outtextxy(txtloc.x - 1, txtloc.y - 1, com[col][row]);
		}
	}
	row = 0;
	col = 0;
	cell.top.x = box.top.x + col * MAXSTR * textwidth("X");
	cell.bot.x = box.top.x + (col + 1) * MAXSTR * textwidth("X");
	cell.top.y = box.top.y + (textheight("X") * SPACE * row);
	cell.bot.y = box.top.y + (textheight("X") * SPACE * (row+1));

	return cell;
}
struct coord options_select(struct box box, char *opt[COLMAX+1][ROWMAX+1])
{
	struct time t;
	struct box cell = box;
	int time1, time2, bgcolor = 4;
	int blinkspd = 20;
	char c = ' ';
	static int row = 0, col = 0;
	struct coord position;

	options_quick_help(opt[col][row]);

	setlinestyle(SOLID_LINE, 0, 1);

	cell.top.x = box.top.x + col * MAXSTR * textwidth("X")+2;
	cell.bot.x = box.top.x + (col + 1) * MAXSTR * textwidth("X")-2;
	cell.top.y = box.top.y + (textheight("X") * SPACE * row)+2;
	cell.bot.y = box.top.y + (textheight("X") * SPACE * (row+1))-2;

	gettime(&t);
	time1 = (t.ti_sec * 100) + t.ti_hund;

	while(1)
	{
		if(kbhit())	c = getch();
		if(c == 27) exit_all();
		if(c == 13)
		{
			position.x = row;
			position.y = col;

			return position;
		}
		if(c == NULL)
		{
			c = getch();

			if(c == F5)  // If F5 key, refresh
			{
				start();
			}
			if(c == F1)
			{
				help_procedure();
				start();
			}
			if(c == 'H') row--;
			if(c == 'P') row++;
			if(c == 'M') col++;
			if(c == 'K') col--;

			if(row > ROWMAX) row = ROWMAX;
			else
			if(row < 0) row = 0;
			else
			if(col > COLMAX) col = COLMAX;
			else
			if(col < 0) col = 0;
			else
			{
				sound(750); delay(50); nosound();
				setcolor(bgcolor);
				rectangle(cell.top.x, cell.top.y, cell.bot.x, cell.bot.y);
				rectangle(cell.top.x+1, cell.top.y+1, cell.bot.x-1, cell.bot.y-1);
				cell.top.x = box.top.x + col * MAXSTR * textwidth("X")+2;
				cell.bot.x = box.top.x + (col + 1) * MAXSTR * textwidth("X")-2;
				cell.top.y = box.top.y + (textheight("X") * SPACE * row)+2;
				cell.bot.y = box.top.y + (textheight("X") * SPACE * (row+1))-2;

				options_quick_help(opt[col][row]);
			}
		}
		gettime(&t);
		time2 = (t.ti_sec * 100) + t.ti_hund;
		if(abs(time1 - time2) > blinkspd/2)
		{
			if(abs(time1 - time2) > blinkspd * 2)
			{
				gettime(&t);
				time1 = (t.ti_sec * 100) + t.ti_hund;
			}
			setcolor(1);
			rectangle(cell.top.x, cell.top.y, cell.bot.x, cell.bot.y);
			rectangle(cell.top.x+1, cell.top.y+1, cell.bot.x-1, cell.bot.y-1);
		}
		if(abs(time1 - time2) < blinkspd/2)
			{
				setcolor(bgcolor);
				rectangle(cell.top.x, cell.top.y, cell.bot.x, cell.bot.y);
				rectangle(cell.top.x+1, cell.top.y+1, cell.bot.x-1, cell.bot.y-1);
			}
	}
}

void options_quick_help(char *opt)
{
		struct textsettingstype original_font;

		gettextsettings(&original_font);

		if(strcmp(opt, "New Graph") == 0)
		{
			quick_help("Press here to Enter and Graph a New equation or Edit an existing one.");
		}
		if(strcmp(opt, "Graph All") == 0)
		{
			quick_help("Press here to Graph All the entered equations.");
		}
		if(strcmp(opt, "Clear Graph") == 0)
		{
			quick_help("Press here to Clear the Graph.");
		}
		if(strcmp(opt, "Roots") == 0)
		{
			quick_help("Press here to see the Root(s) of an equation.");
		}
		if(strcmp(opt, "Zoom") == 0)
		{
			quick_help("Press here to Zoom in/out. Makes the grid smaller or larger.");
		}
		if(strcmp(opt, "Values") == 0)
		{
			quick_help("Press here to view the Values, plotting points of an equation.");
		}

		//set back the font
		settextstyle(original_font.font, original_font.direction, original_font.charsize);
}

struct coord draw_grid(float zoom, struct coord center)
{
	int hor = 0, ver = 0;
	struct box box;

	setlinestyle(0,0,1);

	//Declare all values for graph
	box.top.x = center.x - GRID;
	box.top.y = center.y - GRID;
	box.bot.x = center.x + GRID;
	box.bot.y = center.y + GRID;

	setcolor(7);

	//Draw horizontal grid lines
	for (hor = 0; hor*zoom <=  abs(box.bot.y-box.top.y); hor++)
	{
		line(box.top.x, box.top.y+hor*zoom, box.bot.x, box.top.y+hor*zoom);
	}

	//draw vertival grid lines
	for (ver = 0; ver*zoom <= abs(box.bot.x-box.top.x); ver++)
	{
		line(box.top.x+ver*zoom, box.top.y, box.top.x+ver*zoom, box.bot.y);
	}

	center.x = hor / 2 * zoom + box.top.x; // hor - 1 because it is 1 more
	center.y = ver / 2 * zoom + box.top.y;

	setcolor(15);
	line(center.x, box.top.y, center.x, box.bot.y); //the axises
	line(box.top.x, center.y, box.bot.x, center.y);

	setcolor(15);
	//rectangle(box.top.x, box.top.y, box.bot.x, box.bot.y);
	//rectangle(box.top.x-1, box.top.y-1, box.bot.x+1, box.bot.y+1);
	settextstyle(5, HORIZ_DIR , 2);
	outtextxy(center.x-3, box.top.y-35, "y");
	outtextxy(box.top.x-15, center.y-20,"x");
	//function for numbering the grid
	number_grid(center, zoom);

	return center; //returns the center point
}

void number_grid(struct coord center, int zoom)
{
	int number = 0, font_size;
	char num_str[5];
	struct coord locate;

	if(zoom < 10) return;
	else if(zoom >= 10 && zoom <= 19) font_size = 1;
	else if(zoom >= 20) font_size = 2;

	settextstyle(SMALL_FONT, HORIZ_DIR, font_size);

	locate.x = center.x - GRID + 2; //starting
	locate.y = center.y;           //points

	for (number = -(GRID / zoom); number < (GRID / zoom); number++)
	{
		itoa(number, num_str, 10);
		setcolor(10);
		outtextxy(locate.x, locate.y, num_str);

		locate.x = locate.x + zoom;
	}

	locate.x = center.x + 2;
	locate.y = center.y - GRID;

	for (number =(GRID / zoom); number > -(GRID / zoom); number--)
	{
		itoa(number, num_str, 10);
		setcolor(10);
		outtextxy(locate.x, locate.y, num_str);

		locate.y = locate.y + zoom;
	}
}

struct values get_values(char *put)
{

	struct values vals = {0.0,1,0.0,0.0};   //vals.pow is 1 because pow cannot be 0
	int putn = 0, bufn = 0;
	char buf[10];

	buf[0] = '\0';

	while(put[putn] != '\0')
	{
		if (put[putn] == '+')
		{
			putn++;
			continue;
		}
		while(isdigit(put[putn]) || (put[putn] == '-' && bufn == 0) || put[putn] == '.')
		{
			buf[bufn++] = put[putn++];
		}
		buf[bufn] = '\0';
		if(strlen(buf) > 0 && isdigit(buf[bufn-1]))
		{
			if(put[putn-strlen(buf)-1] == tolower('x') && isdigit(buf[0]))
			{
				vals.pow = atof(buf);
				bufn = 0;
				buf[bufn] = '\0';
			}
			if(put[putn] == tolower('x'))
			{
				if(isdigit(put[putn+1]))
				{
					vals.a = atof(buf);
				}
				else
				{
					vals.b = atof(buf);
				}
				bufn = 0;
				buf[bufn] = '\0';
				putn++; //skip x
			}
			if(strlen(buf) > 0)
			{
				vals.c = atof(buf);
				bufn = 0;
				buf[bufn] = '\0';
			}
		}
		//IF X IS ALONE...
		if(put[putn] == 'x')   //if x alone
		{
			if(isdigit(put[putn+1]))
			{
				buf[bufn++] = '1';
				buf[bufn] = '\0';
				vals.a = atof(buf);
			}
				else
				{
					buf[bufn++] = '1';
					buf[bufn] = '\0';
					vals.b = atof(buf);
				}
				bufn = 0;
				buf[bufn] = '\0';
				putn++; //skip x
		 }
	}
	return vals;
}

void draw_graph(struct values vals, struct coord center, float zoom, int color)
{
	float x, y, yy;
	float step = .01 / zoom; //using zoom to optimize the detai for each zoom

	quick_help("Graphing...");
	for (x=-GRID / zoom; x < GRID / zoom; x = x + step)
	{
		y = -FORMULA(vals, x);
		yy = -FORMULA(vals, x + step);

		if (y * zoom < -GRID || yy * zoom <-GRID || y * zoom > GRID || yy * zoom> GRID)
		continue;

		setcolor(color);
		setlinestyle(0,0,3);
		line(x * zoom + center.x, y * zoom + center.y, (x + step) * zoom + center.x, yy * zoom + center.y);
	}
	color--;
}

void draw_dots(struct values vals, struct coord center, float zoom, int color)
{
	struct coord dot;

	quick_help("Plotting points...");
	for (dot.x = -(GRID/zoom); dot.x <= (GRID/zoom); dot.x++)
	{
		dot.y = -FORMULA(vals, dot.x);


		if (dot.y * zoom < -GRID || dot.y * zoom > GRID)
			continue;

		setcolor(color);
		setlinestyle(0,0,3);
		if(dot.y == (int)dot.y) //if dot.y is an integer...
		{
			circle(dot.x * zoom + center.x, dot.y * zoom + center.y, 1);
		}
	}
}

int zoom_procedure(int zoom, struct coord mid)
{
	int zoom_original = zoom;

	zoom = zoom_graph(zoom);

	if(zoom_original != zoom) //if zoom is changed
	{
		clear_graph(mid);
		draw_grid(zoom, mid);
		graph_all(mid, zoom);
	}

	return zoom;
}

int zoom_graph(int zoom)
{
	void *backimage;
	char c;
	int ctr, zoom_original = zoom;
	struct box box = {{250, 210}, {300, 260}};

	backimage = draw_zoom();

	zoom_sample(zoom, box);

	zoom_number(zoom);

	quick_help("Use Up/Down Arrow Keys to zoom In/Out. ESC to cancel, ENTER to apply change.");

	while(1)
	{
		c = getch();
		if(c == NULL) c = getch(); //if it is an arrow key, next char in buffer
		if(c == 13) break;  //If Enter
		if(c == 27)  //If ESC, cancel
		{
			zoom = zoom_original;
			break;
		}
		if(c == '+' || c == 'H')  //if '+' or up arr key
		{
			for (ctr = zoom+1; ctr <= GRID; ctr++) //+1 so ctr is different from zoom's current value which is a pruduct of GRID
			{
				if ((GRID%ctr) == 0)
				{
					zoom = ctr;
					zoom_sample(zoom, box);
					zoom_number(zoom);
					break;
				}
			}
		}
		if(c == '-' || c == 'P')  //If '-' or down arrow key...
		{
			for (ctr = zoom-1; ctr > 0; ctr--) //-+1 so ctr is different from zoom's current value which is a pruduct of GRID
			{
				if ((GRID%ctr) == 0)
				{
					zoom = ctr;
					zoom_sample(zoom, box);
					zoom_number(zoom);
					break;
				}
			}
		}
	}

	//put the background back...
	putimage(200, 200, backimage, 0);
	free(backimage);

	return zoom;
}

void *draw_zoom()
{
	struct box box = {{200, 200}, {350, 300}};
	void *backimage;
	if ((backimage =
		malloc(imagesize(box.top.x, box.top.y, box.bot.x, box.bot.y)))
		== NULL) exit(1);


	getimage(box.top.x, box.top.y, box.bot.x, box.bot.y, backimage);

	setfillstyle(1, 2);
	setlinestyle(SOLID_LINE,1,1);
	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);
	setcolor(4);
	rectangle(box.top.x, box.top.y, box.bot.x, box.bot.y);

	return backimage;
}

void zoom_number(int zoom)
{
	int bgcolor = 2, color = 1;
	char zoom_str[4];
	static char str_final[20];
	int user_zoom = GRID * 2 / zoom;
	static int prev_user_zoom = 0;
	struct coord txt_coord = {230, 275};

	settextstyle(0,0,0);

	if (prev_user_zoom) //if first time, nothing to erase. Otherwise...
	{
		itoa(prev_user_zoom, zoom_str, 10);
		setcolor(bgcolor);
		outtextxy(txt_coord.x - textwidth(zoom_str), txt_coord.y, str_final);
	}
	itoa(user_zoom, zoom_str, 10);

	strcpy(str_final, "GRID ");
	strcat(str_final, zoom_str);
	strcat(str_final, "x");
	strcat(str_final, zoom_str);
	strcat(str_final, " UNITS");

	setcolor(color);

	outtextxy(txt_coord.x - textwidth(zoom_str), txt_coord.y, str_final);
	prev_user_zoom = user_zoom;
}

void zoom_sample(int zoom, struct box box)
{
	int ctr_hor, ctr_ver, color = 7, bgcolor = 0;

	setfillstyle(SOLID_FILL, bgcolor);
	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);

	setlinestyle(SOLID_LINE, 1, 1);
	setcolor(color);
	rectangle(box.top.x, box.top.y, box.bot.x, box.bot.y);

	for(ctr_hor = box.top.x; ctr_hor <= box.bot.x; ctr_hor += zoom)
	{
		line(ctr_hor, box.top.y, ctr_hor, box.bot.y);
		for(ctr_ver = box.top.y; ctr_ver <= box.bot.y; ctr_ver += zoom)
		{
			line(box.top.x, ctr_ver, box.bot.x, ctr_ver);
		}
	}
}

void get_equation(char *word, int bgcolor)
{
	char buffer[2];
	int ctr = 0, color, xcoord, ycoord, charwidth;
	struct time time[2];
	float dif;

	ctr = strlen(word);

	if(ctr > 0) quick_help("You have chosen to Edit this equation. Use BACKSPACE to delete a character.");
	else quick_help("Type the equation as you would write it, without any special characters.");

	buffer[0] = word[ctr-1];  //set buffer
	buffer[1] = '\0';

	xcoord = getx();
	ycoord = gety();

	color = getcolor();
	//bgcolor = getbkcolor();
	//bgcolor = 2;

	gettime(&time[0]);
	gettime(&time[1]);

	while(1)
	{
		change_font_size(word, buffer);
		gettime(&time[0]);
		dif = abs(time[0].ti_hund -  time[1].ti_hund);

		if(dif <  10)
		{
			setcolor(color);
			outtextxy(xcoord, ycoord, "_");
			delay(0);
		}

		if (dif >= 10)
		{
			setcolor(bgcolor);
			outtextxy(xcoord, ycoord, "_");
			delay(0);
			if(dif > 20)
			gettime(&time[1]);
		}


		if (kbhit())
		{
			setcolor(bgcolor);               //clear cursor
			outtextxy(xcoord, ycoord, "_");
			delay(0);

			setcolor(color);
			buffer[0] = getch();

			if(buffer[0] == (char)8)
			{
				if(strlen(word) < 1) continue; //if it is the start of the word
											  //nothing to erase
				setcolor(bgcolor);


				buffer[0] = word[--ctr];
				buffer[1] = '\0';

				change_font_size(word, buffer); //needs to be here
														  //buffer and word must be the
														  //way they are at this time

				word[ctr] = '\0';

				charwidth = textwidth(buffer);

				xcoord -= charwidth;
				moveto(xcoord, ycoord);
				outtext(buffer);
				moveto(xcoord, ycoord);

				continue;
			}

			if(buffer[0] == '\r')
			{
				word[ctr] = '\0';
				return;
			}

			buffer[1] = '\0';
			//if wrong char is inputed...
			if(!(isdigit(buffer[0])) && buffer[0]!='x' && buffer[0]!='-' && buffer[0]!='+' && buffer[0]!='.' || strlen(word)>=13)
			{
				sound(600);
				delay(50);
				nosound();

				continue;
			}
			word[ctr++] = buffer[0];
			word[ctr] = '\0';

			change_font_size(word,buffer);
			outtext(buffer);

			xcoord = getx();
			ycoord = gety();
	  }

	}
}

void change_font_size(char *word, char *buffer)
{
	int ctr;
	struct textsettingstype text;

	text = defsize();

	ctr = strlen(word)-2;

	if (text.charsize > 1)
	{
		if(tolower(word[ctr]) == 'x' && isdigit(buffer[0]))
		{
			settextstyle(text.font, text.direction, 1);
		}
			else defsize();
	}


}

struct textsettingstype defsize(void)
{
	static struct textsettingstype textstyle;
	static first_time = 1;
	if (first_time)
	{
		gettextsettings(&textstyle);
		first_time = 0;
	}
	settextstyle(textstyle.font, textstyle.direction, textstyle.charsize);
	return textstyle;
}

int select_equations()
{
	static int eqnum=0;
	int maxword = 5, bgcolor = 0, ctr = 0;
	int x1,x2,y1,y2;
	char key = ' ';

	setlinestyle(0,0,1);
	settextstyle(7,HORIZ_DIR, 3);
	//set corners of rectangle
	while(key!=13)
	{


	setcolor(14-eqnum);
	ctr++;
	if(ctr > 1) { ctr = 0; setcolor(6-eqnum); }


	x1 = 3;
	x2 = 250;
	y1 = eqnum * textheight("X")*1.3 + 3;        //need +8 because text is
	y2 = (eqnum + 1) * textheight("X")*1.3 + 3;  //moved alittle down ???

	rectangle(x1, y1, x2, y2);

	delay(100);


		if(kbhit())
		{
			key = getch();
			//if "Enter"...
			if(key==13)
			{
				setcolor(bgcolor);                 //clear box
				rectangle(x1, y1, x2, y2);
				return eqnum;
			}
			if(key==27) //if ESC...
			{
				setcolor(bgcolor);                 //clear box
				rectangle(x1, y1, x2, y2);
				return 27; //27 is code for cancel
			}
			setcolor(bgcolor);                 //clear box
			rectangle(x1, y1, x2, y2);

				//if it is Arrow Keys...
			if (key == 0)
			{
				key = getch();

				switch (key)
				{
					case 'H': if(eqnum<=0) eqnum = 5;
								 else eqnum--;
								 sound(800);
								 delay(10);
								 nosound();
								 break;
					case 'P': if(eqnum>=maxword) eqnum = 0;
								 else eqnum++;
								 sound(800);
								 delay(10);
								 nosound();
								 break;
				}
			}
		}
	}
}

void drawYs()
{
	int ctr, eqnum;
	int x1, x2, y1, y2;

	setcolor(7);
	rectangle(0, 0, 253, 185);
	setfillstyle(1, 8);
	floodfill(100, 100, 7);

	settextstyle(7,HORIZ_DIR, 3); //need for rectangles
	for (eqnum = 0; eqnum <= 5; eqnum++)
	{
		//rectangle init...
		x1 = 3;
		x2 = 250;
		y1 = eqnum * textheight("X")*1.3 + 3;
		y2 = (eqnum + 1) * textheight("X")*1.3 + 3;

		setcolor(0);
		rectangle(x1,y1,x2,y2);
	}

	for(ctr = 0; ctr <= 5; ctr++)
	{
		setcolor(14-ctr);
		settextstyle(7,HORIZ_DIR, 3);
		moveto(5, ctr*textheight("X")*1.3);

		outtext("y = ");
	}

}

char *equations(int eqnum) //returns the address of the equation!!!
{
	static char equation[6][30] = {""};

	return equation[eqnum];
}

void *get_draw_equations(int eqnum, char *equation)
{
	int x, y, ctr;
	char buf[2];

	buf[1] = '\0'; //end of 1-letter string

	setcolor(14-eqnum);
	settextstyle(7,HORIZ_DIR, 3);

	x = textwidth("y = ");
	for (ctr = 0; ctr < strlen(equation); ctr++)
	{
		buf[0] = equation[ctr];
		//change font size if power...
		if(isdigit(buf[0]) && equation[ctr-1] == 'x')
		{
			settextstyle(7,HORIZ_DIR, 1);
		}
		x += textwidth(buf);
		settextstyle(7,HORIZ_DIR, 3); //reset textsize back to original
	}

	y = eqnum*textheight("X")*1.3;

	moveto(x, y);

	get_equation(equation, 8);
}

void graphing_procedure(int zoom, struct coord mid)
{
	char *eq_word;
	int eqnum, graph_color, dot_color = 4;
	struct values vals;

	quick_help("Use Arrow Keys to select a field, then press ENTER to type in the equation.");
	eqnum = select_equations();
	if(eqnum == 27) //if user canceled...
		return;

	eq_word = equations(eqnum);
	get_draw_equations(eqnum, eq_word);
	if(strlen(eq_word) == 0) return;
	vals = get_values(eq_word);
	graph_color = 14 - eqnum; //set color for graphing
	draw_graph(vals, mid, zoom, graph_color);
	draw_dots(vals, mid, zoom, dot_color);
}

void clear_graph(struct coord mid)
{
	struct box corners;

	quick_help("Clearing Graph...");

	corners.top.x = mid.x - GRID - 5; // +/- 5 so it clears more than just the GRID
	corners.top.y = mid.y - GRID - 5;
	corners.bot.x = mid.x + GRID + 5;
	corners.bot.y = mid.y + GRID + 5;

	setfillstyle(1,0);

	bar(corners.top.x, corners.top.y, corners.bot.x, corners.bot.y);
}

void graph_all(struct coord mid, int zoom)
{
	char eqword[20];
	int eqnum, graph_color, dot_color = 4;
	struct values vals;

	for (eqnum = 0; eqnum <= 5; eqnum++)
	{
		strcpy(eqword, equations(eqnum)); //so dont change the actual str value(?)
		if(strlen(eqword) < 1) continue;
		vals = get_values(eqword);
		graph_color = 14 - eqnum; //set color for graphing
		draw_graph(vals, mid, zoom, graph_color);
		draw_dots(vals, mid, zoom, dot_color);
	}
}

void root_procedure()
{
	int eqnum;
	char *equation;
	struct values vals;
	float *root;

	quick_help("Select an equation, then press ENTER to see its Root(s)");
	eqnum = select_equations();
	if(eqnum == 27) return; //if cancel...
	equation = equations(eqnum);
	vals = get_values(equation);
	root = get_roots(vals);
	if(vals.pow > 2)
		root[0] = root[1] = HIGH_POWER;
	if((strlen(equation)) < 1)
		root[0] = root[1] = NO_EQUATION;

	draw_roots(root, eqnum);
}

float *get_roots(struct values vals)
{
	float *root = malloc(2 * sizeof(float));

	if((vals.b*vals.b - 4 * vals.a * vals.c) < 0) //if domain for sqrt is negative
	{
		root[0] = root[1] = IMAGINARY;
		return root;
	}
	if(vals.a == 0) //if linear equation...
	{
		root[0] = root[1] = -vals.c/vals.b;
	}
	else  //if quadratic
	{
		root[0] = QUADRATIC_POS(vals);
		root[1] = QUADRATIC_NEG(vals);
	}
	if(root[0] == -0) root[0] = 0; //sometimes answer comes out -0 :-(
	if(root[1] == -0) root[1] = 0; //ex. y = x   x = -0. This fixes it

	return root;
}

void draw_roots(float root[], int eqnum)
{
	void *backimage;
	char *finalstr = malloc(30 * sizeof(char));
	char *buf = malloc(10 * sizeof(char));
	int seg = 6; //significant digits
	int bgcolor = 14-eqnum-8, color = 14-eqnum; //bg is the darker version of the clr.
	struct box box = {{200, 200}, {450, 230}};
	struct coord center;

	center.x = box.top.x + (abs(box.top.x - box.bot.x) / 2);
	center.y = box.top.y + (abs(box.top.y - box.bot.y) / 2);

	if ((backimage =
		malloc(imagesize(box.top.x, box.top.y, box.bot.x, box.bot.y)))
		== NULL) exit(1);
	getimage(box.top.x, box.top.y,box.bot.x, box.bot.y, backimage);

	setfillstyle(SOLID_FILL, bgcolor);
	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);
	setcolor(color);
	setlinestyle(DOTTED_LINE, 0, 3);
	rectangle(box.top.x+2, box.top.y+2, box.bot.x-2, box.bot.y-2);

	if(root[0] == IMAGINARY)
	{
		quick_help("This equation has no roots. The root is imaginary.");
		strcpy(finalstr, "There is no root.");
	}
		else if(root[0] == HIGH_POWER)
		{
			quick_help("Cannot find the root(s) because this equation is not quadratic or linear.");
			strcpy(finalstr, "Can't find root.");
		}
		else if(root[0] == NO_EQUATION)
		{
			quick_help("You selected a field that has no equation entered.");
			strcpy(finalstr, "ERROR: no equation entered");
		}
		else if(root[0] == root[1])
		{
			strcpy(finalstr, "x = ");
			gcvt(root[0], seg, buf);
			strcat(finalstr, buf);
			quick_help("This equation has one root.");
		}
			else
			{
				strcpy(finalstr, "x = ");
				gcvt(root[0], seg, buf);
				strcat(finalstr, buf);
				strcat(finalstr, " or x = ");
				gcvt(root[1], seg, buf);
				strcat(finalstr, buf);
				quick_help("This equation has two roots.");
			}

	setcolor(color);
	settextstyle(0,0,0);
	center.x -= textwidth(finalstr)/2; //adjust/center the text
	center.y -= textheight(finalstr)/2;
	outtextxy(center.x, center.y, finalstr);
	//clean up
	getch();//suspend
	putimage(box.top.x, box.top.y, backimage, 0);
	free(backimage);
	free(finalstr);
	free(buf);
}

void print_values_procedure()
{
	int eqnum;
	char *equation;
	void *backimage;
	struct values vals;
	struct box box = {{200, 200}, {400, 350}};

	quick_help("Use Arrow Keys to select an equation. Press ENTER to see its values.");
	eqnum = select_equations();
	if(eqnum == 27) return; //if cancel...
	equation = equations(eqnum);
	vals = get_values(equation);

	if ((backimage =
		malloc(imagesize(box.top.x, box.top.y, box.bot.x, box.bot.y)))
		== NULL) exit_all();
	getimage(box.top.x, box.top.y,box.bot.x, box.bot.y, backimage);

	print_values(print_values_draw(box, eqnum), vals, eqnum); //nested function returns struct grpah box

	//clean up
	putimage(box.top.x, box.top.y, backimage, 0);
	free(backimage);
	return;

}

struct box print_values_draw(struct box box, int eqnum)
{
	int color = 14-eqnum, bgcolor = 6-eqnum;
	int marginheight = 20, marginwidth = 10;
	struct box lines = box;
	struct coord textcoord, center;

	center.x = box.top.x + abs(box.top.x - box.bot.x) / 2;
	center.y = box.top.y + abs(box.top.y - box.bot.y) / 2;

	setlinestyle(SOLID_LINE, 0, 1);
	setfillstyle(SOLID_FILL, bgcolor);
	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);
	setcolor(0);
	rectangle(box.top.x, box.top.y, box.bot.x, box.bot.y);

	box.top.x += marginwidth;
	box.top.y += marginheight;
	box.bot.x -= marginwidth;
	box.bot.y -= marginheight;

	setfillstyle(SOLID_FILL, color);
	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);
	rectangle(box.top.x-1, box.top.y-1, box.bot.x+1, box.bot.y+1);

	lines.top.x = lines.bot.x = center.x;
	lines.top.y = box.top.y;
	lines.bot.y = box.bot.y;

	line(lines.top.x, lines.top.y, lines.bot.x, lines.bot.y);

	//text dealing portion...
	settextstyle(0,0,0);
	setcolor(0);

	lines.top.x = box.top.x;
	lines.bot.x = box.bot.x;
	lines.top.y = lines.bot.y = box.top.y + textheight("y") + 3;

	line(lines.top.x, lines.top.y-1, lines.bot.x, lines.bot.y-1);

	textcoord.x = box.top.x + abs(box.top.x - center.x)/2 - textwidth("y")/2;
	textcoord.y = box.top.y + abs(box.top.y - lines.top.y)/2 - textheight("x")/2;
	outtextxy(textcoord.x, textcoord.y, "x");

	textcoord.x = center.x + abs(box.top.x - center.x)/2 - textwidth("y")/2;
	outtextxy(textcoord.x, textcoord.y, "y");

	box.top.y = lines.top.y;

	return box;
}

void print_values(struct box box, struct values vals, int eqnum)
{
	struct coord center, xloc, yloc;
	float x, y, firstx = 6, lastx;
	int color = 0, bgcolor = 14 - eqnum;
	void *imagevalues;
	struct box putbox;
	char c;

	center.x = box.top.x + abs(box.top.x - box.bot.x) / 2;
	center.y = box.top.y + abs(box.top.y - box.bot.y) / 2;

	box.top.y += 2;

	xloc.x = box.top.x + 5;
	xloc.y = yloc.y = box.top.y;
	yloc.x = center.x + 5;

	settextstyle(0,0,0);
	setcolor(color);

	for(x = firstx; xloc.y + textheight("X") < box.bot.y; x -= 1)
	{
		y = FORMULA(vals, x);
		print_xy(xloc, x, yloc, y, color);
		xloc.y = yloc.y += textheight("X");
		lastx = x;
	}
	box.bot.y = yloc.y;
	xloc.y = yloc.y -= textheight("X");

	putbox.top.x = box.top.x;
	putbox.top.y = box.top.y;
	putbox.bot.x = box.bot.x;
	putbox.bot.y = xloc.y;

	if ((imagevalues =
		malloc(imagesize(putbox.top.x, putbox.top.y, putbox.bot.x, putbox.bot.y)))
		== NULL) exit_all();

	quick_help("Use Up/Down Arrow Keys to scroll through all the values.");

	while(1)
	{
		c = getch();
		if(c == NULL)
		{
			c = getch();
			if(c == 'P')
			{
				putbox.top.y = box.top.y + textheight("X");
				putbox.bot.y = box.bot.y;
				getimage(putbox.top.x, putbox.top.y, putbox.bot.x, putbox.bot.y, imagevalues);
				putbox.top.y = box.top.y;
				putimage(putbox.top.x, putbox.top.y, imagevalues, 0);

				y = FORMULA(vals, lastx);
				xloc.y = yloc.y = box.bot.y - textheight("X");
				print_xy(xloc, lastx, yloc, y, bgcolor);
				lastx--;
				firstx--;
				y = FORMULA(vals, lastx);
				print_xy(xloc, lastx, yloc, y, color);
			}
			if(c == 'H')
			{
				putbox.top.y = box.top.y;
				putbox.bot.y = box.bot.y - textheight("X") - 1;
				getimage(putbox.top.x, putbox.top.y, putbox.bot.x, putbox.bot.y, imagevalues);
				putbox.top.y = box.top.y + textheight("X");
				putimage(putbox.top.x, putbox.top.y, imagevalues, 0);

				y = FORMULA(vals, firstx);
				xloc.y = yloc.y = box.top.y;
				print_xy(xloc, firstx, yloc, y, bgcolor);
				firstx++;
				lastx++;
				y = FORMULA(vals, firstx);
				print_xy(xloc, firstx, yloc, y, color);
			}
		}
		else
		{
			free(imagevalues);
			return;
		}
	}
}

void print_xy(struct coord xloc, float x, struct coord yloc, float y, int color)
{
	char *buf = malloc(15 * sizeof(char));
	int seg = 4;

	setcolor(color);
	gcvt(x, seg, buf);
	outtextxy(xloc.x, xloc.y, buf);
	gcvt(y, seg, buf);
	outtextxy(yloc.x, yloc.y, buf);

	free(buf);
}
void about()
{
	char licence[] = "This program is provided without warranty of any kind. It is FREEWARE and can be freely used and distributed.";
	char author[] = "Author: Yevgeniy Eltsufin  Email: orsk@hotmail.com  WebSite: http://yev.cjb.net  Date: 01/09/99";
	struct coord txtloc;

	setcolor(8);
	settextstyle(SMALL_FONT, 0, 4);

	txtloc.x = 320 - textwidth(licence)/2;
	txtloc.y = 470 - textheight("X")*2;

	outtextxy(txtloc.x, txtloc.y, licence);

	txtloc.x = 320 - textwidth(author)/2;
	txtloc.y = 470 - textheight("X");

	setcolor(8);
	outtextxy(txtloc.x, txtloc.y, author);
}
void quick_help_draw()
{
	struct box box = {{10, 430}, {630, 450}};

	setlinestyle(SOLID_LINE, 0, 1);
	setfillstyle(SOLID_FILL, 7);

	bar(box.top.x, box.top.y, box.bot.x, box.bot.y);
	setcolor(8);
	rectangle(box.top.x+1, box.top.y+1, box.bot.x+1, box.bot.y+1);
	setcolor(15);
	rectangle(box.top.x-1, box.top.y-1, box.bot.x-1, box.bot.y-1);
}
void quick_help(char *string)
{
	struct coord loc = {15, 437};
	static char *lststr = "";
	int orig_color = getcolor();
	struct textsettingstype origtxt;

	gettextsettings(&origtxt);

	settextstyle(DEFAULT_FONT, 0, 1);
	if(lststr != "") //clear
	{
		setcolor(LIGHTGRAY);
		outtextxy(loc.x, loc.y, lststr);
	}
	setcolor(0);
	outtextxy(loc.x, loc.y, string);
	lststr = string;

	setcolor(orig_color); //set back the original color
	settextstyle(origtxt.font, origtxt.direction, origtxt.charsize);
}

void print_all_equations()
{
	int x = 0, y = 0, eqnum, charnum = 0;
	char *equation;
	char buf[3];

	settextstyle(7,HORIZ_DIR, 3);

	for(eqnum = 0; eqnum < 6; eqnum++)
	{
		setcolor(14-eqnum);
		defsize(); //set the default font style--size
		y = eqnum*textheight("X")*1.3;
		x = textwidth("y = ");
		equation = equations(eqnum);
		if(strlen(equation) < 1) continue;
		for (charnum = 0; charnum < strlen(equation); charnum++)
		{
			buf[1] = '\0';
			buf[0] = equation[charnum];
			buf[2] = equation[charnum+1];
			equation[charnum+1] = '\0';
			change_font_size(equation, buf);
			outtextxy(x, y, buf);
			equation[charnum+1] = buf[2];
			x += textwidth(buf);
		}
	}
}

void help_procedure()
{
	char *helpstr;

	cleardevice();

	draw_help();

	help_branch(help_select());
}

void draw_help()
{
	int bgcolor = 5, ctr=0;
	struct coord txtloc, center = {320, 240};

	setbkcolor(0);
	setfillstyle(SOLID_FILL, bgcolor);

	stars(500);

	while(ctr < 2)
	{
		ctr++;
		setlinestyle(0, 0, 3);
		settextstyle(0,0,2);
		if(ctr == 1)
		{
			setcolor(2);
			txtloc.x = 203;
			txtloc.y = 203;
		}
		else
		{
			txtloc.x = 200;
			txtloc.y = 200;
			setcolor(10);
		}

		line(txtloc.x-25, txtloc.y-50, txtloc.x-75, getmaxy());
		line(txtloc.x-50, txtloc.y-25, getmaxx(), txtloc.y-25);

		outtextxy(txtloc.x, txtloc.y, "1.Introduction");
		txtloc.y += textheight("X")+10;
		outtextxy(txtloc.x, txtloc.y, "2.General Help");
		txtloc.y += textheight("X")+10;
		outtextxy(txtloc.x, txtloc.y, "3.FAQ");
		txtloc.y += textheight("X")+10;
		outtextxy(txtloc.x, txtloc.y, "4.About");
		txtloc.y += textheight("X")+10;


		line(txtloc.x+textwidth("X")*13+25, txtloc.y+50,
			  txtloc.x+textwidth("X")*13+75, 0);
		line(txtloc.x+textwidth("X")*13+50, txtloc.y+25, 0, txtloc.y+25);
	}
	settextstyle(DEFAULT_FONT, 0, 0);
	setcolor(7);
	outtextxy(txtloc.x-25, txtloc.y, "Press the number of your choice");
	//TITLE...
	settextstyle(0, 0, 5);
	setcolor(8);
	outtextxy(79, 79, "Help");
	outtextxy(304, 404, "GrpahXY");
	setcolor(15);
	outtextxy(75, 75, "Help");
	outtextxy(300, 400, "GrpahXY");
}

char *load_file(char *filename)
{
	FILE *helpfile;
	char *helpstr;
	int strsize;

	if((helpfile = fopen(filename, "r")) == NULL)
	{
		printf("Cannot open file %s", filename);
		return NULL;
	}
	else
	{
		strsize = filesize(helpfile);
		helpstr = malloc(strsize * sizeof(char));
		fread(helpstr, strsize, 1, helpfile);
		helpstr[strsize] = '\0';
		fclose(helpfile);

		return helpstr;
	}
}

int filesize(FILE *stream)
{
	int length = 0;

	while(1)
	{
		fgetc(stream);
		if(feof(stream)) break;
		length++;
	}
	fseek(stream, 0, SEEK_SET);

	return length;
}

int help_select()
{
	char buf;
	int opt;

	buf = getch();

	if(buf == '1') opt = 0;
	if(buf == '2') opt = 1;
	if(buf == '3') opt = 2;
	if(buf == '4') opt = 3;
	if(buf == 27) opt = 27;

	return opt;
}
void help_branch(int opt)
{
	if(opt == 0) help_topic_display("Introduction", "intro.hlp", 6);
	if(opt == 1) help_topic_display("General Help", "general.hlp", 5);
	if(opt == 2) help_topic_display("FAQ", "faq.hlp", 4);
	if(opt == 3) help_topic_display("About", "about.hlp", 6);
	if(opt == 27) return;
}
void help_topic_display(char *title, char *filename, int txtsize)
{
	char *string;

	cleardevice();
	setbkcolor(6);
	stars(1000);

	settextstyle(DEFAULT_FONT, 1, 4);
	setcolor(7);
	outtextxy(5+textheight("X"),240-textwidth(title)/2,title);
	setlinestyle(0,0,3);
	line(10+textheight("X"), 0, 10+textheight("X"), getmaxy());

	settextstyle(SMALL_FONT,0,txtsize);
	//settextstyle(DEFAULT_FONT,0,1);

	string = load_file(filename);

	setcolor(8);
	wordwrap(string, 102, 602, 12);
	setcolor(14);
	wordwrap(string, 100, 600, 10);

	//clean up
	getch();
	free(string);
	help_procedure();
}
void wordwrap(char *str, int min, int max, int ycoord)
{
	#define WORDMAX 40
	char word[WORDMAX], buf[3] = "";
	char specialchar = 'a';
	int wrdctr = 0, strctr = 0, xcoord = min, bufctr = 0;
	int originalcolor = getcolor();

	while(strctr < strlen(str))
	{
		wrdctr = 0;
		setcolor(originalcolor);
		do
		{
			if(str[strctr] == '~')
			{
				strctr++;
				while(str[strctr] != '~')
				{
					buf[bufctr++] = str[strctr++];
				}
				strctr++;
				buf[bufctr] = '\0';
				bufctr = 0;
				if(originalcolor != 8) setcolor(atoi(buf));
			}
			if( str[strctr] == '\n' ||
				 str[strctr] == '\0' ||
				 str[strctr] == '\t'   )
			{
				specialchar = str[strctr];
				word[wrdctr] = ' ';
			}
			else word[wrdctr] = str[strctr];
			wrdctr++;
			strctr++;
		}while(word[wrdctr-1] != ' ' && wrdctr < WORDMAX);
		word[wrdctr] = '\0';

		if(xcoord + textwidth(word) > max)
		{
			ycoord += textheight("X");
			xcoord = min;
		}

		outtextxy(xcoord, ycoord, word);
		xcoord += textwidth(word);

		if(specialchar == '\n')
		{
				ycoord += textheight("X");
				xcoord = min;
		}
		if(specialchar == '\0') return;
		if(specialchar == '\t') xcoord += textwidth("XXXX");
		specialchar = 'a';   //makes it not special character
	}
}
void stars(int num)
{
	int ctr;

	for(ctr = 0; ctr < num; ctr++)
		putpixel(rand() % getmaxx(), rand() % getmaxy(), rand()%getmaxcolor());

}

void exit_all()
{
	closegraph();
	exit(0);
}