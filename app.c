#include "app.h"

struct regmenu_ screen_data = { 55,	1, 0, touch_handler, keypress_handler, update_screen, 0, init_app, 0, 0 };

struct app_data_t** app_data_p;
struct app_data_t* app_data;

// Entry point
int main(int return_screen, char** argv) {
	init_app((void*)return_screen);
}

// App init and memory allocation
void init_app(void* return_screen) {
	// This process commented in more details in the sources of original template for BipOS
	// https://github.com/MNVolkov/Template

	#ifdef BipEmulator
		app_data_p = (app_data_t**)get_ptr_temp_buf_2();
	#else
		app_data_p = get_ptr_temp_buf_2();
	#endif

	if ((return_screen == *app_data_p) && get_var_menu_overlay()) {
		app_data = *app_data_p;
		#ifdef BipEmulator
			* app_data_p = (app_data_t*)NULL;
		#else
			* app_data_p = NULL;
		#endif
		reg_menu(&screen_data, 0);
		*app_data_p = app_data;
	} else {
		reg_menu(&screen_data, 0);
		*app_data_p = (struct app_data_t*)pvPortMalloc(sizeof(struct app_data_t));
		app_data = *app_data_p;
		_memclr(app_data, sizeof(struct app_data_t));

		#ifdef BipEmulator
			app_data->proc = (Elf_proc_*)return_screen;
		#else
			app_data->proc = return_screen;
		#endif

		if (return_screen && app_data->proc->elf_finish)
			app_data->ret_f = app_data->proc->elf_finish;
		else
			app_data->ret_f = show_watchface;

		set_initial_values();
	}

	show_screen();
}

/****** Event handlers for components (touch on buttons, switches, checkboxes, etc.) ******/

// Button click handler on the start screen
void button_intro_handler(unsigned short x, unsigned short y) {
	app_data->currentScreen = 1;
	show_menu_animate(draw_screen, 0, ANIMATE_DOWN);
}

// Handlers for buttons with color change demo
void button_colors_1_handler(unsigned short x, unsigned short y) {
	// Increase the counter value
	app_data->button1Color++;

	// We have 3 color variations for this button (from 0 to 2)
	// If the value is greater than 2 we reset the counter to initial state
	if (app_data->button1Color > 2)
		app_data->button1Color = 0;

	// Depending on variable value set colors for the button
	switch (app_data->button1Color) {
		case 0:
			setButtonTextColor(app_data->buttonColors1, COLOR_WHITE);
			setButtonBorderColor(app_data->buttonColors1, COLOR_AQUA);
		break;
		case 1:
			setButtonTextColor(app_data->buttonColors1, COLOR_GREEN);
			setButtonBorderColor(app_data->buttonColors1, COLOR_WHITE);
			break;
		case 2:
			setButtonTextColor(app_data->buttonColors1, COLOR_YELLOW);
			setButtonBorderColor(app_data->buttonColors1, COLOR_PURPLE);
			break;
	}
}

void button_colors_2_handler(unsigned short x, unsigned short y) {
	// The same principle in this function
	app_data->button2Color++;

	if (app_data->button2Color > 4)
		app_data->button2Color = 0;

	switch (app_data->button2Color) {
		case 0:
			setButtonBackgroundColor(app_data->buttonColors2, COLOR_RED);
			setButtonBorderColor(app_data->buttonColors2, COLOR_RED);
			setButtonTextColor(app_data->buttonColors2, COLOR_WHITE);
			break;
		case 1:
			setButtonBackgroundColor(app_data->buttonColors2, COLOR_BLUE);
			setButtonBorderColor(app_data->buttonColors2, COLOR_BLUE);
			setButtonTextColor(app_data->buttonColors2, COLOR_WHITE);
			break;
		case 2:
			setButtonBackgroundColor(app_data->buttonColors2, COLOR_GREEN);
			setButtonBorderColor(app_data->buttonColors2, COLOR_GREEN);
			setButtonTextColor(app_data->buttonColors2, COLOR_BLACK);
			break;
		case 3:
			setButtonBackgroundColor(app_data->buttonColors2, COLOR_YELLOW);
			setButtonBorderColor(app_data->buttonColors2, COLOR_YELLOW);
			setButtonTextColor(app_data->buttonColors2, COLOR_BLACK);
			break;
		case 4:
			setButtonBackgroundColor(app_data->buttonColors2, COLOR_PURPLE);
			setButtonBorderColor(app_data->buttonColors2, COLOR_PURPLE);
			setButtonTextColor(app_data->buttonColors2, COLOR_WHITE);
			break;
	}
}

