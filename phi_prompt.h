/** \file
 *  \brief     This is an official release of the phi_prompt text-based user interface library for arduino 1.0.
 *  \details   This library requires 1.0 version of phi_interfaces library.
 *  \author    Dr. John Liu
 *  \version   1.6.0.2
 *  \date      02/17/2017
 *  \pre       Compatible with Arduino IDE 1.6.
 *  \bug       Not tested on Arduino IDE higher than 1.6.0!
 *  \warning   PLEASE DO NOT REMOVE THIS COMMENT WHEN REDISTRIBUTING! No warranty!
 *  \copyright Dr. John Liu. GNU GPL V 3.0.
 *  \par Contact
 * Obtain the documentation or find details of the phi_interfaces, phi_prompt TUI library, Phi-2 shield, and Phi-panel hardware or contact Dr. Liu at:
 *
 * <a href="http://liudr.wordpress.com/phi_interfaces/">http://liudr.wordpress.com/phi_interfaces/</a>
 *
 * <a href="http://liudr.wordpress.com/phi-panel/">http://liudr.wordpress.com/phi-panel/</a>
 *
 * <a href="http://liudr.wordpress.com/phi_prompt/">http://liudr.wordpress.com/phi_prompt/</a>
 *
 * <a href="http://liudr.wordpress.com/phi-2-shield/">http://liudr.wordpress.com/phi-2-shield/</a>
 *  \par Library dependency
 * This library requires phi_interfaces library version 1.0
 *
 *  \par Update
 * 02/17/2017: Modified #include and function definitions to use LCD.h from Francisco Malpartida. Replaced LiquidCrystal with LCD.
 * 08/10/2015: Added some define keywords to remove magic numbers.
 * 07/07/2015: Added two more glyphs, music and Lux
 * 05/28/2015: Released under GNU GPL V 3.0 Yeah!
 * 02/12/2015: Removed PROGMEM prog_char and replaced with const char PROGMEM
 * 07/30/2014: Added #ifndef to the library. 
 * 12/22/2012: Moved simple_setup function declarations inside disable_simple_fns. Added that def in this file since including it in phi-panel firmware won't do any good.
 * 11/25/2012: Added def simple_select_list_max_items
 * 11/24/2012: Added 3 simple_setup function declarations
 * 12/18/2011: Modified with #if and (uint8_t) to run on both arduino IDE 1.0 and pre-1.0 versions
 * 05/23/2011: First official release of the library compatible with Arduino IDE 0022. Needs phi_buttons library as the physical layer
 *
 * \par Function keys
 * The roles of the function keys are different in different inputs.
 * \par Up
 * Scroll one line up in long message. Increment one character in place in input panel or input number. Move one item up in the select list.
 * \par Down
 * Scroll one line down in long message. Decrement one character in place in input panel or input number. Move one item down in the select list.
 * \par Left
 * Scroll one page up in long message. Move one character to the left in input panel or input number. Move one column to the left in the select list.
 * \par Right
 * Scroll one page down in long message. Move one character to the right in input panel or input number. Move one item column to the right in the select list.
 * \par Enter
 * Dismiss a long message, return 1. Submit result in input panel or input number and dismiss the input, return 1. Select the highlighted item in the select list and dismiss the list.
 * \par Escape
 * Dismiss a long message, return -1. Dismiss the input and return -1. Dismiss the list and return -1.
 * \par Other keys
 * All other keys are passed to input without change.
 * Select_list() will discard any key other than a function key.
 * Input_panel() will accept any key into its string and treats \n as Enter and \b as back space function besides treating above 6 function keys.
 * Input_number() only accepts numeric keys into its string and treats \n as Enter and \b as back space function besides treating above 6 function keys.
 * Input_integer() will only accept function keys and discards any other keys.
 * Text_area() will only accept function keys and numeric keys and discards any other keys.
 */
