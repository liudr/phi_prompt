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
 * 03/24/2015: Added option 2 (space padding to the left) coding to input_integer(). The option featured in documentation but was not coded till now. \n
 * 02/12/2015: Removed PROGMEM prog_char and replaced with const char PROGMEM. \n
 * 12/22/2012: Moved simple setup functions for 6 buttons, phi-2 and phi-3 shields into simple function block so they can be disabled by disable_simplefns. \n
 * 11/25/2012: Replaced max_items for simple selec list with a def simple_select_list_max_items and set it to 32. Also modified simple_input_panel to display on row=lcd_h/2 so row 1 for 2-row display or row 2 for 4-row display. \n
 * 11/24/2012: Added simple setup functions for 6 buttons, phi-2 and phi-3 shields. Corrected the sense_select_list left and right button didn't scroll column problem since a while back. \n
 * 11/19/2012: Added if-else for i/n index printing so it will stay as %2d/%2d if there are 10 or more items so if one goes from 15/15 back to 1/15, no junk will be left, like 1/155. Also updated sample code to position index according to total number of items.  \n
 * 08/18/2012: Added static variable simple_option to store required features for simple list. \n
 * Also modified the simple_select_list_[auto_scroll, scroll_bar, center_choice] functions to store required option in simple_option instead of directly altering sharedstruct.option, which gets overwritten everytime a new list is rendered. \n
 * Corrected wrong logic in simple_select_list_scroll_bar. \n
 * 03/31/2012: Added function to prepare_simple_select_list to render multi-select list. \n
 * 03/29/2012: Added several functions and restructured how select_list and text_area works. Extracted prepare and sense out of select_list and text_area and use them in both simple and non-simple version. \n
 * 03/19/2012: Changed several byte variables to int (i in simple_select_list adn get_simple_list_item, inc in long_msg_lcd and long_msg_lcd_P) to accommodate larger simple lists and longer messages. \n
 * 03/06/2012: Corrected the yn_dialog return value wrong comment. Added scroll bar and center item only for one column list to simple_select_list. \n
 * 02/18/2012: Corrected simple_select_list(). The restoring new line function was fixed. \n
 * 02/12/2012: Added simple functions simple_select_list(), simple_long_msg(), and simple_input_panel(). \n
 * 12/18/2011: Modified with #if and (uint8_t) to run on both arduino IDE 1.0 and pre-1.0 versions. \n
 * 05/23/2011: First official release of the library compatible with Arduino IDE 0022. Needs phi_buttons library as the physical layer. \n
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
#include <phi_prompt.h>

const char phi_prompt_lcd_ch0[] PROGMEM ={ 4,14,31,64,31,31,31,31,0}; ///< Custom LCD character: Up triangle with block
const char phi_prompt_lcd_ch1[] PROGMEM ={ 4,14,31,64,64,64,64,64,0}; ///< Custom LCD character: Up triangle 
const char phi_prompt_lcd_ch2[] PROGMEM ={31,31,31,31,64,64,64,64,0}; ///< Custom LCD character: Top block
const char phi_prompt_lcd_ch3[] PROGMEM ={64,64,64,64,31,31,31,31,0}; ///< Custom LCD character: Bottom block
const char phi_prompt_lcd_ch4[] PROGMEM ={64,64,64,64,64,31,14, 4,0}; ///< Custom LCD character: Down triangle
const char phi_prompt_lcd_ch5[] PROGMEM ={31,31,31,31,64,31,14, 4,0}; ///< Custom LCD character: Down triangle with block
const char phi_prompt_lcd_ch6[] PROGMEM ={B01000, B01100, B01010, B01001, B01001, B11010, B11000, B00000,0}; ///< Custom LCD character: Music
const char phi_prompt_lcd_ch7[] PROGMEM ={B00111, B00100, B00000, B11100, B10000, B11101, B00010, B00101,0}; ///< Custom LCD character: Lux
const char * const phi_prompt_lcd_ch_item[] PROGMEM = {phi_prompt_lcd_ch0, phi_prompt_lcd_ch1, phi_prompt_lcd_ch2, phi_prompt_lcd_ch3, phi_prompt_lcd_ch4, phi_prompt_lcd_ch5, phi_prompt_lcd_ch6, phi_prompt_lcd_ch7}; ///< Custom LCD character char array addresses. 

const char PROGMEM yn_00[] PROGMEM =" YES >NO<";          ///< This list item is used to render Y/N dialog
const char PROGMEM yn_01[] PROGMEM =">YES< NO ";          ///< This list item is used to render Y/N dialog
const char * const yn_items[] PROGMEM = {yn_00,yn_01};  ///< This list  is used to render Y/N dialog

static LCD * lcd;                 ///< This pointer stores the LiquidCrystal object for display purpose.
static int lcd_w;                           ///< This is the width of the LCD in number of characters.
static int lcd_h;                           ///< This is the height of the LCD in number of characters.
static char indicator;                      ///< This is the character used as indicator in lists/menus. The highlighted item is indicated by this character. Use '~' for a right arrow.
static char bullet;                         ///< This is the bullet used in lists/menus. The non-highlighted items are indicated by this character. Use '\xA5' for a center dot.
static char ** function_keys;               ///< This points to an array of pointers that each is a zero-terminated string representing function keys.
static multiple_button_input ** mbi_ptr;    ///< This points to an array of pointers that each points to a multiple_button_input object.
static byte lcd_type;                       ///< This indicates the type of lcd, such as HD44780 or GLCD.
static boolean key_repeat_enable=1;         ///< This is not used in this version. A future version may make use of it.
static boolean multi_tap_enable=0;          ///< This is not used in this version. A future version may make use of it.
static int simple_option=0;                 ///< This stores the required menu option for simple list.
#ifndef disable_simple_fns
static phi_prompt_struct shared_struct;     ///< This struct is shared among simple function calls.
#endif
//Utilities
/**
 * \details This initializes the phi_prompt library. It needs to be called before any phi_prompt functions are called.
 * \param l This is the address of your LCD object, which you already used begin() on, such as &LCD.
 * \param k This is the name of the pointer array that stores the address of (pointer to) all your input keypads. The last element of the array needs to be 0 to terminate the array.
 * \param fk This is the name of the array that stores the names of all char arrays with function keys. Make sure you use strings such as "U" for each array to indicate function keys instead of char like 'U'.
 * \param w This is the width of the LCD in number of characters.
 * \param h This is the height of the LCD in number of characters.
 * \param i This is the character used as indicator in lists/menus. The highlighted item is indicated by this character. Use '~' for a right arrow.
 */
void init_phi_prompt(LCD *l, multiple_button_input *k[], char ** fk, int w, int h, char i)
{
  lcd=l;
  mbi_ptr=k;
  function_keys=fk;
  lcd_w=w;
  lcd_h=h;
  indicator=i;
  bullet='\xA5';
  byte ch_buffer[10]; // This buffer is required for custom characters on the LCD.
  if (lcd!=0)
  {
    for (int i=0;i<8;i++)
    {
      strcpy_P((char*)ch_buffer,(char*)pgm_read_word(&(phi_prompt_lcd_ch_item[i])));
      lcd->createChar(i, ch_buffer);
    }
  }
  lcd_type=HD44780_lcd;
}


void set_indicator(char i)
{
  indicator=i;
}

void set_bullet(char i)
{
  bullet=i;
}

void set_repeat_time(int i)
{
  mbi_ptr[0]->set_repeat(i);
}

void enable_key_repeat(boolean i)
{
  key_repeat_enable=i;
}

void enable_multi_tap(boolean i)
{
//if (!multi_tap_enable) multi_tap(init);
  multi_tap_enable=i;
}

/**
 * \details Increment character ch according to options set in para. This function is used in input panel with up/down key to increment the current character to the next.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * Option 0: default, option 1: include 0-9 as valid inputs, option 2: only 0-9 are valid, option 3: only 0-9 are valid and the first digit can be '-', option 4: only 0-9 are valid and number increments, option 5: only 0-9 are valid and the first digit can be '-' and number increments.
 * To do:
 * Option 2-4, automatically increment or decrement a whole series of numbers when they are connected, such as File0009.txt when increase 9 (if only 0-9 are allowed) will give File0010.txt.
 * If you are not interested in the inner working of this library, don't call it.
 * \return It returns the character after the increment.
 */
char inc(char ch, phi_prompt_struct *para)
{
  if ((ch<para->high.c)&&(ch>=para->low.c)) return (++ch);
  else
  {
    switch (para->option) /// The following is the switch-case for character increment processing.
    {
      case 0: /// No options. The high and low determine range of characters you can enter.
      if (ch==para->high.c) ch=para->low.c;
      break;
      
      case 1: /// Include 0-9
      if (ch=='9') ch=para->low.c;
      else if ((ch>='0')&&(ch<'9')) ch++;
      else if (ch==para->high.c) ch='0';
      break;

      case 2: /// only 0-9
      case 4: /// only 0-9 and automatically decrements (done at higher level)
      if (ch=='9') ch='0';
      else ch++;
      break;

      case 3: /// only 0-9 and '-'
      case 5: /// only 0-9 and '-' and automatically increments (done at higher level)
      if (ch=='9') ch='-';
      else if (ch=='-') ch='0';
      else ch++;
      break;
    }
  }
  return ch;
}

/**
 * \details Decrement character ch according to options set in para. This function is used in input panel with up/down key to decrement the current character to the previous.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * Option 0: default, option 1: include 0-9 as valid inputs, option 2: only 0-9 are valid, option 3: only 0-9 are valid and the first digit can be '-', option 4: only 0-9 are valid and number increments, option 5: only 0-9 are valid and the first digit can be '-' and number increments.
 * To do:
 * Option 2-4, automatically increment or decrement a whole series of numbers when they are connected, such as File0009.txt when increase 9 (if only 0-9 are allowed) will give File0010.txt.
 * If you are not interested in the inner working of this library, don't call it.
 * \return It returns the character after the decrement.
 */