// Handler for button "Coords test"
void button_coords_handler(unsigned short x, unsigned short y) {
	// Memory allocation for text variable
	char* pText = (char*)pvPortMalloc(sizeof(char) * 20);

	// Set text with coordinates as variable value
	_sprintf(app_data->coordsText, "Click at %d, %d", x, y);

	// Set new value in application data from text variable
	app_data->coordsText = pText;
}


// Handlers for buttons with icons
void button_icons_1_handler(unsigned short x, unsigned short y) {
	// Change the "volume" value
	// Set a limit, so that value is never less than zero
	if (app_data->volumeValue > 0)
		app_data->volumeValue--;
}

void button_icons_2_handler(unsigned short x, unsigned short y) {
	// Change the "volume" value
	// Set a limit, so that value is never more than 100
	if (app_data->volumeValue < 100)
		app_data->volumeValue++;
}

// Handler for the first switch
void switch_1_handler(unsigned short enabled) {
	// Set variable value to new state of switch (true/false)
	app_data->switchHandlerValue = enabled;
}

/****** Application main code ******/

void set_initial_values() {
	// Here we create all components, which will be further used in the application

	/****** Button on intro screen ******/

	// Any component is created via the create[Component]() function
	// For example, createButton(), createCheckbox(), createAnimation() etc.
	Button* buttonIntro = createButton();

	// Set the position of the button - we pass the pointer to the component structure 
	// and set the coordinates along X and Y
	setButtonPosition(buttonIntro, 16, 106);	

	// Set the size, color, etc. in the same way
	setButtonSize(buttonIntro, 144, 50);					// Button size	
	setButtonBackgroundColor(buttonIntro, COLOR_BLUE);		// Background color
	setButtonBorderColor(buttonIntro, COLOR_AQUA);			// Border color
	setButtonHoverColor(buttonIntro, COLOR_YELLOW);			// Hover border color (after button clicked)
	setButtonText(buttonIntro, "Get started!");				// Text on button

	// Set click event handler by passing pointer to the function 
	setButtonClickHandler(buttonIntro, &button_intro_handler);

	// Setting pointer to the component structure in the application data variable
	app_data->buttonIntro = buttonIntro;

	/****** Two buttons on the screen with a demonstration of color change ******/

	Button* buttonColors1 = createButton();
	setButtonPosition(buttonColors1, 16, 40);
	setButtonSize(buttonColors1, 144, 50);
	setButtonText(buttonColors1, "Touch me");
	setButtonClickHandler(buttonColors1, &button_colors_1_handler);
	app_data->buttonColors1 = buttonColors1;

	Button* buttonColors2 = createButton();
	setButtonPosition(buttonColors2, 16, 106);
	setButtonSize(buttonColors2, 144, 50);
	setButtonText(buttonColors2, "Me too!");
	setButtonBackgroundColor(buttonColors2, COLOR_RED);
	setButtonBorderColor(buttonColors2, COLOR_RED);
	setButtonHoverColor(buttonColors2, COLOR_YELLOW);
	setButtonClickHandler(buttonColors2, &button_colors_2_handler);
	app_data->buttonColors2 = buttonColors2;

	// Counter variables for color change in button handlers
	app_data->button1Color = 0;
	app_data->button2Color = 0;

	/****** Demo of event handling with arguments (passing coordinates of touching the screen) ******/

	Button* buttonCoords = createButton();
	setButtonPosition(buttonCoords, 16, 56);
	setButtonSize(buttonCoords, 144, 100);
	setButtonText(buttonCoords, "Coordinates\ntest");
	setButtonBackgroundColor(buttonCoords, COLOR_BLUE);
	setButtonBorderColor(buttonCoords, COLOR_AQUA);
	setButtonHoverColor(buttonCoords, COLOR_YELLOW);
	setButtonClickHandler(buttonCoords, &button_coords_handler);
	app_data->buttonCoords = buttonCoords;

	// The text that is drawn before the button is pressed
	// After clicking, it is updating to "Click at {X}, {Y}"
	app_data->coordsText = "Press button";

	/****** Button with icons ******/

	Button* buttonIcons1 = createButton();
	setButtonPosition(buttonIcons1, 16, 90);
	setButtonSize(buttonIcons1, 62, 40);
	setButtonBackgroundColor(buttonIcons1, COLOR_BLACK);

	// Function for setting icon to the button
	setButtonIcon(
		buttonIcons1,		   // Pointer to component structure

		ELF_INDEX_SELF,		   // Use ELF_INDEX_SELF constant if we want to use resources built into the firmware

							   // If you need to take assets from the application resource file (asset.res)
							   // you have to use the index_listed value

		896 				   // Resource ID
							   // 896 is speaker icon with a minus sign
	);

	setButtonClickHandler(buttonIcons1, &button_icons_1_handler);
	app_data->buttonIcons1 = buttonIcons1;

	Button* buttonIcons2 = createButton();
	setButtonPosition(buttonIcons2, 96, 90);
	setButtonSize(buttonIcons2, 62, 40);
	setButtonBackgroundColor(buttonIcons2, COLOR_BLACK);
	setButtonIcon(buttonIcons2, ELF_INDEX_SELF, 898);
	setButtonClickHandler(buttonIcons2, &button_icons_2_handler);
	app_data->buttonIcons2 = buttonIcons2;

	// The initial "volume" value
	// This variable used to demonstrate changing value
	app_data->volumeValue = 50;

	/****** Switches ******/

	Switch* switch1 = createSwitch();
	setSwitchPosition(switch1, 16, 70);
	setSwitchSize(switch1, 144, 32);
	setSwitchText(switch1, "Switch 1");
	setSwitchClickHandler(switch1, &switch_1_handler);
	app_data->switch1 = switch1;

	Switch* switch2 = createSwitch();
	setSwitchPosition(switch2, 16, 125);
	setSwitchSize(switch2, 144, 32);
	setSwitchText(switch2, "Switch 2");
	app_data->switch2 = switch2;

	app_data->switchHandlerValue = false;

	/****** Checkboxes ******/

	Checkbox* checkbox1 = createCheckbox();
	setCheckboxPosition(checkbox1, 8, 40);
	setCheckboxSize(checkbox1, 152, 28);
	setCheckboxText(checkbox1, "Buy eggs");
	app_data->checkbox1 = checkbox1;

	Checkbox* checkbox2 = createCheckbox();
	setCheckboxPosition(checkbox2, 8, 85);
	setCheckboxSize(checkbox2, 152, 28);
	setCheckboxText(checkbox2, "Buy milk");
	app_data->checkbox2 = checkbox2;

	Checkbox* checkbox3 = createCheckbox();
	setCheckboxPosition(checkbox3, 8, 130);
	setCheckboxSize(checkbox3, 152, 28);
	setCheckboxText(checkbox3, "Make pancakes");
	app_data->checkbox3 = checkbox3;

	/****** Images ******/

	Image* image1 = createImage();
	setImagePosition(image1, 22, 40);
	setImageSize(image1, 45, 100);
	setImageContent(image1, app_data->proc->index_listed, 0);
	app_data->image1 = image1;

	Image* image2 = createImage();
	setImagePosition(image2, 106, 40);
	setImageSize(image2, 40, 40);
	setImageContent(image2, ELF_INDEX_SELF, 251);
	app_data->image2 = image2;

	Image* image3 = createImage();
	setImagePosition(image3, 106, 100);
	setImageSize(image3, 40, 40);
	setImageContent(image3, ELF_INDEX_SELF, 257);
	app_data->image3 = image3;

	/****** Animation ******/

	Animation* animationDemo = createAnimation();
	setAnimationPosition(animationDemo, 63, 63);
	setAnimationSize(animationDemo, 50, 50);

	// Function for setting animation frames
	setAnimationFrames(
		animationDemo,		// Pointer to component structure

		ELF_INDEX_SELF,		// Use ELF_INDEX_SELF constant if we want to use resources built into the firmware

							// If you need to take assets from the application resource file (asset.res)
							// you have to use the index_listed value

		7,					// Animation frames count

		// Specify the IDs of the resources that need to be added to animation frames
		525, 524, 523, 522, 521, 520, 519
	);
	app_data->animationDemo = animationDemo;

	app_data->animationActive = false;

	/****** Animation for the last screen ******/

	Animation* animationHeart = createAnimation();
	setAnimationPosition(animationHeart, 72, 90);
	setAnimationSize(animationHeart, 32, 32);
	setAnimationFrames(animationHeart, ELF_INDEX_SELF, 2,
		98, 99
	);
	app_data->animationHeart = animationHeart;

	// Setting start screen
	app_data->currentScreen = SCREEN_INTRO;
}