/*
.______    __    __   __        .______   .______        ______   .___  ___. .______   .___________.
|   _  \  |  |  |  | |  |       |   _  \  |   _  \      /  __  \  |   \/   | |   _  \  |           |
|  |_)  | |  |__|  | |  |       |  |_)  | |  |_)  |    |  |  |  | |  \  /  | |  |_)  | `---|  |----`
|   ___/  |   __   | |  |       |   ___/  |      /     |  |  |  | |  |\/|  | |   ___/      |  |     
|  |      |  |  |  | |  |       |  |      |  |\  \----.|  `--'  | |  |  |  | |  |          |  |     
| _|      |__|  |__| |__|  _____| _|      | _| `._____| \______/  |__|  |__| | _|          |__|     

 _______  .______                 __       __   __    __  
|       \ |   _  \               |  |     |  | |  |  |  | 
|  .--.  ||  |_)  |              |  |     |  | |  |  |  | 
|  |  |  ||      /               |  |     |  | |  |  |  | 
|  '--'  ||  |\  \----.    __    |  `----.|  | |  `--'  | 
|_______/ | _| `._____|   (__)   |_______||__|  \______/  

http://liudr.wordpress.com
*/
#ifndef phi_prompt_h
#define phi_prompt_h
// To disable the simple functions to save some SRAM, uncomment the next line.
//#define disable_simple_fns

//The following are switches to certain functions. Comment them out if you don't want a particular function to save program space for larger projects
//#define scrolling // This turns on auto strolling on list items and includes scrolling text library function.
// Render list option bits
#define phi_prompt_arrow_dot B00000001      ///< List display option for using arrow/dot before a list item.
#define phi_prompt_index_list B00000010     ///< List display option for using an index list such as 12*4 for 4 total items and 3 is highlighted.
#define phi_prompt_current_total B00000100  ///< List display option for using a current/total index such as 2/4.
#define phi_prompt_auto_scroll B00001000    ///< List display option for using auto scrolling items longer than the width of the list.
#define phi_prompt_flash_cursor B00010000   ///< List display option for using flash cursor as indicator of highlighted item.
#define phi_prompt_center_choice B00100000  ///< List display option for using centering highlighted item on screen so highlighted item is always in the middle when possible.
#define phi_prompt_scroll_bar B01000000     ///< List display option for using a scroll bar on the right.
#define phi_prompt_invert_text B10000000    ///< List display option for using inverted text. Only some modified version of the library uses it.
#define phi_prompt_list_in_SRAM 0x100       ///< List display option for using a list that is stored in SRAM instead of in PROGMEM.
#define phi_prompt_update_function 0x200    ///< Call update function with the current phi_prompt struct.

#define phi_prompt_dialog_NO 0				///< NO response returned by yn_dialog
#define phi_prompt_dialog_YES 1				///< YES response returned by yn_dialog
#define phi_prompt_dialog_escape -1			///< Escape response returned by yn_dialog

// Internal function key codes
#define total_function_keys 6       ///< Total number of function keys. At the moment, only 6 functions exist: up/down/left/right/enter/escape
#define function_key_code_base 1    ///< All function key codes start from this number and up continuously. The key codes are generated in wait_on_escape when a function key is pressed.
#define phi_prompt_up 1             ///< Function key code for up
#define phi_prompt_down 2           ///< Function key code for down
#define phi_prompt_left 3           ///< Function key code for left
#define phi_prompt_right 4          ///< Function key code for right
#define phi_prompt_enter 5          ///< Function key code for enter
#define phi_prompt_escape 6         ///< Function key code for escape

#define phi_prompt_content_needs_update 127         ///< The sense_select_list() reports this return value if the select list needs to be updated, say the user scrolled up/down.

#define simple_select_list_max_items 32             ///< The maximal items allowed in the simple select list

#define HD44780_lcd 0               ///< Type of display is HD44780
#define KS0108_lcd 1                ///< Type of display is KS0108 GLCD
#define serial_lcd 2                ///< Type of display is serial lcd

#ifndef phi_prompt_max_item_display_length      ///< Only define this if it has not been defined. If you need to define phi_prompt_max_item_display_length to different value for 40x2 display, define this as 40 in arduino sketch before any include commands.
#define phi_prompt_max_item_display_length 20   ///< The render_list uses a buffer to render list item. 20 works for up to 20x4 displays to save SRAM. Each item occupies no more than this is the length on the display. Longer items are truncated or scrolled depending on option.

#define phi_prompt_select_list_update_ms 50
#define phi_prompt_text_area_update_ms 50

#endif

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>
#include <phi_interfaces.h>

union buffer_pointer    /// This defines a union to store various pointer types.
{
  int *i_buffer;
  float * f_buffer;
  char ** list;
  char* msg;
  PGM_P msg_P;
};

union four_bytes        /// This defines a union to store various data.
{
int i;
long l;
float f;
byte b;
char c;
char c_arr[4];
};