char dec(char ch, phi_prompt_struct *para)// Decrease character. Used in input panels
{
  if ((ch<=para->high.c)&&(ch>para->low.c)) return (--ch);
  else
  {
    switch (para->option) /// The following is the switch-case for character decrement processing.
    {
      case 0: /// No options. The high and low determine range of characters you can enter.
      if (ch==para->low.c) ch=para->high.c;
      break;
      
      case 1: /// Include 0-9
      if (ch=='0') ch=para->high.c;
      else if ((ch>'0')&&(ch<='9')) ch--;
      else if (ch==para->low.c) ch='9';
      break;

      case 2: /// only 0-9
      case 4: /// only 0-9 and automatically decrements (done at higher level)
      if (ch=='0') ch='9';
      else ch--;
      break;

      case 3: /// only 0-9 and '-'
      case 5: /// only 0-9 and '-' and automatically decrements (done at higher level)
      if (ch=='0') ch='-';
      else if (ch=='-') ch='9';
      else ch--;
      break;
    }
  }
  return ch;
}

/**
 * \details This function translates key press returned from all keypads. This function is only called by wait_on_escape.
 * \return If a key is defined as a function key, it returns the code of the function key defined in the function_key_base define section.
 * If the key is not a function key, it returns the key unaltered.
 * If you are not interested in the inner working of this library, don't call it.
 */
char phi_prompt_translate(char key)
{
  for (byte i=0;i<total_function_keys;i++)
  {
    byte j=0;
    while(function_keys[i][j])
    {
      if (key==function_keys[i][j]) return (function_key_code_base+i);
      j++;
    }
  }
  return key;
}

//Text renderers

/**
 * \details This function displays a short message in SRAM centered with the display size.
 * This outputs a text center-aligned on the display depending on the display size.
 * Eg. center_text("Introduction") outputs ">>>>Introduction<<<<" on 20 column display.
 * \param src This is the string to be displayed.
 */
void center_text(char * src)
{
  byte j=0;
  for (byte i=0;i<lcd_w;i++)
  {
    if (i<lcd_w/2-(strlen(src)-strlen(src)/2)) lcd->write('>');
    else if (i>=lcd_w/2+strlen(src)/2) lcd->write('<');
    else 
    {
      lcd->write(src[j]);
      j++;
    }
  }
}

/**
 * \details This function displays a short message in PROGMEM centered with the display size.
 * This outputs a text center-aligned on the display depending on the display size.
 * Eg. center_text("Introduction") outputs ">>>>Introduction<<<<" on 20 column display.
 * \param src This is the string to be displayed.
 */
void center_text_P(PGM_P src)
{
  byte j=0;
  for (byte i=0;i<lcd_w;i++)
  {
    if (i<lcd_w/2-(strlen_P(src)-strlen_P(src)/2)) lcd->write('>');
    else if (i>=lcd_w/2+strlen_P(src)/2) lcd->write('<');
    else 
    {
      lcd->write(pgm_read_byte_near(src+j));
      j++;
    }
  }
}
/**
 * \details This copies the right amount of text into a narrow space so it can be displayed and scrolled to show the entire message.
 * If you are not very interested in the inner working of this library, this is not for you. It is used to auto scroll long list items.
 * \param src This points to the long string that needs to be scrolled inside a narrow line.
 * \param dst This points to the buffer that has a length of the narrow line width + 1 (for 0 termination). The part of the text that will be displayed is copied to this buffer after the function call.
 * \param dst_len This is the width of the narrow line. Only this many characters from the source will be copied to the dst buffer.
 * \param pos This is the position of the long text. When pos=0, the text's first portion displays left justified in the dst buffer. When pos<0, spaces are padded before the text, useful for scrolling from right to left. When pos>dst_len, space is padded to the end of the text to make it scroll all the way to the left and eventually disappear. 
 * \return It returns the text to be displayed in the buffer and no value is directly returned.
 */
void scroll_text(char * src, char * dst, char dst_len, short pos)
{
  for (byte j=0;j<dst_len;j++)
  {
    if ((pos<0)||(pos>strlen(src)-1))
    {
      dst[j]=' ';
    }
    else dst[j]=src[pos];
    pos++;
  }
  dst[dst_len]=0;
}

/**
 * \details This copies the right amount of text (stored in PROGMEM) into a narrow space so it can be displayed and scrolled to show the entire message.
 * If you are not very interested in the inner working of this library, this is not for you. It is used to auto scroll long list items.
 * \param src This points to the long string that needs to be scrolled inside a narrow line.
 * \param dst This points to the buffer that has a length of the narrow line width + 1 (for 0 termination). The part of the text that will be displayed is copied to this buffer after the function call.
 * \param dst_len This is the width of the narrow line. Only this many characters from the source will be copied to the dst buffer.
 * \param pos This is the position of the long text. When pos=0, the text's first portion displays left justified in the dst buffer. When pos<0, spaces are padded before the text, useful for scrolling from right to left. When pos>dst_len, space is padded to the end of the text to make it scroll all the way to the left and eventually disappear. 
 * \return It returns the text to be displayed in the buffer and no value is directly returned.
 */
void scroll_text_P(PGM_P src, char * dst, char dst_len, short pos)
{
  for (byte j=0;j<dst_len;j++)
  {
    if ((pos<0)||(pos>strlen_P(src)-1))
    {
      dst[j]=' ';
    }
    else dst[j]=pgm_read_byte_near(src+pos);
    pos++;
  }
  dst[dst_len]=0;
}

/**
 * \details This is a quick and easy way to display a string in the PROGMEM to the LCD. Note the string should have a maximum of 20 characters.
 * \param msg_line This is the name of the char string stored in PROGMEM.
 */
void msg_lcd(char* msg_line)
{
  byte i=0,ch;
  while (ch=pgm_read_byte_near(msg_line+i))
  {
    i++;
    lcd->write(ch);
  }
//  lcd->print(phi_prompt_max_item_display_length);
}

/**
 * \details Seeks previous line in a long message stored in SRAM. This seems easy until you start adding \n and \t etc. into the picture.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * This function assumes the long message position has been always saught properly. Any improper movement of the long message pointer could produce unpredictable results.
 * If you are not interested in the inner working of this library, use text_area instead.
 * Return values are updated throught the struct.
 */
void prev_line(phi_prompt_struct* para)
{
  byte columns=para->step.c_arr[1];
  if (para->low.i<=0)
  {
    para->low.i=0;
    return;
  }
  if (para->ptr.msg[para->low.i-1]=='\n')
  { //Seek beginning of a paragraph.
    int dec=para->low.i-2;
    while(para->ptr.msg[dec]!='\n')
    {
      dec--;
      if (dec==0)
      {
        para->low.i=0;
        return;
      }
    }
    para->low.i-=((para->low.i-1-dec-1)%columns+1);
  }

  else para->low.i-=columns;
  return;
}

/**
 * \details Seeks next line in a long message stored in SRAM. This seems easy until you start adding \n and \t etc. into the picture.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * This function assumes the long message position has been always saught properly. Any improper movement of the long message pointer could produce unpredictable results.
 * If you are not interested in the inner working of this library, use text_area instead.
 * Return values are updated throught the struct.
 */
void next_line(phi_prompt_struct* para)
{
  byte columns=para->step.c_arr[1];
  for (int i=para->low.i;i<para->low.i+columns;i++)
  {
    if (para->ptr.msg[i]=='\n')
    {
      para->low.i=i+1;
      return;
    }

    if (i==strlen(para->ptr.msg))
    {
      return;
    }
  }
  para->low.i+=columns;
}

/**
 * \details Seeks previous line in a long message stored in PROGMEM. This seems easy until you start adding \n and \t etc. into the picture.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * This function assumes the long message position has been always saught properly. Any improper movement of the long message pointer could produce unpredictable results.
 * If you are not interested in the inner working of this library, use text_area instead.
 * Return values are updated throught the struct.
 */
void prev_line_P(phi_prompt_struct* para)
{
  byte columns=para->step.c_arr[1];
  if (para->low.i<=0)
  {
    para->low.i=0;
    return;
  }
  if (pgm_read_byte_near(para->ptr.msg_P+para->low.i-1)=='\n')
  { //Seek beginning of a paragraph.
    int dec=para->low.i-2;
    while(pgm_read_byte_near(para->ptr.msg_P+dec)!='\n')
    {
      dec--;
      if (dec==0)
      {
        para->low.i=0;
        return;
      }
    }
    para->low.i-=((para->low.i-1-dec-1)%columns+1);
  }

  else para->low.i-=columns;
  return;
}

/**
 * \details Seeks next line in a long message stored in PROGMEM. This seems easy until you start adding \n and \t etc. into the picture.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * This function assumes the long message position has been always saught properly. Any improper movement of the long message pointer could produce unpredictable results.
 * If you are not interested in the inner working of this library, use text_area instead.
 * Return values are updated throught the struct.
 */
void next_line_P(phi_prompt_struct* para)
{
  byte columns=para->step.c_arr[1];
  for (int i=para->low.i;i<para->low.i+columns;i++)
  {
    if (pgm_read_byte_near(para->ptr.msg_P+i)=='\n')
    {
      para->low.i=i+1;
      return;
    }

    if (i==strlen_P(para->ptr.msg_P))
    {
      return;
    }
  }
  para->low.i+=columns;
}

/**
 * \details Displays a static long message stored in SRAM that could span multiple lines.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * Option 0: just display message. Option 1: display message with a scrollbar to the right.
 * If you are not interested in the inner working of this library, use text_area instead.
 * Return values are updated throught the struct.
 */
void long_msg_lcd(phi_prompt_struct* para)
{
  byte columns=para->step.c_arr[1], rows=para->step.c_arr[0], ch;
  int inc=0;
//  lcd->noBlink();
  for (byte i=0;i<rows;i++)
  {
    if ((para->low.i+inc>=strlen(para->ptr.msg))||(para->ptr.msg[para->low.i+inc]=='\n'))
    {
      ch=0;
      inc++;
    }
    else ch=para->ptr.msg[para->low.i+inc];
    lcd->setCursor(para->col,para->row+i);
    for (byte j=0;j<columns;j++)
    {
      if (ch==0) lcd->write(' ');
      else
      {
        lcd->write(ch);
        ch=para->ptr.msg[para->low.i+(++inc)];
        if ((ch=='\n')&&(j<columns-1))
        {
          ch=0;
          inc++;
        }
      }
    }
  }
  if (para->option==1)
  {
    scroll_bar_v(((long)para->low.i)*100/strlen(para->ptr.msg),para->col+columns,para->row,rows);
  }
}

