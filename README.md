# xerox_phaser_6600_lcd
USE Xerox Phaser 6600 LCD module with arduino

The LCD & key module of a Xerox Phaser 6600 looks like this
![Screenshot](https://raw.githubusercontent.com/0zoon0/xerox_phaser_6600_lcd/master/images/xerox.jpg)
![Screenshot](https://raw.githubusercontent.com/0zoon0/xerox_phaser_6600_lcd/master/images/xeroxb.jpg)

The ribbon cable on the PCB has pin numbered from 1 to 12. The pinout is as follows:
1 +5v
2 GND
3 D7
4 D6
5 D5
6 D4
7 E
8 GND (most probable RW)
9 RS 
10 ? (Capacitor to the ground & resistor to 5V on the board)
11 ? (Resistor to 5v on the board)
12 GND

Here is an example of a board that can be used to interface with arduino:
![Screenshot](https://raw.githubusercontent.com/0zoon0/xerox_phaser_6600_lcd/master/images/board1.jpg)
![Screenshot](https://raw.githubusercontent.com/0zoon0/xerox_phaser_6600_lcd/master/images/board2.jpg)

The arduino code can be found in xerox_phaser_6600_lcd.ino file.