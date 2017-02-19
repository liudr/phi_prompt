# phi_prompt
Phi_prompt interactive library developed by Dr. Liu GNU GPL V3.0
<br>This version is specifically made to support I2C character LCDs, while still maintaining support on regular HD44780 LCDs. 
<br>This library was the first interactive library that renders menus, does text entries etc. using character LCD and buttons or other input devices. It has been around since around 2010 and has been updated many times.
<br>
<br>It requires another library, phi_interfaces, to sense buttons, rotary encoders, keypads etc.
<br>To download this library, go to this webpage:
<br>https://github.com/liudr/phi_interfaces

<br>It requires another library, NewliquidCrystal, to interact with various types of character LCDs, regular HD44780 with parallel interfaces, HD44780 LCDs with I2C backpacks, etc.
<br>To download this library, go to this webpage:
<br>https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
<br>
<br>This library was developed also to support the Arduino shield Phi-1 shield, which evolved intp Phi-2 shield, and now Phi-3 shield. To find information of the latested supported hardware, visit this webpage:
<br>https://liudr.wordpress.com/shields/phi-3-shield/
<br>
<br>Currently this library can't be automatically added to your Arduino IDE using the library manager. I will make effort to get it to work with library manager.
<br>To download. click the DOWNLOADS button to the right, and rename the uncompressed folder phi_prompt.
<br>
<br>Place the phi_prompt folder in your arduino sketchfolder/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.