/**
 * \details Displays a static long message stored in PROGMEM that could span multiple lines.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * Option 0: just display message. Option 1: display message with a scrollbar to the right.
 * If you are not interested in the inner working of this library, use text_area instead.
 * Return values are updated throught the struct.
 */
void long_msg_lcd_P(phi_prompt_struct* para) // Displays a long message stored in PROGMEM that could span multiple lines.
{
  byte columns=para->step.c_arr[1], rows=para->step.c_arr[0], ch;
  int inc=0;
//  lcd->noBlink();
  for (byte i=0;i<rows;i++)
  {
    if ((para->low.i+inc>=strlen_P(para->ptr.msg_P))||(pgm_read_byte_near(para->ptr.msg_P+para->low.i+inc)=='\n'))
    {
      ch=0;
      inc++;
    }
    else ch=pgm_read_byte_near(para->ptr.msg_P+para->low.i+inc);
    lcd->setCursor(para->col,para->row+i);
    for (byte j=0;j<columns;j++)
    {
      if (ch==0) lcd->write(' ');
      else
      {
        lcd->write(ch);
        ch=pgm_read_byte_near(para->ptr.msg_P+para->low.i+(++inc));
        if ((ch=='\n')&&(j<columns-1))
        {
          ch=0;
          inc++;
        }
      }
    }
  }

  if (para->option==1)
  {
    scroll_bar_v(((long)para->low.i)*100/strlen_P(para->ptr.msg_P),para->col+columns,para->row,rows);
  }
}

/**
 * \details Displays a scroll bar at column/row with height and percentage.
 * If you are not very interested in the inner working of this library, this is not for you.
 * This is the only function in phi_prompt library to use custom characters.
  * If you used custom characters and want to use this function or a long message with scroll bar, run the init again to reinitialize custom characters.
 * \param percentage This goes between 0 and 99, representing the location indicator on the bar.
 * \param column This is the column location of the scroll bar's top.
 * \param row This is the row location of the scroll bar's top.
 * \param v_height This is the height of the bar in number of rows.
 */
void scroll_bar_v(byte percent, byte column, byte row, byte v_height)
{
  int mapped;
  if (percent>99) percent=99;
  mapped=(int)(v_height*2-2)*percent/100; // This is mapped position, 2 per row of bar.
  for (byte i=0;i<v_height;i++)
  {
    lcd->setCursor(column,row+i);
    if (i==(mapped+1)/2)
    {
      if (i==0)
      {
        lcd->write((uint8_t)0);
      }
      else if (i==v_height-1)
      {
        lcd->write(5);
      }
      else
      {
        if (mapped+1==(mapped+1)/2*2) lcd->write(2);
        else lcd->write(3);
      }
    }
    else
    {
      if (i==0)
      {
        lcd->write(1);
      }
      else if (i==v_height-1)
      {
        lcd->write(4);
      }
      else
      {
        lcd->write(' ');
      }
    }
  }
}

/**
 * \details Displays a static list or menu stored in SRAM or PROGMEM that could span multiple lines.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * Option is very extensive and please refer to the documentation and option table.
 * The options are combined with OR operation with "Render list option bits" you can find in the library include file.
 * To find out what each option does exactly, run phi_prompt_big_show demo code and try the options out and write the number down.
 * If you are not interested in the inner working of this library, use text_area instead.
 * \return If further update is needed, it returns 1. The caller needs to call it again to update display, such as scrolling item.
 * If it returns 0 then no further display update is needed and the caller can stop calling it.
 */
byte render_list(phi_prompt_struct* para)
{
  byte ret=0, _first_item, _last_item, columns=para->step.c_arr[1], rows=para->step.c_arr[0], item_per_screen=columns*rows, x1=para->col, y1=para->row, x2=para->step.c_arr[3], y2=para->step.c_arr[2]; // Which items to display.
  char list_buffer[phi_prompt_max_item_display_length+2];   ///< Each list item can have an indicator such as dot or arrow and the item needs to be zero-terminated so 2 extra characters.
  long pos=millis()/500;
  if (para->width>phi_prompt_max_item_display_length) para->width=phi_prompt_max_item_display_length;   ///< Make sure the caller doesn't demand a list item that is longer than the list_buffer can hold.

  if (para->option&phi_prompt_center_choice) // Determine first item on whether choice is displayed centered.
  {
    _first_item=para->low.i-item_per_screen/2;
    if (_first_item>127) _first_item=0;
    else if (para->low.i-item_per_screen/2+item_per_screen>para->high.i) _first_item=para->high.i+1-item_per_screen;
  }
  else
  {
    _first_item=(para->low.i/item_per_screen)*item_per_screen;
  }
    
  _last_item=_first_item+item_per_screen-1; // Determine last item based on first item, total item per screen, and total item.
  if (_last_item>para->high.i) _last_item=para->high.i;
  
  for (byte i=_first_item;i<_first_item+item_per_screen;i++)
  {
    if (i<=_last_item) // Copy item
    {
      if ((para->option&phi_prompt_list_in_SRAM))
      {
        if ((para->option&phi_prompt_auto_scroll)&&(i==para->low.i)&&strlen(*(para->ptr.list+i))>para->width) // Determine what portion of the item to be copied. In case of no auto scrolling, only first few characters are copied till the display buffer fills. In case of auto scrolling, a certain portion of the item is copied.
        {
          pos=pos%(strlen(*(para->ptr.list+i))+para->width)-para->width;
          scroll_text(*(para->ptr.list+i), list_buffer, para->width, pos);//Does the actual copy
          ret=1; // More update is needed to scroll text.
        }
        else //Does the actual truncation
        {
          byte len=strlcpy(list_buffer,*(para->ptr.list+i), para->width+1);
          if (len<para->width)
          {
            for (byte k=len;k<para->width;k++)
            {
              list_buffer[k]=' ';
            }
            list_buffer[para->width]=0;
          }
        }
      }
      else // The list is in PROGMEM
      {
        if ((para->option&phi_prompt_auto_scroll)&&(i==para->low.i)&&strlen_P((char*)pgm_read_word(para->ptr.list+i))>para->width) // Determine what portion of the item to be copied. In case of no auto scrolling, only first few characters are copied till the display buffer fills. In case of auto scrolling, a certain portion of the item is copied.
        {
          pos=pos%(strlen_P((char*)pgm_read_word(para->ptr.list+i))+para->width)-para->width;
          scroll_text_P((char*)pgm_read_word(para->ptr.list+i), list_buffer, para->width, pos);//Does the actual copy with scroll_text_P
          ret=1; // More update is needed to scroll text.
        }
        else //Does the actual truncation
        {
          byte len=strlcpy_P(list_buffer,(char*)pgm_read_word(para->ptr.list+i), para->width+1);
          if (len<para->width)
          {
            for (byte k=len;k<para->width;k++)
            {
              list_buffer[k]=' ';
            }
            list_buffer[para->width]=0;
          }
        }
      }
    }
    else // Fill blank
    {
      byte j;
      for (j=0;j<para->width;j++)
      {
        list_buffer[j]=' ';
      }
      list_buffer[j]=0;
    }
//Display item on LCD
    lcd->setCursor(para->col+((i-_first_item)/rows)*(para->width+1), para->row+(i-_first_item)%rows);

    if (para->option&phi_prompt_arrow_dot) // Determine whether to render arrow and dot. In case of yes, the buffer is shifted to the right one character.       
    {
      if (i<=_last_item)
      {
      lcd->write((i==para->low.i)?indicator:bullet);// Show ">" or a dot
      }
      else
      {
        lcd->write(' ');
      }
    }
    lcd->print(list_buffer);
  }

  if (para->option&phi_prompt_index_list) // Determine whether to display 1234567890 index
  {
    lcd->setCursor(x2,y2);
    for (byte i=0;i<=para->high.i;i++)
    {
      if (i==para->low.i) lcd->write(indicator); // Display indicator on index
      else lcd->write(i%10+'1');
    }
  }
  
  else if (para->option&phi_prompt_current_total) // Determine whether to display current/total index
  {
    if (para->high.i+1<10) sprintf(list_buffer,"%c%d/%d", indicator,para->low.i+1, para->high.i+1); // Make sure the index will occupy 1 digit+/+1 digit when there are less than 10 items
    else sprintf(list_buffer,"%c%2d/%2d", indicator,para->low.i+1, para->high.i+1); // Make sure the index will occupy 2 digit+/+2 digit when there are 10 or more items
    lcd->setCursor(x2,y2);
    lcd->print(list_buffer);// Prints index
  }
  
  if (para->option&phi_prompt_scroll_bar) // Determine whether to display scroll bar
  {
    scroll_bar_v(((int)para->low.i+1)*100/(para->high.i+1),para->col+columns*(para->width+1)-1*(!(para->option&phi_prompt_arrow_dot)),para->row,rows);
  }
  
  if (para->option&phi_prompt_flash_cursor) // Determine whether to display flashing cursor
  {
    lcd->setCursor(para->col+((para->low.i-_first_item)/rows)*(para->width+1), para->row+(para->low.i-_first_item)%rows);
    lcd->blink();
  }
  else lcd->noBlink();
  
  if (para->option&phi_prompt_invert_text) // Determine whether to invert highlighted item
  {
    
  }
  return ret;  
}

//Interactions

/**
 * \details This function is the center of phi_prompt key sensing. It polls all input keypads for inputs for the length of ref_time in ms
 * If a key press is sensed, it attempts to translate it into function keys or pass the result unaltered if it is not a function key.
 * It only detects one key presses so holding multiple keys will not produce what you want.
 * For function key codes, refer to the "Internal function key codes" section in the library header.
 * \param ref_time This is the time the function is allowed to wait, while polling on all input keypads.
 * \return Returns translated key or NO_KEY if no key is detected after the time expires.
 */
