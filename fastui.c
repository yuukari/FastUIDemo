#include "fastui.h"

/************* Button component *************/

Button* createButton() {
	Button* button = (Button*)pvPortMalloc(sizeof(Button));

	button->x = 16;
	button->y = 16;
	button->width = 120;
	button->height = 50;
	button->background_color = COLOR_BLACK;
	button->border_color = COLOR_AQUA;
	button->hover_color = COLOR_YELLOW;
	button->text_color = COLOR_WHITE;

	button->text = "Button";
	button->icon_id = ICON_EMPTY;

	button->is_pressed = false;

	#ifdef BipEmulator
		button->click_handler = nullptr;
	#else
		button->click_handler = NULL;
	#endif

	return button;
}

void setButtonPosition(struct Button* c, int x, int y) {
	c->x = x;
	c->y = y;
}

position getButtonPosition(struct Button* c) {
	position _position = {
		c->x,
		c->y
	};

	return _position;
}

void setButtonSize(struct Button* c, int width, int height) {
	c->width = width;
	c->height = height;
}

size getButtonSize(struct Button* c) {
	size _size = {
		c->width,
		c->height
	};

	return _size;
}

void setButtonBackgroundColor(struct Button* c, unsigned int color) {
	c->background_color = color;
}

unsigned int getButtonBackgroundColor(struct Button* c) {
	return c->background_color;
}

void setButtonBorderColor(struct Button* c, unsigned int color) {
	c->border_color = color;
}

unsigned int getButtonBorderColor(struct Button* c) {
	return c->border_color;
}

void setButtonHoverColor(struct Button* c, unsigned int color) {
	c->hover_color = color;
}

unsigned int getButtonHoverColor(struct Button* c) {
	return c->hover_color;
}

void setButtonTextColor(struct Button* c, unsigned int color) {
	c->text_color = color;
}

unsigned int getButtonTextColor(struct Button* c) {
	return c->text_color;
}

void setButtonText(struct Button* c, char* text) {
	c->text = text;
}

char* getButtonText(struct Button* c) {
	return c->text;
}

void setButtonIcon(struct Button* c, int index_listed, unsigned int resource_id) {
	if (index_listed == ELF_INDEX_SELF)
		c->index_listed = INDEX_MAIN_RES;
	else
		c->index_listed = c->index_listed = index_listed;

	c->icon_id = resource_id;
}

unsigned int getButtonIcon(struct Button* c) {
	return c->icon_id;
}

void setButtonClickHandler(struct Button* c, button_click_handler handler) {
	c->click_handler = handler;
}

int gestureButton(struct Button* c, struct gesture_* gest) {
	button_click_handler clickHandler = c->click_handler;

	switch (gest->gesture) {
		case GESTURE_CLICK:
			if (
				gest->touch_pos_x >= c->x && gest->touch_pos_x <= c->x + c->width &&
				gest->touch_pos_y >= c->y && gest->touch_pos_y <= c->y + c->height
			){
				c->is_pressed = true;

				#ifdef BipEmulator
					if (clickHandler != nullptr)
				#else
					if (clickHandler != NULL)
				#endif
						clickHandler(gest->touch_pos_x - c->x, gest->touch_pos_y - c->y);

				return true;
			}
			break;
	}

	return false;
}

void updateButton(struct Button* c) {
	c->is_pressed = false;
}

void drawButton(struct Button* c) {
	set_bg_color(c->background_color);
	draw_filled_rect_bg(
		c->x,
		c->y,
		c->x + c->width,
		c->y + c->height
	);

	set_fg_color(c->is_pressed ? c->hover_color : c->border_color);
	draw_rect(
		c->x,
		c->y,
		c->x + c->width,
		c->y + c->height
	);

	if (c->icon_id == ICON_EMPTY) {
		int textLength = _strlen(c->text);
		int textHeight = get_text_height();

		for (unsigned int i = 0; i < textLength; i++)
			if (c->text[i] == '\n')
				textHeight += get_text_height();

		set_fg_color(c->text_color);
		text_out_center(
			c->text,
			c->x + c->width / 2,
			c->y + (c->height / 2) - textHeight / 2
		);
	} else {
		struct res_params_ res_params;

		if (c->index_listed == ELF_INDEX_SELF) {
			get_res_params(INDEX_MAIN_RES, c->icon_id, &res_params);
			show_res_by_id(
				c->icon_id,
				c->x + (c->width / 2) - (res_params.width / 2),
				c->y + (c->height / 2) - (res_params.height / 2)
			);
		} else {
			get_res_params(c->index_listed, c->icon_id, &res_params);
			show_elf_res_by_id(
				c->index_listed,
				c->icon_id,
				c->x + (c->width / 2) - (res_params.width / 2),
				c->y + (c->height / 2) - (res_params.height / 2)
			);
		}
	}

	#ifdef FASTUI_BASE_FG_COLOR
		set_fg_color(FASTUI_BASE_FG_COLOR);
	#endif

	#ifdef FASTUI_BASE_BG_COLOR
		set_bg_color(FASTUI_BASE_BG_COLOR);
	#endif
}

