#include "libbip.h"

/************************************************/
/*                                              */
/*               FastUI beta v1.0               */
/*                                              */
/*          UI library for BipOS 0.5.2+         */
/*                                              */
/*             Developer by Yuukari             */
/*   https://github.com/Yuukari/FastUILibrary   */
/*                                              */
/************************************************/

/* Options */

#define FASTUI_BASE_FG_COLOR COLOR_WHITE
#define FASTUI_BASE_BG_COLOR COLOR_BLACK

#define FASTUI_UPDATE_DELAY 150

/* Constants */

#define ICON_EMPTY 0xFFFFFFFF

/* Basic structures */

typedef struct position {
	int x;
	int y;
} position;

typedef struct size {
	int width;
	int height;
} size;

typedef struct icon {
	unsigned int id;
	unsigned short type;
	int index_listed;
} icon;

/* Handlers */

typedef void (*button_click_handler)(unsigned short, unsigned short);
typedef void (*switch_click_handler)(unsigned short);
typedef void (*checkbox_click_handler)(unsigned short);

/************* Button component *************/

typedef struct Button {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	unsigned int background_color;
	unsigned int border_color;
	unsigned int hover_color;
	unsigned int text_color;

	char* text;
	unsigned int icon_id;
	unsigned short icon_type;
	int index_listed;

	unsigned int is_pressed;

	button_click_handler click_handler;
} Button;

/* Position and size */

void setButtonPosition(struct Button* c, int x, int y);
position getButtonPosition(struct Button* c);
void setButtonSize(struct Button* c, int width, int height);
size getButtonSize(struct Button* c);

/* View */

void setButtonBackgroundColor(struct Button* c, unsigned int color);
unsigned int getButtonBackgroundColor(struct Button* c);
void setButtonBorderColor(struct Button* c, unsigned int color);
unsigned int getButtonBorderColor(struct Button* c);
void setButtonHoverColor(struct Button* c, unsigned int color);
unsigned int getButtonHoverColor(struct Button* c);

void setButtonTextColor(struct Button* c, unsigned int color);
unsigned int getButtonTextColor(struct Button* c);
void setButtonText(struct Button* c, char* text);
char* getButtonText(struct Button* c);

void setButtonIcon(struct Button* c, int index_listed, unsigned int resource_id);
unsigned int getButtonIcon(struct Button* c);

/* Handlers */

void setButtonClickHandler(struct Button* c, button_click_handler handler);

/* Lifecycle methods */

Button* createButton();
int gestureButton(struct Button* c, struct gesture_* gest);
void updateButton(struct Button* c);
void drawButton(struct Button* c);
void destroyButton(struct Button* c);

/************* Switch component *************/

typedef struct Switch {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	unsigned int text_color;
	char* text;
	unsigned short text_padding;

	unsigned short enabled;

	switch_click_handler click_handler;
} Switch;

/* Position and size */

void setSwitchPosition(struct Switch* c, int x, int y);
position getSwitchPosition(struct Switch* c);
void setSwitchSize(struct Switch* c, int width, int height);
size getSwitchSize(struct Switch* c);

/* View */

void setSwitchTextColor(struct Switch* c, unsigned int color);
unsigned int getSwitchTextColor(struct Switch* c);
void setSwitchText(struct Switch* c, char* text);
char* getSwitchText(struct Switch* c);

/* Handlers */

void setSwitchClickHandler(struct Switch* c, switch_click_handler handler);

/* Lifecycle methods */

Switch* createSwitch();
int gestureSwitch(struct Switch* c, struct gesture_* gest);
void drawSwitch(struct Switch* c);
void destroySwitch(struct Switch* c);

/************ Checkbox component ************/

typedef struct Checkbox {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	unsigned int text_color;
	char* text;
	unsigned short text_padding;

	unsigned short enabled;

	checkbox_click_handler click_handler;
} Checkbox;

/* Position and size */

void setCheckboxPosition(struct Checkbox* c, int x, int y);
position getCheckboxPosition(struct Checkbox* c);
void setCheckboxSize(struct Checkbox* c, int width, int height);
size getCheckboxSize(struct Checkbox* c);

/* View */

void setCheckboxTextColor(struct Checkbox* c, unsigned int color);
unsigned int getCheckboxTextColor(struct Checkbox* c);
void setCheckboxText(struct Checkbox* c, char* text);
char* getCheckboxText(struct Checkbox* c);

/* Handlers */

void setCheckboxClickHandler(struct Checkbox* c, checkbox_click_handler handler);

/* Lifecycle methods */

Checkbox* createCheckbox();
int gestureCheckbox(struct Checkbox* c, struct gesture_* gest);
void drawCheckbox(struct Checkbox* c);
void destroyCheckbox(struct Checkbox* c);

/************* Image component *************/

typedef struct Image {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	unsigned int image_id;
	int index_listed;
} Image;

/* Position and size */

void setImagePosition(struct Image* c, int x, int y);
position getImagePosition(struct Image* c);
void setImageSize(struct Image* c, int width, int height);
size getImageSize(struct Image* c);

/* View */

void setImageContent(struct Image* c, int index_listed, unsigned int resource_id);
unsigned int getImageContent(struct Image* c);

/* Lifecycle methods */

Image* createImage();
void drawImage(struct Image* c);
void destroyImage(struct Image* c);

/*********** Animation component ***********/

typedef struct Animation {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	int* frames;
	int frames_count;
	unsigned short current_frame;
	int index_listed;

	unsigned short loop;
} Animation;

/* Position and size */

void setAnimationPosition(struct Animation* c, int x, int y);
position getAnimationPosition(struct Animation* c);
void setAnimationSize(struct Animation* c, int width, int height);
size getAnimationSize(struct Animation* c);

/* View */

void setAnimationFrames(struct Animation* c, int index_listed, int count, ...);
int* getAnimationFrames(struct Animation* c);
int getAnimationFramesCount(struct Animation* c);

void setAnimationNextFrame(struct Animation* c);
void setAnimationPrevFrame(struct Animation* c);
void setAnimationCurrentFrame(struct Animation* c, unsigned short frame);
unsigned short getAnimationCurrentFrame(struct Animation* c);

void setAnimationLoop(struct Animation* c, unsigned short enabled);
unsigned short getAnimationLoop(struct Animation* c);

/* Lifecycle methods */

Animation* createAnimation();
void drawAnimation(struct Animation* c);
void destroyAnimation(struct Animation* c);