int wait_on_escape(int ref_time)
{
//Wait on button push.
  long temp0;
  byte temp1;
  temp0=millis();
  do
  {
    byte i=0;
    while(mbi_ptr[i])
    {
      temp1=mbi_ptr[i]->getKey();
      if (temp1!=NO_KEY) return (phi_prompt_translate(temp1));
      i++;
    }
  }   while ((millis()-temp0<ref_time));

  return (NO_KEY);
}

//Inputs
/**
 * \details Input an integer value with wrap-around capability. Integers are inputted with up and down function keys. The value has upper and lower limits and step.
 * Pressing number keys has no effect since the input needs to be restricted with limits and steps.
 * This function prints the initial value first so the caller doesn't need to.
 * Function traps until the update is finalized by the left, right, enter button or escape button.
 * Return values are updated throught the pointer.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * Display options for integers:
 * 0: space pad right, 1: zero pad left, 2: space pad left.
 * \return Returns function keys pushed so the caller can determine what to do:
 * Go back to the last slot with left (-3)
 * Go forward to the next slot with right (-4)
 * Enter(1)
 * Escape(-1).
 */
int input_integer(phi_prompt_struct *para)
{
  
  int number=*(para->ptr.i_buffer);
  byte temp1;
  byte space=para->width;
  char msg[space+1];
  char format[]="%00d";
  for (byte i=0;i<space;i++) msg[i]=' '; // Create mask the size of the output space.
  msg[space]=0;
  lcd->setCursor(para->col,para->row); // Write mask to erase previous content.
  lcd->print(msg);
  lcd->setCursor(para->col,para->row); // Write initial value.
  switch (para->option) // Prints out the content once before accepting user inputs.
  {
    case 0://padding with space to the right (padding done in erasing phase)
    sprintf(msg,"%d",number);
    break;

    case 1://padding with zero to the left
    format[2]='0'+space;
    sprintf(msg,format,number);
    break;

    case 2://padding with space to the left
    format[1]='0'+space;
    format[2]='d';
    format[3]='\0';
    sprintf(msg,format,number);
    break;
  }

  lcd->print(msg);
  lcd->setCursor(para->col,para->row); // Write initial value.
  lcd->cursor();

  while(true)
  {
    temp1=wait_on_escape(50);
    switch (temp1) /// The following is the switch-case for key press processing.
    {
      case phi_prompt_up: /// Up is pressed. The number increments with a step size of para->step.i. The number wraps around.
      if (number+(para->step.i)<=(para->high.i)) number+=(para->step.i);
      else number=para->low.i;
      lcd->setCursor(para->col,para->row);
      for (byte i=0;i<space;i++) msg[i]=' '; // Create mask the size of the output space.
      msg[space]=0;
      lcd->print(msg);
      lcd->setCursor(para->col,para->row);
      
      switch (para->option)
      {
        case 0://padding with space to the right (padding done in erasing phase)
        sprintf(msg,"%d",number);
        break;

        case 1://padding with zero to the left
        format[2]='0'+space;
        sprintf(msg,format,number);
        break;
		
		case 2://padding with space to the left
		format[1]='0'+space;
		format[2]='d';
		format[3]='\0';
		sprintf(msg,format,number);
        break;
      }

      lcd->print(msg);
      lcd->setCursor(para->col,para->row);
      break;
      
      case phi_prompt_down: /// Down is pressed. The number decrements with a step size of para->step.i. The number wraps around.
      if (number-para->step.i>=para->low.i) number-=para->step.i;
      else number=para->high.i;
      lcd->setCursor(para->col,para->row);
      for (byte i=0;i<space;i++) msg[i]=' '; // Create mask the size of the output space.
      msg[space]=0;
      lcd->print(msg);
      lcd->setCursor(para->col,para->row);

      switch (para->option)
      {
        case 0://padding with space to the right (padding done in erasing phase)
        sprintf(msg,"%d",number);
        break;

        case 1://padding with zero to the left
        format[2]='0'+space;
        sprintf(msg,format,number);
        break;
		
		case 2://padding with space to the left
		format[1]='0'+space;
		format[2]='d';
		format[3]='\0';
		sprintf(msg,format,number);
		break;
      }

      lcd->print(msg);
      lcd->setCursor(para->col,para->row);
      break;
      
      case phi_prompt_left: /// Left is pressed. The function returns -3. This can be used to switch to an entry to the left.
      *(para->ptr.i_buffer)=number;
      lcd->noCursor();
      return (-3);
      break;
      
      case phi_prompt_right: /// Right is pressed. The function returns -4. This can be used to switch to an entry to the right.
      *(para->ptr.i_buffer)=number;
      lcd->noCursor();
      return (-4);
      break;
      
      case phi_prompt_enter: /// Enter is pressed. The function returns 1.
      *(para->ptr.i_buffer)=number;
      lcd->noCursor();
      return(1);
      break;
      
      case phi_prompt_escape: /// Escape is pressed. The function returns -1.
      lcd->noCursor();
      return (-1);
      break;
      
      default:
      break;
    }
  }
}

/*
This is still supported but not number keypad support has not been expanded to it and it will disappear from the library in future releases.
Input a floating point value with fixed decimal point. Ironic but true.
This function prints the initial value first so the caller doesn't need to.
Function traps until the update is finalized by the enter button or escape button.
Return values are updated throught the pointer.
Returns buttons pushed so the caller can determine what to do:
Go back to the last slot with left (-3)
Go forward to the next slot with right (-4)
Enter(1)
Escape(-1).
Display options for floats:
0: only positive numbers allowed, 1: only negative numbers allowed, 2: both positive and negative numbers are allowed.
*/
int input_float(phi_prompt_struct *para)
{
  phi_prompt_struct myTextInput;
  char number[17]; // This is the buffer that will store the content of the text panel.
  char format[]="%04d.%02d";
  byte before=para->step.c_arr[1], after=para->step.c_arr[0]; // How many digits to display before and after decimal point.
  int ret;
  format[2]=before+'0';
  format[7]=after+'0';
  sprintf(number,format,int(*para->ptr.f_buffer),int((*para->ptr.f_buffer-int(*para->ptr.f_buffer))*pow(10,after)*(*para->ptr.f_buffer>0?1:-1)+0.5)); // Form the string to represent current value. This string will be the default value of the float input.

  myTextInput.ptr.msg=number; // Assign the text buffer address
  myTextInput.width=before+after+1; // Length of the input panel is 12 characters.
  myTextInput.col=para->col; // Display input panel at required column
  myTextInput.row=para->row; // Display input panel at required row
  switch (para->option)
  {
    case 0:
    myTextInput.low.c='0'; // Text panel valid input starts with character '-'.
    myTextInput.high.c='9'; // Text panel valid input ends with character '-'. Could end with '.' if real floating number is enabled.
    myTextInput.option=0; // Option 1 incluess 0-9 as valid characters. Option 0, default, option 1 include 0-9 as valid inputs.
    break;

    case 1:
    number[0]='-';
    myTextInput.low.c='0'; // Text panel valid input starts with character '-'.
    myTextInput.high.c='9'; // Text panel valid input ends with character '-'. Could end with '.' if real floating number is enabled.
    myTextInput.option=0; // Option 1 incluess 0-9 as valid characters. Option 0, default, option 1 include 0-9 as valid inputs.
    break;

    case 2:
    myTextInput.low.c='-'; // Text panel valid input starts with character '-'.
    myTextInput.high.c='-'; // Text panel valid input ends with character '-'. Could end with '.' if real floating number is enabled.
    myTextInput.option=1; // Option 1 incluess 0-9 as valid characters. Option 0, default, option 1 include 0-9 as valid inputs.
    break;

    default:
    break;
  }
  
  ret=input_panel(&myTextInput);
  if (ret!=-1)
  {  
    sscanf(number,"%d%*c%d",&myTextInput.high.i,&myTextInput.low.i);
    *para->ptr.f_buffer=(-1)*(myTextInput.high.i<0?1:-1)*myTextInput.low.i/pow(10.0,(float)after)+myTextInput.high.i;
  }
  return ret;  
}

/**
 * \details Sense the keypad and reacts to the button pushes.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 *
 * This function doesn't print the list so please call render_list before calling this. In fact, please call simple_select_list or select_list instead of using this.
 * Function traps for phi_prompt_select_list_update_ms with wait_on_escape call.
 * Updates are made through the struct.
 * \return Returns buttons pushed so the caller can determine what to do. The function returns 0 if nothing important happens, 1 if enter is pressed, 127 if a refresh is needed such as due to up/down button.
 * It returns -1 if the input is cancelled, -2 if left is pressed on a single column list and -3 if right is pressed on a single column list.
 */

int sense_select_list(phi_prompt_struct * para)
{
    int temp1;
//    byte width=para->width;
    temp1=wait_on_escape(phi_prompt_select_list_update_ms);
    byte columns=para->step.c_arr[1], rows=para->step.c_arr[0];
    switch (temp1) /// The following is the switch-case for key press processing.
    {
        case NO_KEY: /// If no key is pressed, render the list only when needed, such as when the highlighted item is long and needs scrolling.
        return NO_KEY;
        break;
        
        case phi_prompt_up: /// Up is pressed. Move to the previous item.
        if (para->low.i-1>=0) para->low.i--;
        else para->low.i=para->high.i;
        break;
        
        case phi_prompt_down: /// Down is pressed. Move to the next item.
        if ((para->low.i+1)<=(para->high.i)) para->low.i++;
        else para->low.i=0;
        break;
        
        case phi_prompt_left: /// Left is pressed. Move one column to the left if it is a multi-column list. para->step.c_arr[0] stores number of rows for the list. Return -2 if it is a single column list for on_list modification.
        if ((para->step.c_arr[1]>1)&&(para->low.i-para->step.c_arr[0]>=0)) para->low.i-=para->step.c_arr[0];
        else return (-2);
        break;
        
        case phi_prompt_right: /// Right is pressed. Move one column to the right if it is a multi-column list. . para->step.c_arr[0] stores number of rows for the list. Return -3 if it is a single column list for on_list modification.
        if ((para->step.c_arr[1]>1)&&(para->low.i+para->step.c_arr[0]<=para->high.i)) para->low.i+=para->step.c_arr[0];
        else return (-3);
        break;
        
        case phi_prompt_enter: /// Enter is pressed. The function returns 1.
        lcd->noCursor();
        return 1;
        break;
        
        case phi_prompt_escape: /// Escape is pressed. The function returns -1.
        lcd->noCursor();
        return -1;
        break;
        
        default: /// Any other keys will flip the list to the next page.
        if ((para->low.i+columns*rows)<=(para->high.i)) para->low.i+=columns*rows;
        else if (para->low.i==para->high.i) para->low.i=0;
        else para->low.i=para->high.i;
        break;
    }
    return phi_prompt_content_needs_update;
}

