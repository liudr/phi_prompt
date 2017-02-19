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

#include <LiquidCrystal.h>
#include <phi_interfaces.h>
#include <phi_prompt.h>

#define lcd_rows 4
#define lcd_columns 20

//The following button pins apply to phi-1 and phi-2 shields. Please make appropriate modification for your own setup.
// For phi-1 shield btn_r is 3. For phi-2 shield btn_r is 4
#define total_buttons 6
#define btn_u 5
#define btn_d 10
#define btn_l 11
#define btn_r 4
#define btn_b 14
#define btn_a 15

// LCD pin setting
// For phi-1 shield LCD_D7 is 4. For phi-2 shield LCD_D7 is 3
#define LCD_RS 8
#define LCD_EN 9
#define LCD_D4 7
#define LCD_D5 6
#define LCD_D6 2
#define LCD_D7 3

LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object

// The following lines instantiates a button group to control 6 buttons
byte pins[]={btn_u,btn_d,btn_l,btn_r,btn_b,btn_a}; // The digital pins connected to the 6 buttons.
char mapping[]={'U','D','L','R','B','A'}; // This is a list of names for each button.
phi_button_groups my_btns(mapping, pins, total_buttons);

// This serial keypad is for debugging.
phi_serial_keypads debug_keypad(&Serial,115200); 

// The following sets up function keys for phi_prompt library
char up_keys[]={"U"}; ///< All keys that act as the up key are listed here.
char down_keys[]={"D"}; ///< All keys that act as the down key are listed here.
char left_keys[]={"L"}; ///< All keys that act as the left key are listed here.
char right_keys[]={"R"}; ///< All keys that act as the right key are listed here.
char enter_keys[]={"B"}; ///< All keys that act as the enter key are listed here.
char escape_keys[]={"A"}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.

// The following adds all available keypads as inputs for phi_prompt library
multiple_button_input * keypads[]={&my_btns,&debug_keypad,0};

void setup()
{
  Serial.begin(9600);
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
}

void loop()
{
  char menu1[]="Main menu:\nDisplay GPS info\nRecord GPS info \nErase data\nDisplay records\nParameters\nShow credit\n";
  char input_buffer[]="40000";
  int ret_val=255;
  int user_input;
  // Displays a long message
  simple_long_msg("This is a sample code for collecting user inputs on phi-2 shield with phi_prompt library.\nAuthor: Dr. Liu");
  
  // Use a long message as a simple menu, if you have a numeric keypad to select an item with a number key. Use serial monitor as a keypad if you want. Just type a number and enter.
  ret_val=simple_long_msg("Main menu:\n1.Display GPS info\n2.Record GPS info \n3.Erase data\n4.Display records\n5.Parameters\n6.Show credit");
  lcd.clear();
  lcd.print("Your choice:");
  lcd.print(ret_val-'0');
  wait_on_escape(2000);
  
  // Use a select list as a more elegant menu. You select it with up/down/enter keys.
  ret_val=simple_select_list(menu1);
  lcd.clear();
  lcd.print("Your choice:");
  lcd.print(ret_val);
  wait_on_escape(2000);

  // Use a simple input panel to collect input. You can use sscanf or else to convert numbers in the string to integers.
  lcd.clear();
  lcd.print("Enter 4-digit:");
  ret_val=simple_input_panel(input_buffer,'0','9',0);
  lcd.clear();
  lcd.print("Your input:");
  lcd.print(input_buffer+1);
  wait_on_escape(2000);
  
  // Use sscanf to turn text input into number, that is if the input is number
  lcd.clear();
  lcd.print("Your value:");
  sscanf(input_buffer+1,"%d",&user_input);
  lcd.print(user_input);
  wait_on_escape(2000);
}
  