void show_screen() {
	load_font();

	// Do not exit the application when idle
	set_display_state_value(8, 1);

	// Initial screen rendering
	draw_screen();
	repaint_screen_lines(1, 176);
}

// Function to handle pressing the "Back" button
void keypress_handler() {
	// Before exiting the application, destroy all previously created components
	// This is necessary to free memory in the watch, otherwise we will get a memory leak
	destroyButton(app_data->buttonIntro);

	destroyButton(app_data->buttonColors1);
	destroyButton(app_data->buttonColors2);

	destroyButton(app_data->buttonCoords);

	destroyButton(app_data->buttonIcons1);
	destroyButton(app_data->buttonIcons2);

	destroySwitch(app_data->switch1);
	destroySwitch(app_data->switch2);

	destroyCheckbox(app_data->checkbox1);
	destroyCheckbox(app_data->checkbox2);
	destroyCheckbox(app_data->checkbox3);

	destroyImage(app_data->image1);
	destroyImage(app_data->image2);
	destroyImage(app_data->image3);

	destroyAnimation(app_data->animationDemo);

	destroyAnimation(app_data->animationHeart);

	// Returning the default screen setting
	set_display_state_value(8, 0);

	// Exiting from the application by return address
	show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

// Timer function which updates the necessary components depending on the current screen
// In the original BipOS template this function is named screen_job()
void update_screen() {
	// This function is called some time after touching the screen (working off the touch_handler function)
	// We use set_update_period(...) function for set a timer with a short delay

	// This is where the "update" of the components takes place. This is necessary, for example, for reset 
	// the active buttons border color after clicking on them. At the moment, only button components require update

	// Also, in this function we implements the processing of animation components

	// Update components we need depending on the current screen
	switch (app_data->currentScreen) {
		case SCREEN_INTRO:
			updateButton(app_data->buttonIntro);
			break;
		case SCREEN_BUTTON_STYLES:
			updateButton(app_data->buttonColors1);
			updateButton(app_data->buttonColors2);
			break;
		case SCREEN_BUTTON_COORDS:
			updateButton(app_data->buttonCoords);
			break;
		case SCREEN_BUTTON_ICONS:
			updateButton(app_data->buttonIcons1);
			updateButton(app_data->buttonIcons2);
			break;

		// If we have one of the screens that contains the animation components, we call the function 
		// that make animation transition to the next frame
		case SCREEN_ANIMATION:
			setAnimationNextFrame(app_data->animationDemo);
			break;
		case SCREEN_BYE:
			setAnimationNextFrame(app_data->animationHeart);
			break;
	}

	// Rendering screen after components update
	draw_screen();
	repaint_screen_lines(1, 176);

	// If one of the screens with animation is currently active, set the timer to change the animation frame
	// to the next one
	if (app_data->currentScreen == SCREEN_ANIMATION)
		set_update_period(1, ANIMATION_DEMO_DELAY);
	else if (app_data->currentScreen == SCREEN_BYE)
		set_update_period(1, ANIMATION_HEART_DELAY);
	else
		// If this is a different screen, stop the timer, because we need to update components only once
		// after interacting with them
		set_update_period(0, FASTUI_UPDATE_DELAY);
}

// Function for handling clicks and gestures on screen
// In the original BipOS template this function is named dispatch_screen()
void touch_handler(void* param) {
	#ifdef BipEmulator
		struct gesture_* gest = (gesture_*)param;
	#else
		struct gesture_* gest = param;
	#endif

	// Call the gesture handling method for components we need, depending on the current screen
	switch (app_data->currentScreen) {
		case SCREEN_INTRO:
			// To process a button click, you dont need to do any additional checks such as
			// check coordinates of click on screen, etc.

			// All you need to do is call the gestureButton() method, passing in a pointer to component
			// structure, and a pointer to the gesture struct. The component will determine whether an 
			// interaction has been made with it and call the click handler/change its state automatically

			// Very simple and convenient!
			gestureButton(app_data->buttonIntro, gest);
			break;
		case SCREEN_BUTTON_STYLES:
			gestureButton(app_data->buttonColors1, gest);
			gestureButton(app_data->buttonColors2, gest);
			break;
		case SCREEN_BUTTON_COORDS:
			gestureButton(app_data->buttonCoords, gest);
			break;
		case SCREEN_BUTTON_ICONS:
			gestureButton(app_data->buttonIcons1, gest);
			gestureButton(app_data->buttonIcons2, gest);
			break;
		case SCREEN_SWITCHES:
			gestureSwitch(app_data->switch1, gest);
			gestureSwitch(app_data->switch2, gest);
			break;
		case SCREEN_CHECKBOXES:
			gestureCheckbox(app_data->checkbox1, gest);
			gestureCheckbox(app_data->checkbox2, gest);
			gestureCheckbox(app_data->checkbox3, gest);
			break;
	}

	// Additional gesture handling separate from components
	switch (gest->gesture) {
		case GESTURE_CLICK:
			// If a click was made, we will render screen to show the changes to the user 
			// For example, if user pressed a button or a switch
			draw_screen();
			repaint_screen_lines(1, 176);

			// In addition to rendering, we will set a timer to update the components in a short time
			// For example, reset the active state of buttons after clicking on them
			set_update_period(1, FASTUI_UPDATE_DELAY);
			break;

		case GESTURE_SWIPE_RIGHT:
			break;

		case GESTURE_SWIPE_LEFT:
			break;

		// Handling screen transitions via swipes
		case GESTURE_SWIPE_UP:
			if (app_data->currentScreen < SCREENS_COUNT - 1) {
				app_data->currentScreen++;
				show_menu_animate(draw_screen, 0, ANIMATE_UP);
			}

			#ifdef BipEmulator
				draw_screen();
				repaint_screen_lines(1, 176);
			#endif
			break;

		case GESTURE_SWIPE_DOWN:
			if (app_data->currentScreen > 0) {
				app_data->currentScreen--;
				show_menu_animate(draw_screen, 0, ANIMATE_DOWN);
			}

			#ifdef BipEmulator
				draw_screen();
				repaint_screen_lines(1, 176);
			#endif
			break;
	}

	// If we switched to one of the screens with animation, we will set a timer
	if (app_data->currentScreen == SCREEN_ANIMATION) {
		// We have different delays for different screens with animation
		// To make the loading animation fast and the heart animation slow
		set_update_period(1, ANIMATION_DEMO_DELAY);

		// Set the flag that we went to the screen with animation
		app_data->animationActive = true;
	} else if (app_data->currentScreen == SCREEN_BYE) {
		set_update_period(1, ANIMATION_HEART_DELAY);
		app_data->animationActive = true;
	} else if (app_data->animationActive) {
		// If we switched to the normal screen from the screen with animation, we will reset the timer
		set_update_period(0, FASTUI_UPDATE_DELAY);
		app_data->animationActive = false;
	}

}

// Function for work with graphics and rendering screen
void draw_screen() {
	// Fill the screen with the colors specified in the constants in header file of the library
	set_fg_color(FASTUI_BASE_FG_COLOR);
	set_bg_color(FASTUI_BASE_BG_COLOR);
	fill_screen_bg();

	// This is necessary to avoid conflicts with the colors of the components after graphics rendering,
	// since the library after rendering the component sets the text color to the one that was set earlier

	// Variable for storing text and "volume" value
	char volume[12];

	// Drawing components we need depending on the current screen
	switch (app_data->currentScreen) {
		case SCREEN_INTRO:
			// The component rendering process can be mixed with the standard graphics functions from libbip
			text_out_center("FastUI demo\napplication", 88, 12);
			text_out_center("Swipe up or press\nbutton below", 88, 54);

			// You can only use components where you really need them to save RAM and application performance

			// To draw a button you just need to call the drawButton() method and pass a pointer to the
			// component structure
			drawButton(app_data->buttonIntro);
			break;
		case SCREEN_BUTTON_STYLES:
			text_out_center("Buttons", 88, 12);

			drawButton(app_data->buttonColors1);
			drawButton(app_data->buttonColors2);
			break;
		case SCREEN_BUTTON_COORDS:
			text_out_center(app_data->coordsText, 88, 20);

			drawButton(app_data->buttonCoords);
			break;
		case SCREEN_BUTTON_ICONS:
			text_out_center("Button /w icons", 88, 12);

			_sprintf(volume, "Volume: %d", app_data->volumeValue);
			text_out_center(volume, 88, 60);

			drawButton(app_data->buttonIcons1);
			drawButton(app_data->buttonIcons2);
			break;
		case SCREEN_SWITCHES:
			text_out_center("Switches", 88, 12);

			// Here we use the value that we assign in the event handler when the first switch is clicked
			set_fg_color(app_data->switchHandlerValue == true ? COLOR_GREEN : COLOR_RED);
			text_out_center(app_data->switchHandlerValue == true ? "SW1: On" : "SW1: Off", 44, 34);

			// And here we take the 'enabled' property directly from the component
			set_fg_color(app_data->switch2->enabled == true ? COLOR_GREEN : COLOR_RED);
			text_out_center(app_data->switch2->enabled == true ? "SW2: On" : "SW2: Off", 132, 34);
			
			drawSwitch(app_data->switch1);
			drawSwitch(app_data->switch2);
			break;
		case SCREEN_CHECKBOXES:
			text_out_center("Checkboxes", 88, 10);

			drawCheckbox(app_data->checkbox1);
			drawCheckbox(app_data->checkbox2);
			drawCheckbox(app_data->checkbox3);
			break;
		case SCREEN_IMAGES:
			text_out_center("Images", 88, 10);

			drawImage(app_data->image1);
			drawImage(app_data->image2);
			drawImage(app_data->image3);

			text_out_center("App res", 45, 150);
			text_out_center("FW res", 126, 150);
			break;
		case SCREEN_ANIMATION:
			text_out_center("Animation", 88, 10);

			drawAnimation(app_data->animationDemo);
			break;
		case SCREEN_BYE:
			text_out_center("Hope this library\nmakes coding easer\nfor you", 88, 10);

			drawAnimation(app_data->animationHeart);

			text_out_center("- Yuukari", 88, 145);
			break;
	}
}