/**
 * \details Select from a list with wrap-around capability.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * \par Display option
 * Option 0: display classic list, option 1: display MXN list, option 2: display list with index, option 3: display list with index2, option 4: display MXN list with scrolling
 *
 * This function prints the initial value first so the caller doesn't need to.
 * Function traps until the update is finalized by the left, right, enter button or escape button.
 * Return values are updated throught the pointer.
 * \return Returns buttons pushed so the caller can determine what to do. The function returns 1 if enter is pressed, -1 if the input is cancelled.
 */
int select_list(phi_prompt_struct *para)
{
    int temp1=0;
    byte render=1, width=para->width;
    while(true)
    {
        if ((temp1==phi_prompt_content_needs_update)||(render)) render=render_list(para);// Render the list if update is needed either because the user scrolled or the renderer says so (scrolling long item).
        temp1=sense_select_list(para);
        if ((temp1==1)||(temp1==-1)) return(temp1);
    }
}

/**
 * \details Alphanumerical input panel for texts up to 16 characters.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * If you input the characters with up/down keys, then each character is restricted to start with the character _start and end with character _end.
 * Since up/down key input cycles through all characters, it is slow. This way with _start and _end characters, you don't have to cycle through all characters.
 * If you use a larger keypad to input, such as matrix keypad or multi-tap enabled matrix keypad, the restriction won't apply.
 * The total length will not exceed length.
 * The first letter appears at column, row.
 * The function prints the content of the buffer msg before polling for input.
 * The function fills the buffer after the last character with \0 only if the buffer is not filled. The caller is responsible to fill the character beyond the end of the buffer with \0.
 * Input character options for input panel:
 * Option 0: default, option 1: include 0-9 as valid inputs, option 2: only 0-9 are valid, option 3: only 0-9 are valid and the first digit can be '-', option 4: only 0-9 are valid and number increments, option 5: only 0-9 are valid and the first digit can be '-' and number increments.
 * To do:
 * Option 2-4, automatically increment or decrement a whole series of numbers when they are connected, such as File0009.txt when increase 9 (if only 0-9 are allowed) will give File0010.txt.
 * \return The function returns number of actual characters. The function returns -1 if the input is cancelled.
 */
int input_panel(phi_prompt_struct *para)
{
  byte pointer=0, chr;
  int temp1;
  lcd->setCursor(para->col,para->row);
  lcd->print(para->ptr.msg);
  lcd->setCursor(para->col,para->row);
  lcd->cursor();
  while(true)
  {
    temp1=wait_on_escape(50);
    chr=*(para->ptr.msg+pointer); // Loads the current character.
    switch (temp1) /// The following is the switch-case for key press processing.
    {
      case phi_prompt_up: /// Up is pressed. The character at the cursor location increments to the next allowed character.
      *(para->ptr.msg+pointer)=inc(chr, para);
      lcd->write(*(para->ptr.msg+pointer));
      lcd->setCursor(pointer+para->col,para->row);
      
      //do automatic increment
      break;
      
      case phi_prompt_down: /// Down is pressed. The character at the cursor location decrements to the previous allowed character.
      *(para->ptr.msg+pointer)=dec(chr, para);
      lcd->write(*(para->ptr.msg+pointer));
      lcd->setCursor(pointer+para->col,para->row);
      
      //do automatic decrement
      break;
      
      case phi_prompt_left:  /// Left is pressed. Moves the sursor one character to the left.
      if (pointer>0)
      {
        pointer--;
        lcd->setCursor(pointer+para->col,para->row);
      }
      else
      {
        lcd->noCursor();
        return (-3);
      }
      break;
      
      case '\b': /// Back space is pressed. Erase the current character with space and back up one character.
      *(para->ptr.msg+pointer)=' ';
      lcd->write(*(para->ptr.msg+pointer));
      if (pointer>0)
      {
        pointer--;
      }
      lcd->setCursor(pointer+para->col,para->row);
      break;
      
      case phi_prompt_right: /// Right is pressed. Moves the sursor one character to the right.
      if (pointer<(para->width)-1)
      {
        pointer++;
        lcd->setCursor(pointer+para->col,para->row);
      }
      else
      {
        lcd->noCursor();
        return (-4);
      }
      break;
      
      case phi_prompt_enter: /// Enter is pressed. The function returns 1.
      case '\n': /// New line is received. It is treated the same as Enter.
      lcd->noCursor();
      return(1);
      break;
      
      case phi_prompt_escape: /// Escape is pressed. The function returns -1.
      lcd->noCursor();
      return (-1);
      break;
      
      default: /// Other keys were pressed. Just print the key press to the input panel and advance one character.
      if (temp1==NO_KEY) break;
      *(para->ptr.msg+pointer)=temp1;
      lcd->write(*(para->ptr.msg+pointer));
      if (pointer<(para->width)-1)
      {
        pointer++;
      }
      lcd->setCursor(pointer+para->col,para->row);
      break;
    }
  }
}

/**
 * \details Numerical input panel for a number up to 16 characters. This function is for keypads (or serial keypads) with numerical keys only. If you only have up/down keys, please use input_integer().
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * 
 * Up key outputs a negative sign and moves cursor to the right.
 * Down key outputs a decimal sign and moves cursor to the right.
 * If you use a larger keypad to input, such as matrix keypad or multi-tap enabled matrix keypad, the restriction won't apply.
 * The total length will not exceed length.
 * The first letter appears at column, row.
 * The function prints the content of the buffer msg before polling for input.
 * The function fills the buffer after the last character with \0 only if the buffer is not filled. The caller is responsible to fill the character beyond the end of the buffer with \0.
 * After the function returns, you will need to use sscanf to convert the string into number. See example code.
 * \return The function returns number of actual characters. The function returns -1 if the input is cancelled.
 */
int input_number(phi_prompt_struct *para)
{
  byte pointer=0, chr;
  int temp1;
  lcd->setCursor(para->col,para->row);
  lcd->print(para->ptr.msg);
  lcd->setCursor(para->col,para->row);
  lcd->cursor();
  while(true) 
  {
    temp1=wait_on_escape(50);
    chr=*(para->ptr.msg+pointer); // Loads the current character.
    switch (temp1) /// The following is the switch-case for key press processing.
    {
      case phi_prompt_up: /// Up key outputs a negative sign and moves cursor to the right.
      *(para->ptr.msg+pointer)='-';
      lcd->write(*(para->ptr.msg+pointer));
      if (pointer<(para->width)-1)
      {
        pointer++;
      }
      lcd->setCursor(pointer+para->col,para->row);
      break;
      
      case phi_prompt_down: /// Down key outputs a decimal sign and moves cursor to the right.
      *(para->ptr.msg+pointer)='.';
      lcd->write(*(para->ptr.msg+pointer));
      if (pointer<(para->width)-1)
      {
        pointer++;
      }
      lcd->setCursor(pointer+para->col,para->row);
      break;
      
      case phi_prompt_left: /// Left is pressed. The cursor moves one position to the left. If this moves beyond the first character, the function returns -3. This can be used to switch to an entry to the left.
      if (pointer>0)
      {
        pointer--;
        lcd->setCursor(pointer+para->col,para->row);
      }
      else
      {
        lcd->noCursor();
        return (-3);
      }
      break;
      
      case '\b': /// Back space is pressed. Erase the current character with space and back up one character.
      *(para->ptr.msg+pointer)=' ';
      lcd->write(*(para->ptr.msg+pointer));
      if (pointer>0)
      {
        pointer--;
      }
      lcd->setCursor(pointer+para->col,para->row);
      break;
      
      case phi_prompt_right: /// Right is pressed. The cursor moves one position to the right. If this moves beyond the last character, the function returns -4. This can be used to switch to an entry to the right.
      if (pointer<(para->width)-1)
      {
        pointer++;
        lcd->setCursor(pointer+para->col,para->row);
      }
      else
      {
        lcd->noCursor();
        return (-4);
      }
      break;
      
      case phi_prompt_enter: /// Enter is pressed. The function returns 1.
      case '\n': /// New line is received and is processed just as Enter.
      lcd->noCursor();
      return(1);
      break;
      
      case phi_prompt_escape: /// Escape is pressed. The function returns -1.
      lcd->noCursor();
      return (-1);
      break;
      
      default: /// Other keys were pressed. Accept only number keys.
      if ((temp1>='0')&&(temp1<='9'))
      {
        *(para->ptr.msg+pointer)=temp1;
        lcd->write(*(para->ptr.msg+pointer));
        if (pointer<(para->width)-1)
        {
          pointer++;
        }
        lcd->setCursor(pointer+para->col,para->row);
      }
      break;
    }
  }
}

/**
 * \details Sense the keypad and reacts to the button pushes for text area.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 *
 * This function doesn't print the text_area so please call long_msg_lcd before calling this. In fact, please call simple_long_msg or text_area instead of using this.
 * Function traps for phi_prompt_text_area_update_ms with wait_on_escape call.
 * Updates are made through the struct.
 * \return Returns buttons pushed so the caller can determine what to do. The function returns 0 if nothing important happens, 1 if enter is pressed, 127 if a refresh is needed such as due to up/down button, -1 if the input is cancelled. It also returns number key pressed.
 */

