#include "libbip.h"
#include "fastui.h"

#ifndef FastUI_Demo
#define FastUI_Demo

#define SCREEN_INTRO 0
#define SCREEN_BUTTON_STYLES 1
#define SCREEN_BUTTON_COORDS 2
#define SCREEN_BUTTON_ICONS 3
#define SCREEN_SWITCHES 4
#define SCREEN_CHECKBOXES 5
#define SCREEN_IMAGES 6
#define SCREEN_ANIMATION 7
#define SCREEN_BYE 8

#define SCREENS_COUNT 9

#define ANIMATION_DEMO_DELAY 80
#define ANIMATION_HEART_DELAY 300

struct app_data_t
{
	void* ret_f;
	Elf_proc_* proc;

	short currentScreen;

	Button* buttonIntro;

	unsigned short button1Color;
	unsigned short button2Color;
	Button* buttonColors1;
	Button* buttonColors2;

	char* coordsText;
	Button* buttonCoords;

	unsigned short volumeValue;
	Button* buttonIcons1;
	Button* buttonIcons2;

	unsigned short switchHandlerValue;
	Switch* switch1;
	Switch* switch2;

	Checkbox* checkbox1;
	Checkbox* checkbox2;
	Checkbox* checkbox3;

	Image* image1;
	Image* image2;
	Image* image3;

	unsigned short animationActive;
	Animation* animationDemo;

	Animation* animationHeart;
};

void 	init_app(void* return_screen);
void 	set_initial_values();
void 	show_screen();
void	touch_handler(void* gesture);
void 	keypress_handler();
void 	update_screen();
void	draw_screen();

#endif