struct phi_prompt_struct    /// This struct is used in exchange information between caller and function.
{
  buffer_pointer ptr;
  four_bytes low;   // Lower limit for text panel (.c), integers (.i) and floats (.f) and first item displayed + highlighted item for list (.i), say 102 means first item displayed on screen is item 1 and highlighted item is 2, both starting from 0.
  four_bytes high;  // Upper limit for text panel (.c), integers (.i) and floats (.f) and last item for list (.i)
  four_bytes step;  // Step size for integers (.i) and integer/decimal for floats (.i), such as 302 means 3 digits before decimal and 2 digits after.
  byte col;         // Which column to display input
  byte row;         // Which row to display input
  byte width;       // Maximal number of character on integers, floats, a list item, and total allowed input characters for text panel
  int option;       // What display options to choose
  void (*update_function)(phi_prompt_struct *); // This is not being used in this version but reserved for future releases.
}; //22 bytes

void init_phi_prompt(LCD *l, multiple_button_input *k[], char ** fk, int w, int h, char i); ///< This is the library initialization routine.
void set_indicator(char i);                         ///< This sets the indicator used in lists/menus. The highlighted item is indicated by this character. Use '~' for a right arrow.
void set_bullet(char i);                            ///< This sets the bullet used in lists/menus. The non-highlighted items are indicated by this character. Use '\xA5' for a center dot.
void set_repeat_time(int i);                        ///< This sets key repeat time, how often a key repeats when held. It uses multiple_button_input.set_repeat()
void enable_key_repeat(boolean i);                  ///< Sets whether to enable key repeat on inputs. This is not used in this version. A future version may make use of it.
void enable_multi_tap(boolean i);                   ///< Sets whether to enable multi-tap on inputs. This is not used in this version. A future version may make use of it.
char inc(char ch, phi_prompt_struct *para);         ///< Increment character ch according to options set in para.
char dec(char ch, phi_prompt_struct *para);         ///< Decrement character ch according to options set in para.
char phi_prompt_translate(char key);                ///< This translates keypad input in case the key press is a function key. If the key press is not a function key, it returns without translation.
void scroll_text(char * src, char * dst, char dst_len, short pos);  ///< This scrolls a string into and out of a narrow window, for displaying long message on a narrow line.
void scroll_text_P(PGM_P src, char * dst, char dst_len, short pos); ///< This scrolls a string stored in PROGMEM into and out of a window, for displaying long message on a narrow line.
void msg_lcd(char* msg_lined);                      ///< This is a quick and easy way to display a string in the PROGMEM to the LCD.
void prev_line(phi_prompt_struct* para);            ///< Seeks previous line in a long message stored in SRAM.
void next_line(phi_prompt_struct* para);            ///< Seeks next line in a long message stored in SRAM.
void prev_line_P(phi_prompt_struct* para);          ///< Seeks previous line in a long message stored in PROGMEM.
void next_line_P(phi_prompt_struct* para);          ///< Seeks previous line in a long message stored in PROGMEM.
void center_text(char * src);                       ///< This function displays a short message in SRAM centered with the display size.
void center_text_P(PGM_P src);                      ///< This function displays a short message in PROGMEM centered with the display size.
void scroll_bar_v(byte p, byte c, byte r, byte h);  ///< Displays a scroll bar at column/row with height and percentage.
void long_msg_lcd(phi_prompt_struct *para);         ///< Displays a static long message stored in SRAM that could span multiple lines.
void long_msg_lcd_P(phi_prompt_struct *para);       ///< Displays a static long message stored in PROGMEM that could span multiple lines.
byte render_list(phi_prompt_struct *para);          ///< 

int wait_on_escape(int ref_time);                   ///< Returns key pressed or NO_KEY if time expires before any key was pressed. This does the key sensing and translation.

int ok_dialog(char msg[]);                          ///< Displays an ok dialog
int yn_dialog(char msg[]);                          ///< Displays a short message with yes/no options.
int input_integer(phi_prompt_struct *para);         ///< Input integer on keypad with fixed step, upper and lower limits.
int input_float(phi_prompt_struct *para);           ///< This function is kept in this release but will be removed in the future. Please don't use it.
int sense_select_list(phi_prompt_struct * para);    ///< Senses the user input for phi_prompt_select_list_update_ms, modifies the shared struct accordingly and returns what the user did in the phi_prompt_select_list_update_ms.  
int select_list(phi_prompt_struct *para);           ///< Displays a list/menu for the user to select. Display options for list: Option 0, display classic list, option 1, display 2X2 list, option 2, display list with index, option 3, display list with index2.
int input_panel(phi_prompt_struct *para);           ///< Input character options for input panel: Option 0, default, option 1 include 0-9 as valid inputs.
int input_number(phi_prompt_struct *para);          ///< Input number on keypad with decimal point and negative.
int sense_text_area(phi_prompt_struct * para);      ///< Senses the user input for phi_prompt_text_area_ms, modifies the shared struct accordingly and returns what the user did in the phi_prompt_text_area_update_ms.  
int text_area(phi_prompt_struct *para);             ///< Displays a text area using message stored in the SRAM.
int text_area_P(phi_prompt_struct *para);           ///< Displays a text area using message stored in PROGMEM.