void destroyButton(struct Button* c) {
	#ifndef BipEmulator
		vPortFree(c);
	#endif
}

/************* Switch component *************/

Switch* createSwitch() {
	Switch* _switch = (Switch*)pvPortMalloc(sizeof(Switch));

	_switch->x = 16;
	_switch->y = 16;
	_switch->width = 120;
	_switch->height = 35;
	_switch->text_color = COLOR_WHITE;
	_switch->text = "Switch";
	_switch->text_padding = 10;
	_switch->enabled = false;

	#ifdef BipEmulator
		_switch->click_handler = nullptr;
	#else
		_switch->click_handler = NULL;
	#endif

	return _switch;
}

void setSwitchPosition(struct Switch* c, int x, int y) {
	c->x = x;
	c->y = y;
}

position getSwitchPosition(struct Switch* c) {
	position _position = {
		c->x,
		c->y
	};

	return _position;
}

void setSwitchSize(struct Switch* c, int width, int height) {
	c->width = width;
	c->height = height;
}

size getSwitchSize(struct Switch* c) {
	size _size = {
		c->width,
		c->height
	};

	return _size;
}

void setSwitchTextColor(struct Switch* c, unsigned int color) {
	c->text_color = color;
}

unsigned int getSwitchTextColor(struct Switch* c) {
	return c->text_color;
}

void setSwitchText(struct Switch* c, char* text) {
	c->text = text;
}

char* getSwitchText(struct Switch* c) {
	return c->text;
}

void setSwitchClickHandler(struct Switch* c, switch_click_handler handler) {
	c->click_handler = handler;
}

int gestureSwitch(struct Switch* c, struct gesture_* gest) {
	switch_click_handler clickHandler = c->click_handler;

	switch (gest->gesture) {
		case GESTURE_CLICK:
			if (
				gest->touch_pos_x >= c->x - 10 && gest->touch_pos_x <= c->x + c->width + 10 &&
				gest->touch_pos_y >= c->y - 10 && gest->touch_pos_y <= c->y + c->height + 10
			){
				c->enabled = !c->enabled;
				
				#ifdef BipEmulator
					if (clickHandler != nullptr)
				#else
					if (clickHandler != NULL)
				#endif
						clickHandler(c->enabled);
				
				return true;
			}
		break;
	}

	return false;
}

void drawSwitch(struct Switch* c) {
	show_res_by_id(c->enabled ? ICON_TOGGLE_ON : ICON_TOGGLE_OFF, c->x, c->y + (c->height / 2) - 13);

	set_bg_color(COLOR_BLACK);
	set_fg_color(c->text_color);
	text_out(c->text, c->x + 53 + c->text_padding, c->y + (c->height / 2) - (get_text_height() / 2));

	#ifdef FASTUI_BASE_FG_COLOR
		set_fg_color(FASTUI_BASE_FG_COLOR);
	#endif

	#ifdef FASTUI_BASE_BG_COLOR
		set_bg_color(FASTUI_BASE_BG_COLOR);
	#endif
}

void destroySwitch(struct Switch* c) {
	#ifndef BipEmulator
		vPortFree(c);
	#endif
}

/************ Checkbox component ************/