int sense_text_area(phi_prompt_struct* para)
{
//    byte columns=para->step.c_arr[1], rows=para->step.c_arr[0], ch;
    byte temp1=wait_on_escape(50);
    switch (temp1) /// The following is the switch-case for key press processing.
    {
        case NO_KEY: /// If no key is pressed, render the list only when needed, such as when the highlighted item is long and needs scrolling.
        return 0;
        break;
        
        case phi_prompt_up: /// Up is pressed and the message goes to previous line.
        prev_line(para);
        break;
        
        case phi_prompt_down: /// Down is pressed and the message goes to next line.
        next_line(para);
        break;
        
        case phi_prompt_left: /// Left is pressed. Scroll up one page, which is total_row-1 lines.
        for (byte i=0;i<para->step.c_arr[0]-1;i++) prev_line(para);
        break;
        
        case phi_prompt_right: /// Right is pressed.  Scroll down one page, which is total_row-1 lines.
        for (byte i=0;i<para->step.c_arr[0]-1;i++) next_line(para);
        break;
        
        case phi_prompt_enter: /// Enter is pressed. The text_area returns 1.
        return(1);
        break;
        
        case phi_prompt_escape: /// Escape is pressed. The text_area returns -1.
        return (-1);
        break;
        
        default: /// By default if a number key 0-9 is pressed, the function returns this number (in ASCII) as a simple means to implement list/menu.
        if ((temp1>='0')&&(temp1<='9')) return (temp1);
        break;
    }
    return phi_prompt_content_needs_update; // Needs text area update
}

/**
 * \details Displays a text area using message stored in the SRAM.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * It traps execution. It monitors the key pad input and changes message position with up and down keys.
 * When the user presses left and right keys, the message scrolls up or down one page, which is 2 lines if you display a 3-line message or 3 lines if you display a 4-line message.
 * If the user presses 0-9 number keys, the function returns these numbers in ASCII for simple list select functions.
 * \return '0'-'9' if the user presses one of these keys. Enter was pressed (1), Escape was pressed (-1).
 */
int text_area(phi_prompt_struct *para)
{
    byte columns=para->step.c_arr[1], rows=para->step.c_arr[0], ch;
    int temp1=phi_prompt_content_needs_update;
    long_msg_lcd(para);
    while(1)
    {
        if (temp1==phi_prompt_content_needs_update) long_msg_lcd(para);// Render the long message if update is needed because the user scrolled.
        temp1=sense_text_area(para);
        if (((temp1==1)||(temp1==-1))||((temp1>='0')&&(temp1<='9'))) return(temp1); // Return if enter, escape or number keys are pressed
    }
}

/**
 * \details Displays a text area using message stored in PROGMEM.
 * \param para This is the phi_prompt struct to carry information between callers and functions.
 * It traps execution. It monitors the key pad input and changes message position with up and down keys.
 * When the user presses left and right keys, the message scrolls up or down one page, which is 2 lines if you display a 3-line message or 3 lines if you display a 4-line message.
 * If the user presses 0-9 number keys, the function returns these numbers in ASCII for simple list select functions.
 * \return '0'-'9' if the user presses one of these keys. Enter was pressed (1), Escape was pressed (-1).
 */
int text_area_P(phi_prompt_struct *para)
{
  byte columns=para->step.c_arr[1], rows=para->step.c_arr[0], ch;
  long_msg_lcd_P(para);
  while(true)
  {
    byte temp1=wait_on_escape(50);
    switch (temp1) /// The following is the switch-case for key press processing.
    {
      case phi_prompt_up: /// Up is pressed and the message goes to previous line.
      prev_line_P(para);
      long_msg_lcd_P(para);
      break;
      
      case phi_prompt_down: /// Down is pressed and the message goes to next line.
      next_line_P(para);
      long_msg_lcd_P(para);
      break;

      case phi_prompt_left: /// Left is pressed. Scroll up one page, which is total_row-1 lines.
      for (byte i=0;i<para->step.c_arr[0]-1;i++) prev_line_P(para);
      long_msg_lcd_P(para);
      break;
      
      case phi_prompt_right: /// Right is pressed.  Scroll down one page, which is total_row-1 lines.
      for (byte i=0;i<para->step.c_arr[0]-1;i++) next_line_P(para);
      long_msg_lcd_P(para);
      break;
      
      case phi_prompt_enter: /// Enter is pressed. The text_area returns 1.
      return(1);
      break;
      
      case phi_prompt_escape: /// Escape is pressed. The text_area returns -1.
      return (-1);
      break;
      
      default: /// By default if a number key 0-9 is pressed, the function returns this number (in ASCII) as a simple means to implement list/menu.
      if ((temp1>='0')&&(temp1<='9')) return (temp1);
      break;
    }
  }
}

/**
 * \details Displays a short message with yes/no options.
 * \param msg This is the message to be displayed with the yes/no choice.
 * It traps execution. It monitors the key pad input and toggles between yes and no with up and down keys.
 * The function auto scales to fit on any LCD.
 * \return Returns 0 for no choice 1 for yes choice, -1 if Escape was pressed.
 */
int yn_dialog(char msg[])
{
  phi_prompt_struct yn_list;
  
  lcd->clear(); // Clear the lcd.
// Use the yn_list struct to display the message as a long message to enable multiple line question.
  yn_list.ptr.msg=msg; // Assign the address of the text string to the pointer.
  yn_list.low.i=0; // Default text starting position. 0 is highly recommended.
  yn_list.high.i=((sizeof(msg)-1)>lcd_w*lcd_h-10)?lcd_w*lcd_h-10:(sizeof(msg)-1); // Position of the last character in the text string, which is size of the string - 1.
  yn_list.step.c_arr[0]=lcd_h; // row
  yn_list.step.c_arr[1]=lcd_w; // column
  yn_list.col=0; // Display the text area starting at column 0
  yn_list.row=0; // Display the text area starting at row 0
  yn_list.option=0; // Option 0, display classic message, option 1, display message with scroll bar on right.

  long_msg_lcd(&yn_list);

  yn_list.ptr.list=(char**)&yn_items; // Assign the address of the list array to the pointer.
  yn_list.low.i=0; // Default item highlighted on the list is #0, the first item on the list.
  yn_list.high.i=1; // Last item on the list is size of the list - 1.
  yn_list.width=9; // Length in characters of the longest list item, for screen update purpose.
  yn_list.col=lcd_w-9; // Display the list a column to make it right-alighed
  yn_list.row=lcd_h-1; // Display the list at a row to make it bottom-alighed
  yn_list.step.c_arr[0]=1; // 1 row
  yn_list.step.c_arr[1]=1; // 1 column
  yn_list.option=0; // Option 3 is an indexed list for clarity. Option 0, display classic list, option 1, display MXN list, option 2, display list with index, option 3, display list with index2.

  if (select_list(&yn_list)!=-1) return yn_list.low.i; //If the user didn't press escape (return -1) then update the user choice with the value in myListInput.low.
}

/**
 * \details Displays a short message with OK.
 * \param msg This is the message to be displayed with the OK button.
 * It traps execution. It monitors the key pad input and returns if any key is pressed.
 * The function auto scales to fit on any LCD.
 * \return It returns no value but for compatibility it is defined to return integer.
 */
int ok_dialog(char msg[])
{
  phi_prompt_struct yn_list;
  
  lcd->clear(); // Clear the lcd.
// Use the yn_list struct to display the message as a long message to enable multiple line question.
  yn_list.ptr.msg=msg; // Assign the address of the text string to the pointer.
  yn_list.low.i=0; // Default text starting position. 0 is highly recommended.
  yn_list.high.i=((sizeof(msg)-1)>lcd_w*lcd_h-5)?lcd_w*lcd_h-5:(sizeof(msg)-1); // Position of the last character in the text string, which is size of the string - 1.
  yn_list.step.c_arr[0]=lcd_h; // row
  yn_list.step.c_arr[1]=lcd_w; // column
  yn_list.col=0; // Display the text area starting at column 0
  yn_list.row=0; // Display the text area starting at row 0
  yn_list.option=0; // Option 0, display classic message, option 1, display message with scroll bar on right.

  long_msg_lcd(&yn_list);

  lcd->setCursor((lcd_w-4),lcd_h-1);
  lcd->print(">OK<");
  
  while(wait_on_escape(500)==0)
  {
  }
}

#ifndef disable_simple_fns

// The following are simple setup functions to quickly set up the library
/**
 * \details Set up the library with LCD object and 6 buttons.
 * \param lcd This is the lcd object
 * \param columns This is the width or columns of the lcd
 * \param rows This is the rows of the lcd
 * \param btn_u This and the next five are six digital pins for 6 button pins
 */
void simple_setup_6_buttons(LCD * lcd, byte columns, byte rows, byte btn_u, byte btn_d, byte btn_l, byte btn_r, byte btn_b, byte btn_a)
{
  static char up_keys[]={"U"}; ///< All keys that act as the up key are listed here.
  static char down_keys[]={"D"}; ///< All keys that act as the down key are listed here.
  static char left_keys[]={"L"}; ///< All keys that act as the left key are listed here.
  static char right_keys[]={"R"}; ///< All keys that act as the right key are listed here.
  static char enter_keys[]={"B"}; ///< All keys that act as the enter key are listed here.
  static char escape_keys[]={"A"}; ///< All keys that act as the escape key are listed here.
  static char * fn_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.
  static byte pins[]={btn_u,btn_d,btn_l,btn_r,btn_b,btn_a};  ///< The digital pins for buttons
  static char mapping[]={'U','D','L','R','B','A'};           ///< This is a list of names for each button.
  static phi_button_groups* my_btns= new phi_button_groups(mapping, pins, 6);    
  static multiple_button_input * keypads[]={my_btns, 0};
  init_phi_prompt(lcd, keypads, fn_keys, columns, rows, '~');
}

/**
 * \details Set up the library with phi-2 shield LCD and button pins. All pins are implied as Phi-2 shield setup
 * \return It returns the liquid crystal object address, in case you need it for future reference.
 */
