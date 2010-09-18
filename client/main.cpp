#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <vector>

#include <allegro.h>
#include <arpa/inet.h>

#define WIDTH  800.0
#define HEIGHT 600.0

#define FRAME_DELAY 60
#define Njets 5

#define JET_FILE "img/jet.bmp"
#define JET_SPEED 5.0

#include "netobj.h"
#include "phyobj.h"
#include "jet.h"


Jet *jetMe;
std::vector<Jet> jets;

bool running = true;
volatile long timer = 0; /* fps business */

int lewp(void);
void timerfunc(void);

int  initjets(void);
void termjets(void);

void handler_close(void);


void timerfunc(void)
{
	/* this gets called FRAME_DELAY times per frame */
	timer++;
}
END_OF_FUNCTION(timerfunc)


inline void logic()
{
	jetMe->move(WIDTH, HEIGHT);

	for(std::vector<Jet>::iterator it = jets.begin();
			it != jets.end();
			++it)
		(*it).move(WIDTH, HEIGHT);
}

inline void proc_keys()
{
	if(key[KEY_W])
		jetMe->thrust(true);
	else
		jetMe->thrust(false);

	if(key[KEY_D])
		jetMe->rotate_right();
	else if(key[KEY_A])
		jetMe->rotate_left();
}

inline void draw(BITMAP *buffer)
{
	jetMe->draw(buffer);

	for(std::vector<Jet>::iterator it = jets.begin();
			it != jets.end();
			++it)
		(*it).draw(buffer);


	blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
	clear_bitmap(buffer);
}

int lewp(void)
{
	BITMAP *buffer = create_bitmap(WIDTH, HEIGHT); /* backbuffer */

	clear_bitmap(buffer);
	clear_bitmap(screen);

	while(running && !key[KEY_ESC]){
		/*
		 * timer remains >0 while we're still in a frame
		 * once the frame's time is up, timer <= 0
		 */

		while(timer > 0){
			logic();
			proc_keys();
			timer--;
		}


		draw(buffer);
	}

	destroy_bitmap(buffer);
	return 0;
}

int initjets()
{
	BITMAP *tmpbmp;

	if(access(JET_FILE, F_OK) == -1){
		allegro_message("access(): " JET_FILE ": %s\n", strerror(errno));
		return 1;
	}

	if(!(tmpbmp = load_bitmap(JET_FILE, NULL))){
		perror("load_bitmap("JET_FILE")");
		return 1;
	}

	jetMe = new Jet(JET_SPEED, tmpbmp, NULL);

	return 0;
}

void termjets()
{
	delete jetMe;
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

int main(void)
{
	int ret = 0;

	srand(time(NULL));

	if(allegro_init()){
		allegro_message("allegro_init(): %s", strerror(errno));
		return 1;
	}

	funcs_init();
	if(graphx_init() || initjets()) /* must load the bitmap(s) after graphic setup */
		goto bail;

	ret = lewp();

	termjets();
bail:
	allegro_exit();
	return ret;
}