Checkbox* createCheckbox() {
	Checkbox* checkbox = (Checkbox*)pvPortMalloc(sizeof(Checkbox));
	checkbox->x = 16;
	checkbox->y = 16;
	checkbox->width = 120;
	checkbox->height = 35;
	checkbox->text_color = COLOR_WHITE;
	checkbox->text = "Checkbox";
	checkbox->text_padding = 10;
	checkbox->enabled = false;

	#ifdef BipEmulator
		checkbox->click_handler = nullptr;
	#else
		checkbox->click_handler = NULL;
	#endif

	return checkbox;
}

void setCheckboxPosition(struct Checkbox* c, int x, int y) {
	c->x = x;
	c->y = y;
}

position getCheckboxPosition(struct Checkbox* c) {
	position _position = {
		c->x,
		c->y
	};

	return _position;
}

void setCheckboxSize(struct Checkbox* c, int width, int height) {
	c->width = width;
	c->height = height;
}

size getCheckboxSize(struct Checkbox* c) {
	size _size = {
		c->width,
		c->height
	};

	return _size;
}

void setCheckboxTextColor(struct Checkbox* c, unsigned int color) {
	c->text_color = color;
}

unsigned int getCheckboxTextColor(struct Checkbox* c) {
	return c->text_color;
}

void setCheckboxText(struct Checkbox* c, char* text) {
	c->text = text;
}

char* getCheckboxText(struct Checkbox* c) {
	return c->text;
}

void setCheckboxClickHandler(struct Checkbox* c, checkbox_click_handler handler) {
	c->click_handler = handler;
}

int gestureCheckbox(struct Checkbox* c, struct gesture_* gest) {
	checkbox_click_handler clickHandler = c->click_handler;

	switch (gest->gesture) {
		case GESTURE_CLICK:
			if (
				gest->touch_pos_x >= c->x - 10 && gest->touch_pos_x <= c->x + c->width + 10 &&
				gest->touch_pos_y >= c->y - 10 && gest->touch_pos_y <= c->y + c->height + 10
			){
				c->enabled = !c->enabled;
				
				#ifdef BipEmulator
					if (clickHandler != nullptr)
				#else
					if (clickHandler != NULL)
				#endif
						clickHandler(c->enabled);
				
				return true;
			}
		break;
	}

	return false;
}

void drawCheckbox(struct Checkbox* c) {
	show_res_by_id(c->enabled ? CHECK_BOX_ON : CHECK_BOX_OFF, c->x, c->y + (c->height / 2) - 12);

	set_bg_color(COLOR_BLACK);
	set_fg_color(c->text_color);
	text_out(c->text, c->x + 25 + c->text_padding, c->y + (c->height / 2) - (get_text_height() / 2));

	#ifdef FASTUI_BASE_FG_COLOR
		set_fg_color(FASTUI_BASE_FG_COLOR);
	#endif

	#ifdef FASTUI_BASE_BG_COLOR
		set_bg_color(FASTUI_BASE_BG_COLOR);
	#endif
}

void destroyCheckbox(struct Checkbox* c) {
	#ifndef BipEmulator
		vPortFree(c);
	#endif
}

/************* Image component *************/

Image* createImage() {
	Image* image = (Image*)pvPortMalloc(sizeof(Image));

	image->x = 16;
	image->y = 16;
	image->width = 60;
	image->height = 60;

	image->image_id = ICON_EMPTY;

	return image;
}

void setImagePosition(struct Image* c, int x, int y) {
	c->x = x;
	c->y = y;
}

position getImagePosition(struct Image* c) {
	position _position = {
		c->x,
		c->y
	};

	return _position;
}

void setImageSize(struct Image* c, int width, int height) {
	c->width = width;
	c->height = height;
}

size getImageSize(struct Image* c) {
	size _size = {
		c->width,
		c->height
	};

	return _size;
}

void setImageContent(struct Image* c, int index_listed, unsigned int resource_id) {
	if (index_listed == ELF_INDEX_SELF)
		c->index_listed = INDEX_MAIN_RES;
	else
		c->index_listed = c->index_listed = index_listed;

	c->image_id = resource_id;
}

unsigned int getImageContent(struct Image* c) {
	return c->image_id;
}