LCD * simple_setup_phi_2(byte columns,byte rows)
{
  static char up_keys[]={"U"}; ///< All keys that act as the up key are listed here.
  static char down_keys[]={"D"}; ///< All keys that act as the down key are listed here.
  static char left_keys[]={"L"}; ///< All keys that act as the left key are listed here.
  static char right_keys[]={"R"}; ///< All keys that act as the right key are listed here.
  static char enter_keys[]={"B"}; ///< All keys that act as the enter key are listed here.
  static char escape_keys[]={"A"}; ///< All keys that act as the escape key are listed here.
  static char * fn_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.
  static byte pins[]={5, 10, 11, 4, A0, A1};  ///< The digital pins for buttons
  static char mapping[]={'U','D','L','R','B','A'};           ///< This is a list of names for each button.
  static phi_button_groups* my_btns= new phi_button_groups(mapping, pins, 6);    
  static multiple_button_input * keypads[]={my_btns, 0};
  static LCD * lcd= new LiquidCrystal(8,9,7,6,2,3);
  lcd->begin(columns,rows);
  init_phi_prompt(lcd, keypads, fn_keys, columns, rows, '~');
}

/**
 * \details Set up the library with phi-3 shield LCD and analog button pin. All pins are implied as Phi-3 shield setup
 * \return It returns the liquid crystal object address, in case you need it for future reference.
 */