#ifndef disable_simple_fns                          ///< Define disable_simple_fns in your sketch BEFORE #include <phi_prompt.h> to disable simple functions to save SRAM
// Simple phi_prompt functions
void simple_setup_6_buttons(LCD * lcd, byte columns, byte rows, byte btn_u, byte btn_d, byte btn_l, byte btn_r, byte btn_b, byte btn_a); ///< Sets up the library with lcd object and 6 buttons
LCD * simple_setup_phi_2(byte columns=20,byte rows=4);    ///< Set up the library for phi-2 shield, default 20X4
LCD * simple_setup_phi_3(byte columns=20,byte rows=4);    ///< Set up the library for phi-3 shield, default 20X4
LCD * get_lcd_ptr();                      ///< return pointer to the lcd
multiple_button_input ** get_mbi_ptr();             ///< return pointer to the multiple_button_input

void prepare_simple_select_list(char msg[], byte highlighted=0, long selections=-1);    ///< Prepares a menu/list from a formatted string including replacing \n with \0 etc.
void simple_select_list_center_choice(byte b);                      ///< Enable or disable center_select list choice. Use before simple_select_list.
void simple_select_list_scroll_bar(byte b);                         ///< Enable or disable scroll bar. Use before simple_select_list.
void simple_select_list_auto_scroll(byte b);                        ///< Enable or disable auto scroll long item. Use before simple_select_list.
void restore_string_for_simple_select_list();                       ///< This undoes the modification done on the string by prepare.
byte render_simple_select_list();                                   ///< Renders the simple select list using the prepared shared_struct by prepare_simple_select_list. It simply calls render_list
byte sense_simple_select_list();                                    ///< Senses the user input for phi_prompt_select_list_update_ms, modifies the shared struct accordingly and returns what the user did in the phi_prompt_select_list_update_ms. 
byte get_simple_select_list_item();                                 ///< Returns the current highlighted item from shared_struct.low.i
byte set_simple_select_list_item(byte b);                           ///< Sets the current highlighted item for shared_struct.low.i
byte simple_select_list(char msg[],byte b=0,long l=-1);             ///< Displays a menu/list from a formatted string. With no b option the list item 0 is highlighted. Returns the item selected.
byte get_simple_list_title(char msg[], char buf[]);                 ///< Extracts the title from a simple list msg. The list has to observe the same format as the list for simple_select_list: "Menu_title\nItem1\nItem2\nLast_item\n".
byte get_simple_list_item(char msg[], char buf[], byte item_num);   ///< Extracts the "item_num" item from a simple list msg. The list has to observe the same format as the list for simple_select_list: "Menu_title\nItem1\nItem2\nLast_item\n".

byte simple_input_panel(char msg[],byte lo, byte h, byte opt);      ///< Collect user input. Returns the user input in a string.

void prepare_simple_text_area(char msg[], byte highlighted=0);      ///< Prepares a text area from a formatted string.
void render_simple_text_area();                                     ///< Renders the simple text area using the prepared shared_struct by prepare_simple_text_area. It simply calls long_msg_lcd.
byte sense_simple_text_area();                                      ///< Senses the user input for phi_prompt_text_area_ms, modifies the shared struct accordingly and returns what the user did in the phi_prompt_text_area_update_ms.  
byte get_simple_text_area_location();                               ///< Returns the current text area location from shared_struct.low.i
byte set_simple_text_area_location(byte b);                         ///< Sets the current text area location  for shared_struct.low.i
byte simple_text_area(char msg[],byte b=0);                                  ///< Displays a long message with scrolling. It returns buttons pressed.
byte simple_formatted_msg(char msg[]);                              ///< Displays a long message to fill the size of the screen. It returns nothing.
#endif

#endif