void drawImage(struct Image* c) {
	if (c->image_id != ICON_EMPTY) {
		struct res_params_ res_params;

		if (c->index_listed == ELF_INDEX_SELF) {
			get_res_params(INDEX_MAIN_RES, c->image_id, &res_params);
			show_res_by_id(
				c->image_id,
				c->x + (c->width / 2) - (res_params.width / 2),
				c->y + (c->height / 2) - (res_params.height / 2)
			);
		} else {
			get_res_params(c->index_listed, c->image_id, &res_params);
			show_elf_res_by_id(
				c->index_listed,
				c->image_id,
				c->x + (c->width / 2) - (res_params.width / 2),
				c->y + (c->height / 2) - (res_params.height / 2)
			);
		}
	}
}

void destroyImage(struct Image* c) {
	#ifndef BipEmulator
		vPortFree(c);
	#endif
}

/*********** Animation component ***********/

Animation* createAnimation() {
	Animation* animation = (Animation*)pvPortMalloc(sizeof(Animation));

	animation->x = 16;
	animation->y = 16;
	animation->width = 60;
	animation->height = 60;

	#ifdef BipEmulator
		animation->frames = nullptr;
	#else
		animation->frames = NULL;
	#endif
	animation->frames_count = 0;
	animation->current_frame = 0;
	animation->index_listed = ELF_INDEX_SELF;

	animation->loop = true;

	return animation;
}

void setAnimationPosition(struct Animation* c, int x, int y) {
	c->x = x;
	c->y = y;
}

position getAnimationPosition(struct Animation* c) {
	position _position = {
		c->x,
		c->y
	};

	return _position;
}

void setAnimationSize(struct Animation* c, int width, int height) {
	c->width = width;
	c->height = height;
}

size getAnimationSize(struct Animation* c) {
	size _size = {
		c->width,
		c->height
	};

	return _size;
}

void setAnimationFrames(struct Animation* c, int index_listed, int count, ...) {
	if (count == 0)
		return;

	c->frames_count = count;
	c->current_frame = 0;
	c->index_listed = index_listed;

	#ifndef BipEmulator
		vPortFree(c->frames);
	#endif

	c->frames = (int*)pvPortMalloc(sizeof(int) * count);
	unsigned short i = 0;

	for (int* ptr = &count; count > 0; count--) {
		c->frames[i] = *(++ptr);
		i++;
	}
}

int* getAnimationFrames(struct Animation* c) {
	return c->frames;
}

int getAnimationFramesCount(struct Animation* c) {
	return c->frames_count;
}

void setAnimationNextFrame(struct Animation* c) {
	c->current_frame++;

	if (c->current_frame > c->frames_count - 1) {
		if (c->loop)
			c->current_frame = 0;
		else
			c->current_frame = c->frames_count - 1;
	}
}

void setAnimationPrevFrame(struct Animation* c) {
	c->current_frame--;

	if (c->current_frame < 0) {
		if (c->loop)
			c->current_frame = c->frames_count - 1;
		else
			c->current_frame = 0;
	}
}

void setAnimationCurrentFrame(struct Animation* c, unsigned short frame) {
	c->current_frame = frame;
}

unsigned short getAnimationCurrentFrame(struct Animation* c) {
	return c->current_frame;
}

void setAnimationLoop(struct Animation* c, unsigned short enabled) {
	c->loop = enabled;
}

unsigned short getAnimationLoop(struct Animation* c) {
	return c->loop;
}

void drawAnimation(struct Animation* c) {
	if (c->frames_count > 0) {
		struct res_params_ res_params;

		if (c->index_listed == ELF_INDEX_SELF) {
			get_res_params(INDEX_MAIN_RES, c->frames[c->current_frame], &res_params);
			show_res_by_id(
				c->frames[c->current_frame],
				c->x + (c->width / 2) - (res_params.width / 2),
				c->y + (c->height / 2) - (res_params.height / 2)
			);
		} else {
			get_res_params(c->index_listed, c->frames[c->current_frame], &res_params);
			show_elf_res_by_id(
				c->index_listed,
				c->frames[c->current_frame],
				c->x + (c->width / 2) - (res_params.width / 2),
				c->y + (c->height / 2) - (res_params.height / 2)
			);
		}
	}
}

void destroyAnimation(struct Animation* c) {
	#ifndef BipEmulator
		vPortFree(c->frames);
	#endif

	#ifndef BipEmulator
		vPortFree(c);
	#endif
}