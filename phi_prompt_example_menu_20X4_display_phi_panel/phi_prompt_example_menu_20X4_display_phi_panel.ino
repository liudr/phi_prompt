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

#define lcd_rows 4
#define lcd_columns 20

// LCD pin setting
//Phi-panel LCD pin setting
#define LCD_RS 2
#define LCD_EN 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

#include <LiquidCrystal.h>
#include <phi_interfaces.h>
#include <phi_prompt.h>

LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

char mapping[]={'1','2','3',1,'4','5','6',2,'7','8','9',3,5,'0',6,4}; // This is a matrix keypad.
byte pins[]={17, 16, 15, 13, 12, 11, 9, 8}; // Both 3*4 and 4*4 matrix keypads can use this definition 
phi_matrix_keypads my_btns(mapping, pins, 4, 4);
phi_serial_keypads debug_keypad(&Serial,9600);
multiple_button_input * keypads[]={&my_btns,&debug_keypad,0};
char up_keys[]={1,0}; ///< All keys that act as the up key are listed here.
char down_keys[]={2,0}; ///< All keys that act as the down key are listed here.
char left_keys[]={3,0}; ///< All keys that act as the left key are listed here.
char right_keys[]={4,0}; ///< All keys that act as the right key are listed here.
char enter_keys[]={5,0}; ///< All keys that act as the enter key are listed here.
char escape_keys[]={6,0}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.

PROGMEM prog_char msg_00[]="Developed by:\nDr.Liu 05/23/11\nhttp://liudr.wordpress.com\nThis is just a mock-up of an actual data acquisition system with a 2-level menu.\nIt serves as a template for your actual project. It also shows off various features of the phi_prompt library.\nGo in \"Set Menu Style\" to find out some menu features you could be using in your project.\nPress Confirm to continue";
// Setting up two menus top and sub_1

void setup()
{
  Serial.begin(9600);
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
  show_credit();
}

void loop()
{
// A two-level menu using two lists, one for top menu and one for a sub menu. Create any level of menu you like.
/*
.___  ___.  _______ .__   __.  __    __  
|   \/   | |   ____||  \ |  | |  |  |  | 
|  \  /  | |  |__   |   \|  | |  |  |  | 
|  |\/|  | |   __|  |  . `  | |  |  |  | 
|  |  |  | |  |____ |  |\   | |  `--'  | 
|__|  |__| |_______||__| \__|  \______/  
*/
  top_menu(); // See Example_menu.pde
}

void show_credit()
{
  // Display credits
  phi_prompt_struct myLongMsg;

  lcd.clear();
  lcd.noBlink();
  center_text("Credits"); // Prompt user for input. Use center_text to display center-aligned.
  myLongMsg.ptr.msg_P=msg_00; // Assign the address of the text string to the pointer.
  myLongMsg.low.i=0; // Default text starting position. 0 is highly recommended.
  myLongMsg.high.i=strlen_P(msg_00); // Position of the last character in the text string, which is size of the string - 1.
  myLongMsg.step.c_arr[0]=lcd_rows-1; // rows to auto fit entire screen
  myLongMsg.step.c_arr[1]=lcd_columns-1; // one col list
  myLongMsg.col=0; // Display the text area starting at column 0
  myLongMsg.row=1; // Display the text area starting at row 0
  myLongMsg.option=1; // Option 0, display classic message, option 1, display message with scroll bar on right.

  text_area_P(&myLongMsg);
}


