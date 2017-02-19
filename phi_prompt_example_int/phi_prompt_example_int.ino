// Input an integer with phi_prompt for Arduino Phi-2 shield:
/*
 __  .__   __. .___________. _______   _______  _______ .______      
|  | |  \ |  | |           ||   ____| /  _____||   ____||   _  \     
|  | |   \|  | `---|  |----`|  |__   |  |  __  |  |__   |  |_)  |    
|  | |  . `  |     |  |     |   __|  |  | |_ | |   __|  |      /     
|  | |  |\   |     |  |     |  |____ |  |__| | |  |____ |  |\  \----.
|__| |__| \__|     |__|     |_______| \______| |_______|| _| `._____|


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

char mapping[]={'U','D','L','R','B','A'}; // This is a list of names for each button.
byte pins[]={btn_u,btn_d,btn_l,btn_r,btn_b,btn_a}; // The digital pins connected to the 6 buttons.
phi_button_groups my_btns(mapping, pins, total_buttons);
phi_serial_keypads debug_keypad(&Serial,9600);
multiple_button_input * keypads[]={&my_btns,&debug_keypad,0};
char up_keys[]={"1U"}; ///< All keys that act as the up key are listed here.
char down_keys[]={"2D"}; ///< All keys that act as the down key are listed here.
char left_keys[]={"3L"}; ///< All keys that act as the left key are listed here.
char right_keys[]={"4R"}; ///< All keys that act as the right key are listed here.
char enter_keys[]={"5B"}; ///< All keys that act as the enter key are listed here.
char escape_keys[]={"6A"}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.

void setup()
{
  Serial.begin(9600);
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
}

void loop()
{
  int user_input=10; // This is the storage for the integer
  phi_prompt_struct myIntegerInput;

  myIntegerInput.ptr.i_buffer=&user_input; // Pass the address of the buffer. After library function call, user input will be stored in this variable.
  myIntegerInput.low.i=0; // Lower limit of the number is 0
  myIntegerInput.high.i=20; // Upper limit of the number is 20
  myIntegerInput.step.i=2; // Step size. User choice increments 2 every time you push up and decrements 2 when you press down.
  myIntegerInput.col=7; // Display number at column 7
  myIntegerInput.row=1; // Display number at row 1
  myIntegerInput.width=2; // The number occupies 2 characters space.
  myIntegerInput.option=0; // Option 0, space pad right, option 1, zero pad left, option 2, space pad left.
  lcd.clear();
  lcd.print("Number(0-20):"); // Prompt user for input

  input_integer(&myIntegerInput); // This actually calls the library function to ask user for input. The input is stored in user_input, not the return value. The call supplies the address ("&") of the phi_prompt structure to the library function input_integer.
  
  lcd.clear();
  lcd.print("Your choice was:"); // Display the user choice one more time
  lcd.setCursor(7,1);
  lcd.print(user_input); // Prompt user for input
  wait_on_escape(4000);
}
  
