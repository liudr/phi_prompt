// Enter a string with phi_prompt for Arduino Phi-2 shield:
/*
     _______.___________..______       __  .__   __.   _______ 
    /       |           ||   _  \     |  | |  \ |  |  /  _____|
   |   (----`---|  |----`|  |_)  |    |  | |   \|  | |  |  __  
    \   \       |  |     |      /     |  | |  . `  | |  | |_ | 
.----)   |      |  |     |  |\  \----.|  | |  |\   | |  |__| | 
|_______/       |__|     | _| `._____||__| |__| \__|  \______| 

http://liudr.wordpress.com
*/

#define lcd_rows 2
#define lcd_columns 16

//The following button pins apply to phi-1 and phi-2 shields. Please make appropriate modification for your own setup.
// For phi-1 shield btn_r is 3. For phi-2 shield btn_r is 4
#define btn_u 5
#define btn_d 10
#define btn_l 11
#define btn_r 4
#define btn_b 14
#define btn_a 15
#define total_buttons 6

// LCD pin setting
// For phi-1 shield LCD_D7 is 4. For phi-2 shield LCD_D7 is 3
#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 2
#define LCD_D7 3

#include <LiquidCrystal.h>
#include <phi_interfaces.h>
#include <phi_prompt.h>

LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

char mapping[]={1,2,3,4,5,6}; // This is a list of names for each button.
byte pins[]={btn_u,btn_d,btn_l,btn_r,btn_b,btn_a}; // The digital pins connected to the 6 buttons.
phi_button_groups my_btns(mapping, pins, total_buttons);
phi_serial_keypads debug_keypad(&Serial,9600);
multiple_button_input * keypads[]={&my_btns,&debug_keypad,0};
char up_keys[]={1,0}; ///< All keys that act as the up key are listed here.
char down_keys[]={2,0}; ///< All keys that act as the down key are listed here.
char left_keys[]={3,0}; ///< All keys that act as the left key are listed here.
char right_keys[]={4,0}; ///< All keys that act as the right key are listed here.
char enter_keys[]={5,0}; ///< All keys that act as the enter key are listed here.
char escape_keys[]={6,0}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.

void setup()
{
  Serial.begin(9600);
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
}

void loop()
{
  int result;
  phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.

  char file_name[]="AAAAAAAA.TXT"; // This is the buffer that will store the content of the text panel. 
  myTextInput.ptr.msg=file_name; // Assign the text buffer address
  myTextInput.low.c='A'; // Text panel valid input starts with character 'A'.
  myTextInput.high.c='Z'; // Text panel valid input ends with character 'Z'.
  myTextInput.width=12; // Length of the input panel is 12 characters.
  myTextInput.col=2; // Display input panel at column 2
  myTextInput.row=1; // Display input panel at row 1
  myTextInput.option=1; // Option 1 incluess 0-9 as valid characters. Option 0, default, option 1 include 0-9 as valid inputs.
  lcd.clear(); // Clear the lcd
  lcd.print("File name:"); // Prompt user for input

  if (input_panel(&myTextInput)!=-1) // input_panel stores user choice in file_name.
  {  
    lcd.clear();
    lcd.print("Your choice was:"); // Display the user choice one more time
    lcd.setCursor(0,1); // Display the list at column 2 row 1.
    lcd.print(file_name); // Recall the text entered by the user.
  }
  else 
  {
    lcd.clear();
    lcd.print("You pressed ESC");
  }
  wait_on_escape(4000);
}
  