LCD * simple_setup_phi_3(byte columns,byte rows)
{
  static char up_keys[]={"U"}; ///< All keys that act as the up key are listed here.
  static char down_keys[]={"D"}; ///< All keys that act as the down key are listed here.
  static char left_keys[]={"L"}; ///< All keys that act as the left key are listed here.
  static char right_keys[]={"R"}; ///< All keys that act as the right key are listed here.
  static char enter_keys[]={"B"}; ///< All keys that act as the enter key are listed here.
  static char escape_keys[]={"A"}; ///< All keys that act as the escape key are listed here.
  static char * fn_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.
  static byte pins[]={ A0};  ///< The digital pins for buttons
  static int values[]={0, 417, 562, 647, 672, 709}; //These numbers need to increase monotonically.
  static char mapping[]={'U','D','L','R','B','A'};           ///< This is a list of names for each button.
  static phi_analog_keypads* my_btns= new phi_analog_keypads(mapping, pins, values, 1, 6);    
  static multiple_button_input * keypads[]={my_btns, 0};
  static LCD * lcd= new LiquidCrystal_I2C(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
  lcd->begin(columns,rows);
  init_phi_prompt(lcd, keypads, fn_keys, columns, rows, '~');
}

/**
 * \details Return pointer to the lcd, stored by phi_prompt_init
 * \return It returns the liquid crystal object address.
 */
LCD * get_lcd_ptr()
{
  return lcd;
}

/**
 * \details Return pointer to the multiple_button_input, stored by phi_prompt_init
 * \return It returns the multiple_button_input array address.
 */
multiple_button_input ** get_mbi_ptr()
{
  return mbi_ptr;
}             

void simple_update_function(void (*u_f)(phi_prompt_struct*))
{
    shared_struct.update_function=u_f;
}
// Simple functions
/**
 * \details Collect user input. Returns the user input in a string.
 * \param msg This is the message to be displayed as the initial content of the input panel, which you can modify. Warning: this string is used to store user input and is thus modified.
 * \param lo This is the lower limit of each character, when inputted with up/down. With lo='A', you can only enter characters with ASCII values equal or above 'A'. This only applies to inputs made with up/down keys.  
 * \param h This is the upper limit of each character, when inputted with up/down. With h='Z', you can only enter characters with ASCII values equal or below 'Z'. This only applies to inputs made with up/down keys.  
 * \param opt This is the option in addition to the upper and lower limits. Option 0, default, option 1 include 0-9 as valid inputs. This only applies to inputs made with up/down keys.
 *
 * It traps execution. It monitors the key pad input and returns if any key is pressed.
 * The function auto scales to fit on any LCD with size determined by phi_prompt initialization.
 * The function doesn't clear screen and prints on row 1 or 2 depending on display size. You can print a prompt on row 0 and then call this function.
 * \par Example
 * simple_input_panel(msg, 'A', 'Z', 1) will have 6 digits for input and restricted within numbers 0-9 and letters A-Z on each character. Please define char msg[]="000000".
 * "000000" will set up initial content of a numeric input with up to 6 digits. To enter 123, just make the change with your keypad so the string reads "000123".
 * \return It returns the length of the string. The actual user input is already in the string msg.
 */
byte simple_input_panel(char msg[],byte lo, byte h, byte opt)
{
  byte msg_len=strlen(msg);

  shared_struct.ptr.msg=msg; // Assign the text buffer address
  shared_struct.low.c=lo; // Text panel valid input starts with character 'A'.
  shared_struct.high.c=h; // Text panel valid input ends with character 'Z'.
  shared_struct.width=msg_len; // Length of the input panel in characters.
  shared_struct.col=(lcd_w-msg_len)/2; // Display input panel at column 2
  shared_struct.row=lcd_h/2; // Display input panel at row 1 for 2-row display or row 2 for 4-row display
  shared_struct.option=opt; // Option 0, default, option 1 include 0-9 as valid inputs.
  if (input_panel(&shared_struct)!=-1) // input_panel stores user choice in file_name.
  {  
    return strlen(msg); // Recall the text entered by the user.
  }
  return 255;  
}

/**
 * \details Prepares a menu/list from a formatted string. It replaces the \n with \0, populates the string pointers to each item, prepares values for shared_struct.
 * It also prints out the title. No clear screen is called to prevent flashing. You may read the simple_select_list code to find out how to do dynamic menu content.
 * You will need to add some update to the content. The program sets options according to need and simple_option value, which is set by simple_select_list_XXX().
 * \param msg This is the message to be displayed as menu/list. This message must be formatted as "Menu_title\nItem1\nItem2\nLast_item\n". The '\n' is new line.
 * \param highlighted This is the highlighted item in the menu/list. Default value is 0 so the first item on the list is highlighted.
 * \param selections This is the selections of a multi-select list. Default value is -1 so the list is not a multi-select list.
 * The first line Menu_title will be displayed on line one of the display. The rest of the lines will be treated as menu/list items.
 * Warning: There is a maximal of simple_select_list_max_items menu/items to this function. To use more items, you need to use the more feature-rich, flexible, but complex select_list().
 *
 * It preapres the list by transforming the \n separated list into \0 separated list and an array of index. To display a dynamically updating list, this should be step 1.
 * The function auto scales to fit on any LCD with size determined by phi_prompt initialization.
 * \par Example
 * prepare_simple_select_list(msg) will parpare a select list with the 12 months and highlights the first item.
 * Please define char msg[]="Select month\nJAN\nFEB\nMAR\nAPR\nMAY\nJUN\nJUL\nAUG\nSEP\nOCT\nNOV\nDec\n".
 */
void prepare_simple_select_list(char msg[], byte highlighted, long selections) 
{
    int ret_val;
    static char * ptr_array[simple_select_list_max_items]; // Only up to 16 list items is allowed for a simple list.
    byte item_len=0, max_item_len=0;
    int i=0;
    shared_struct.high.i=0;
    while (msg[i]!='\0') // Convert the simple list into phi_compatible form
    {
        if (msg[i]=='\n')
        {
            msg[i]='\0';
            if (msg[i+1]!='\0')
            {
                if ((selections!=-1L)&&(shared_struct.high.i>0)) msg[i-1]=(bitRead(selections,shared_struct.high.i-1))?'*':' '; // Only add * or space if this is an item, not the title and it is a multi-select list
                shared_struct.high.i++;
                ptr_array[shared_struct.high.i-1]=&(msg[i+1]);
            }
            else if ((selections!=-1L)&&(shared_struct.high.i>1)) msg[i-1]=(bitRead(selections,shared_struct.high.i-1))?'*':' '; // Only add * or space if this is an item, not the title and it is a multi-select list
            if (item_len>max_item_len)
            {
                max_item_len=item_len;
            }
            item_len=0;
        }
        else
        {
            if (shared_struct.high.i>0) item_len++; // Only count item length, skipping the first string, the prompt.
        }
        if (shared_struct.high.i==simple_select_list_max_items-1) break;
        i++;
    }
    shared_struct.high.i--; //Now it becomes the last item number, total item number-1.
    shared_struct.ptr.list=ptr_array; // Assign the list to the pointer
    shared_struct.low.i=(highlighted<shared_struct.high.i)?highlighted:shared_struct.high.i; // Default item highlighted on the list is either b or the maximal item number
    shared_struct.width=(max_item_len>lcd_w-2)?lcd_w-2:(lcd_w-1)/((lcd_w-1)/(max_item_len+1))-1; // Length in characters of each list item. If the longest list item is too long, use lcd_w-1.
    shared_struct.col=0; // Display prompt at column 0
    shared_struct.row=1; // Display prompt at row 1
    shared_struct.option=phi_prompt_list_in_SRAM+phi_prompt_arrow_dot;
    shared_struct.step.c_arr[0]=(shared_struct.high.i+1>=lcd_h-1)?lcd_h-1:shared_struct.high.i+1; // rows to auto fit entire screen but if only less items than available screen lines-1, use actual item number.
    shared_struct.step.c_arr[1]=(max_item_len>lcd_w-2)?1:(lcd_w-1)/(max_item_len+1); // number of columns of the list. If the longest list item is too long, display with one column.
    if ((shared_struct.step.c_arr[1]==1)&&((simple_option&phi_prompt_auto_scroll)!=0)) shared_struct.option+=phi_prompt_auto_scroll; // Only turn on auto scroll if for 1 column list and it is required by simple_option.
    if ((shared_struct.step.c_arr[1]==1)&&((simple_option&phi_prompt_scroll_bar)!=0)) shared_struct.option+=phi_prompt_scroll_bar; // Only turn on scroll bar if for 1 column list and it is required by simple_option.
    if ((shared_struct.step.c_arr[1]==1)&&((simple_option&phi_prompt_center_choice)!=0)) shared_struct.option+=phi_prompt_center_choice; // Only turn on center choice for 1 column list and it is required by simple_option.
    lcd->setCursor(0,0);
    lcd->print(msg);
}

/**
 * \details Enable or disable center_select list choice. You no longer have to use it after prepare_simple_select_list and before render_simple_select_list. Just use before simple_select_list().
 * \param b If it is true, center_choice is enabled, else it is disabled.
 */
void simple_select_list_center_choice(byte b)
{
    if (b) simple_option|=(int)phi_prompt_center_choice;
    else simple_option&=(~(int)phi_prompt_center_choice);
}

/**
 * \details Enable or disable scroll_bar.  You no longer have to use it after prepare_simple_select_list and before render_simple_select_list. Just use before simple_select_list(). This function adjusts item length accordingly
 * \param b If it is true, scroll_bar is enabled, else it is disabled.
 */
void simple_select_list_scroll_bar(byte b)
{
    if (b)
    {
        if (!(simple_option&phi_prompt_scroll_bar)) //if the scroll bar was not selected
        {
            shared_struct.width--; //Decrease length of each item to accommodate the scroll bar only if the scroll bar was not selected. 
            simple_option|=(int)phi_prompt_scroll_bar;
        }
    }
    else
    {
        if (simple_option&phi_prompt_scroll_bar) //if the scroll bar was selected before
        {
            shared_struct.width++; // Increase length of each item to take the space of the scroll bar only if the scroll bar was selected before
            simple_option&=(~(int)phi_prompt_scroll_bar);
        }
    }
}

/**
 * \details Enable or disable auto_scroll.  You no longer have to use it after prepare_simple_select_list and before render_simple_select_list. Just use before simple_select_list().
 * \param b If it is true, auto_scroll is enabled, else it is disabled.
 */
void simple_select_list_auto_scroll(byte b)
{
    if (b) simple_option|=(int)phi_prompt_auto_scroll;
    else simple_option&=(~(int)phi_prompt_auto_scroll);
}

/**
 * \details This restores the string sent to prepare_simple_select_list, by replacing all the \0 with \n, reversing the modification prepare_simple_select_list does to the status before prepare_simple_select_list is called.
 * Use it only on strings that are to be reused again as select lists. If the string is dynamically rendered and resides in a shared buffer, there is no need to restore the modification done by prepare_simple_select_list.
 * The dynamic string will likely be created again next time the list needs to be displayed. Only use this restore when you need to restore the string so that you will use it again especially in a different place in your code where you can't assume the string has been prepared for display.
 * It assume the shared_struct has been filled with the right info to start with.
 */

void restore_string_for_simple_select_list()
{
    for(byte i=0;i<=shared_struct.high.i;i++) // Restore the string. Replace \0 with \n.
    {
        *(shared_struct.ptr.list[i]-1)='\n';
        if (i==shared_struct.high.i)
        {
            byte j=0;
            while(*(shared_struct.ptr.list[i]+j)!=0)
        {
            j++;
        }
            *(shared_struct.ptr.list[i]+j)='\n';
        }
    }
}

byte render_simple_select_list()
{
    return render_list(&shared_struct);
}

byte sense_simple_select_list()
{
    return (byte)sense_select_list(&shared_struct);
}

byte get_simple_select_list_item()
{
    return (byte)(shared_struct.low.i);
}

byte set_simple_select_list_item(byte b)
{
    shared_struct.low.i=b;
}

/**
 * \details Displays a menu/list from a formatted string. Returns the item selected.
 * \param msg This is the message to be displayed as menu/list. This message must be formatted as "Menu_title\nItem1\nItem2\nLast_item\n". The '\n' is new line.
 * The first line Menu_title will be displayed on line one of the display. The rest of the lines will be treated as menu/list items.
 * The user can scroll through the items while the title is always displayed on line one.
 * Warning: There is a maximal of 16 menu/items to this function. To use more items, you need to use the more feature-rich, flexible, but complex select_list().
 *
 * It traps execution. It monitors the key pad input and returns if any key is pressed.
 * The function auto scales to fit on any LCD with size determined by phi_prompt initialization.
 * \par Example
 * simple_select_list(msg) will display a select list with the 12 months on your display.
 * Please define char msg[]="Select month\nJAN\nFEB\nMAR\nAPR\nMAY\nJUN\nJUL\nAUG\nSEP\nOCT\nNOV\nDec\n".
 * \return It returns the item the user selected, the first item being number zero. If escape is pressed, it returns 255.
 */
byte simple_select_list(char msg[], byte highlighted, long selections) 
{
    byte response;
    lcd->clear();
    prepare_simple_select_list(msg,highlighted, selections);
    while (1)
    {
        render_simple_select_list();
        response=sense_simple_select_list();
        if (response==1)
        {
            response=get_simple_select_list_item();
            break;
        }
        if (response==255) break;
    }
    restore_string_for_simple_select_list();
    return response;
}

/**
 * \details Prepares a text area from a formatted string. It prepares values for shared_struct. It also sets the lcd to no blink and no cursor. No clear screen is called to prevent flashing. You may read the simple_text-area code to find out how to do dynamic text_area content. You will need to add some update to the content.
 * \param msg This is the message to be displayed as text area message.
 * \param highlighted This is the current text area pointer. Default value is 0 so the line is displayed.
 */
void prepare_simple_text_area(char* msg, byte highlighted)
{
  lcd->noCursor();
  lcd->noBlink();
  shared_struct.ptr.msg=msg; // Assign the address of the text string to the pointer.
  shared_struct.low.i=highlighted; // Default text starting position. 0 is highly recommended.
  shared_struct.high.i=strlen(msg)-1; // Position of the last character in the text string, which is size of the string - 1.
  shared_struct.step.c_arr[0]=lcd_h; // rows to auto fit entire screen
  shared_struct.step.c_arr[1]=lcd_w-1;
  shared_struct.col=0; // Display the text area starting at column 0
  shared_struct.row=0; // Display the text area starting at row 0
  shared_struct.option=1; // Option 0, display classic message, option 1, display message with scroll bar on right.
}

void render_simple_text_area()
{
    long_msg_lcd(&shared_struct);
}

byte sense_simple_text_area()
{
    return (byte)sense_text_area(&shared_struct);
}

byte get_simple_text_area_location()
{
    return (byte)(shared_struct.low.i);
}

byte set_simple_text_area_location(byte b)
{
    shared_struct.low.i=b;
}

/**
 * \details Displays a text_area to fill the size of the screen.
 * \param msg This is the message to be displayed with formatting characters such as '\n' and '\t' in the message.
 *
 * It traps execution until the user made a choice of '0'-'9' or enter/escape.
 * The function auto scales to fit on any LCD with size determined by phi_prompt initialization.
 * \par Example
 * simple_text_area("Designed by Dr. Liu\nVersion:1.0\nFree software") will display a long message on LCD.
 * \return It returns 1 for enter 255 for escape, or '0'-'9' for number keys.
 */
byte simple_text_area(char msg[], byte highlighted)
{
    byte response;
    lcd->clear();
    prepare_simple_text_area(msg, highlighted);
    while (1)
    {
        render_simple_text_area();
        response=sense_simple_text_area();
        if ((response!=NO_KEY)&&(response!=phi_prompt_content_needs_update))// Not escaping or updating text_area
        {
            return response;
        }
    }
}

/**
 * \details Displays a long message to fill the size of the screen. It returns 0. It doesn't clear the screen.
 * \param msg This is the message to be displayed with formatting characters such as '\n' and '\t' in the message.
 *
 * It doesn't trap execution.
 * The function auto scales to fit on any LCD with size determined by phi_prompt initialization.
 * \par Example
 * simple_formatted_msg("Designed by Dr. Liu\nVersion:1.0\nFree software") will display a long message on LCD.
 * \return It returns 0.
 */
byte simple_formatted_msg(char msg[])
{
  lcd->noCursor();
  lcd->noBlink();
  shared_struct.ptr.msg=msg; // Assign the address of the text string to the pointer.
  shared_struct.low.i=0; // Default text starting position. 0 is highly recommended.
  shared_struct.high.i=strlen(msg)-1; // Position of the last character in the text string, which is size of the string - 1.
  shared_struct.step.c_arr[0]=lcd_h; // rows to auto fit entire screen
  shared_struct.step.c_arr[1]=lcd_w;
  shared_struct.col=0; // Display the text area starting at column 0
  shared_struct.row=0; // Display the text area starting at row 0
  shared_struct.option=0; // Option 0, display classic message, option 1, display message with scroll bar on right.
  long_msg_lcd(&shared_struct);
  return (0);
}
/**
 * \details Extracts the title from a simple list msg. Returns whether the title is extracted.
 * \param msg This is the message of a menu/list. This message must be formatted as "Menu_title\nItem1\nItem2\nLast_item\n". The '\n' is new line.
 * \param buf This is the buffer to store extracted menu/list title. It must be long enough to hold the title to be extracted.
 *
 * If you display a simple list, chances are you will want to later display the title of the list in text form. This function extracts the title.
 * Since the first item in a simple list is the list title, it simply returns this item AKA title. To get the simple list item, call get_simple_list_item.
 * \return It returns whether the title is extracted, 0 for success, other values for failure. The extracted title is in buf.
 */
byte get_simple_list_title(char msg[], char buf[])
{
    byte i=0;
    while(msg[i]!='\n')
    {
        buf[i]=msg[i];
        i++;
    }
    buf[i]=0;   ///< Terminate the string.
}

/**
 * \details Extracts the "item_num" item from a simple list msg. Returns whether the item is extracted.
 * \param msg This is the message of a menu/list. This message must be formatted as "Menu_title\nItem1\nItem2\nLast_item\n". The '\n' is new line.
 * \param buf This is the buffer to store extracted menu/list item. It must be long enough to hold the item to be extracted.
 * \param item_num This is the menu/list item to be extracted. It is zero-based.
 *
 * If you display a simple list, chances are you will want to later display the selected item in text form. This function extracts that item.
 * Since the first item in a simple list is the list title, it skips it. To get the simple list title, call get_simple_list_title.
 * \return It returns whether the item is extracted, 0 for success, other values for failure. The extracted item is in buf.
 */
byte get_simple_list_item(char msg[], char buf[], byte item_num)
{
    byte j=0;
    int i=0;
    while(j<=item_num)
    {
        while(msg[i]!='\n'){i++;}
        i++;
        j++;
    }
    j=0;
    while(msg[i]!='\n')
    {
        buf[j]=msg[i];
        i++;
        j++;
    }
    buf[j]=0;   ///< Terminate the string.
}
#endif
