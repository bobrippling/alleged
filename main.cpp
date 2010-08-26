#include <cstdio>
#include <cstdlib>
#include <allegro.h>
#include <cerrno>
#include <cmath>

#define WIDTH 800
#define HEIGHT 600

#define FRAME_DELAY 60
#define NBALLS 5

#define FILE_BALL "ball_24.bmp"

#define BALL_SPEED 5.0f

#include "phyobj.h"
#include "ball.h"

Ball *balls[NBALLS];
bool running = true;
volatile long timer = 0; /* fps business */

int lewp(void);
void timerfunc(void);

int  initballs(void);
void termballs(void);

void handler_close(void);


void timerfunc(void)
{
	/* this gets called FRAME_DELAY times per frame */
	timer++;
}
END_OF_FUNCTION(timerfunc)

int lewp(void)
{
	BITMAP *buffer = create_bitmap(WIDTH, HEIGHT); /* backbuffer */
	int i;

	clear_bitmap(buffer);
	clear_bitmap(screen);

	while(running && !key[KEY_Q]){
		/*
		 * timer remains >0 while we're still in a frame
		 * once the frame's time is up, timer <= 0
		 */

		while(timer > 0){
			/* logic here */
			for(i = 0; i < NBALLS; i++)
				balls[i]->move(WIDTH, HEIGHT);

			timer--;
		}

		/* draw here */
		/*
		line(buffer, 250, 250, 350, 350, makecol(255, 255, 255));
		circle(buffer, 250, 250, 500, makecol(255, 255, 255));
		*/

		for(i = 0; i < NBALLS; i++){
			draw_sprite(buffer, balls[i]->bmp(), balls[i]->x(), balls[i]->y());

			/* bmp, font, x, y, col, transparency, str, ... */
			textprintf_centre_ex(buffer, font, balls[i]->x(), balls[i]->y(), balls[i]->col(), -1, "balls[%d]", i);
		}

		blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
		clear_bitmap(buffer);
	}

	destroy_bitmap(buffer);
	return 0;
}

int initballs()
{
	int i;
	BITMAP *tmpbmp;

	if(access(FILE_BALL, F_OK) == -1){
		allegro_message("access(): "FILE_BALL": %s\n", strerror(errno));
		return 1;
	}

	for(i = 0; i < NBALLS; i++){
		tmpbmp = load_bitmap(FILE_BALL, NULL /* pallete */);
		if(!tmpbmp){
			allegro_message("load_bitmap(): %s: %s", FILE_BALL, errno ? strerror(errno) : "unknown error");
			while(i > 0)
				delete balls[--i];
			return 1;
		}

		balls[i] = new Ball(rand() % WIDTH, rand() % HEIGHT,
				BALL_SPEED, (rand() % 360) * M_PI/180, tmpbmp, makecol(0, 255, 0));
	}

	return 0;
}

void termballs()
{
	int i;
	for(i = 0; i < NBALLS; i++)
		delete balls[i];
}

void handler_close()
{
	running = false;
}
END_OF_FUNCTION(timerfunc)

void funcs_init()
{
	install_keyboard();
	install_timer();

	LOCK_VARIABLE(timer);
	/* the following functions need END_OF_FUNCTION($_) after them */
	LOCK_FUNCTION(&timerfunc);
	LOCK_FUNCTION(&handler_close);

	/* more timer business - this calls timerfunc every frame */
	install_int_ex(timerfunc, BPS_TO_TIMER(FRAME_DELAY));
	set_close_button_callback(&handler_close);
}

int graphx_init()
{
	set_color_depth(16);
	if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0)){
		allegro_message("set_gfx_mode(): %s", strerror(errno));
		return 1;
	}
	return 0;
}

int main(int argc, const char **argv)
{
	int ret = 0;

	srand(time(NULL));

	if(allegro_init()){
		allegro_message("allegro_init(): %s", strerror(errno));
		return 1;
	}

	funcs_init();
	if(graphx_init() || initballs()) /* must load the bitmap(s) after graphic setup */
		goto bail;

	ret = lewp();

	termballs();
bail:
	allegro_exit();
	return ret;
}
