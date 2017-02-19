// Enter a number with phi_prompt for phi-panel backpacks:
/*
.__   __.  __    __  .___  ___. .______    _______ .______      
|  \ |  | |  |  |  | |   \/   | |   _  \  |   ____||   _  \     
|   \|  | |  |  |  | |  \  /  | |  |_)  | |  |__   |  |_)  |    
|  . `  | |  |  |  | |  |\/|  | |   _  <  |   __|  |      /     
|  |\   | |  `--'  | |  |  |  | |  |_)  | |  |____ |  |\  \----.
|__| \__|  \______/  |__|  |__| |______/  |_______|| _| `._____|

http://liudr.wordpress.com
*/

#define lcd_rows 2
#define lcd_columns 16

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

void setup()
{
  Serial.begin(9600);
  lcd.begin(lcd_columns, lcd_rows);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
}

void loop()
{
  long result=0;
  phi_prompt_struct myTextInput; // This is the structure you will use to invoke library function input_panel.

  char nmuber_buffer[]="            "; // This is the buffer that will store the content of the text panel. 
  myTextInput.ptr.msg=nmuber_buffer; // Assign the text buffer address
  myTextInput.width=12; // Length of the input panel is 12 characters.
  myTextInput.col=2; // Display input panel at column 2
  myTextInput.row=1; // Display input panel at row 1
  lcd.clear(); // Clear the lcd
  lcd.print("Enter number:"); // Prompt user for input

  if (input_number(&myTextInput)!=-1) // input_panel stores user choice in file_name.
  {  
    lcd.clear();
    lcd.print("You entered:"); // Display the user choice one more time
    lcd.setCursor(0,1); // Display the list at column 2 row 1.
    if (sscanf(nmuber_buffer,"%ld",&result)==1) lcd.print(result); ///< Turn the text entered into integer number. If a valid input is acquired, sscanf returns 1.
    else lcd.print("Invalid input!");
  }
  else 
  {
    lcd.clear();
    lcd.print("You pressed ESC");
  }
  wait_on_escape(4000);
}
  
