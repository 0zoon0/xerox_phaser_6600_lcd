
/******************************* Program Notes ******************************
This code is compatible with LCD modules used in Xerox Phaser 6600 printers.
It is based on Donald Weiman's work here
http://web.alfredstate.edu/faculty/weimandn/programming/lcd/ATmega328/LCD_code_ino_4d.html

The initialization routine is adapted to work correctly with Xerox Phaser 6600 LCD modules.
The initialization routine shows the commands & data & timings as Xerox is sending to this LCD.
It turns out that not all commands are mandatory for successful LCD initialization. Such commands
are commented out. Same might be true for timings, but code uses original timings.
 
 
                 -----------                   ----------
                |  Arduino  |                 |   LCD    |  MCU PIN
                |           |                 |          |
                |           |6  ------------->|D7        |   20
                |           |7  ------------->|D6        |   19
                |           |8  ------------->|D5        |   18
                |           |9  ------------->|D4        |   16
                |           |2  ------------->|E         |   22
                |           |         GND --->|RW        |
                |           |4  ------------->|RS        |   21
                 -----------                   ----------
The ribon cable pads on the PCB are numbered from 1 to 12. Their meaning are:
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
  **************************************************************************/
uint8_t  lcd_D7_ArdPin   =  6;                
uint8_t  lcd_D6_ArdPin   =  7;
uint8_t  lcd_D5_ArdPin   =  8;
uint8_t  lcd_D4_ArdPin   =  9;
uint8_t  lcd_E_ArdPin    =  2;                  // lcd Enable pin
uint8_t  lcd_RS_ArdPin   =  4;                  // lcd Register Select pin

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

#define lcd_Unk1Fun8bit     0b00101001          // Unknown mandatory function Xerox is using   
#define lcd_Unk2Fun8bit     0b01101010          // Unknown mandatory function Xerox is using  

#define index 1 //index of the first character to display, just for demonstration purpose

// Program ID
uint8_t program_author[]   = "Donald Weiman";
uint8_t program_version[]  = "LCD_AVR_4d(ino)";


/******************************* Main Program Code *************************/

void setup (void)
{
// configure the microprocessor pins for the data lines
    pinMode(lcd_D7_ArdPin, OUTPUT);                 
    pinMode(lcd_D6_ArdPin, OUTPUT);
    pinMode(lcd_D5_ArdPin, OUTPUT);
    pinMode(lcd_D4_ArdPin, OUTPUT);

// configure the microprocessor pins for the control lines
    pinMode(lcd_E_ArdPin, OUTPUT);                  // E line - output
    pinMode(lcd_RS_ArdPin, OUTPUT);                 // RS line - output

// initialize the LCD controller as determined by the defines (LCD instructions)
    lcd_init_4d();    

// display the first line of information
    lcd_write_string_4d(program_author);

// set cursor to start of second line
    lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo | index);
// display the second line of information
    lcd_write_string_4d(program_version);
}

// endless loop
void loop()
{  
}
/******************************* End of Main Program Code ******************/

/*============================== 4-bit LCD Functions ======================*/
/*
  Name:     lcd_init_4d
  Purpose:  initialize the LCD module for a 8-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_init_4d(void)
{
// Power-up delay
    delay(100);                                     // initial 40 mSec delay

 // IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive  
//   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.
//
// Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to 
//   the microprocessor and the lower four data lines are typically left open.  Therefore, when 
//   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper 
//   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).
//
// Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so  
//   this instruction can be sent on just the four available data lines and it will be interpreted 
//   properly by the LCD controller.  The 'lcd_write_4' subroutine will accomplish this if the 
//   control lines have previously been configured properly.

// Set up the RS and E lines for the 'lcd_write_4' subroutine.
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    
    lcd_write_4(lcd_FunctionReset);                 
    delayMicroseconds(5100);       
                                 
    lcd_write_4(lcd_FunctionReset);                 
    delayMicroseconds(5100);                                    

    lcd_write_4(lcd_FunctionReset);                 
    delayMicroseconds(5100);  

    lcd_write_4(lcd_FunctionSet4bit);   //0b00100000 original value the printer is sending               
    delayMicroseconds(5100);  
    
    lcd_write_instruction_4d(lcd_Unk1Fun8bit);                 
    delayMicroseconds(5000); 

//    lcd_write_instruction_4d(0b11010100);                 
//    delayMicroseconds(5000);

//    lcd_write_instruction_4d(0b01111000);                 
//    delayMicroseconds(5000);

//    lcd_write_instruction_4d(0b11010010);                 
//    delayMicroseconds(5000);

    lcd_write_instruction_4d(lcd_Unk2Fun8bit);                 
    delayMicroseconds(5000);

    delay(200);
//    lcd_write_instruction_4d(lcd_FunctionSet4bit);     
//    delayMicroseconds(5000);

//    lcd_write_instruction_4d(lcd_DisplayOff);     
//    delayMicroseconds(5000);

//    lcd_write_instruction_4d(0b10000001);     
//    delayMicroseconds(5000);

//    lcd_write_instruction_4d(0b00010110);     
//    delayMicroseconds(5000);

    lcd_write_instruction_4d(lcd_DisplayOn);     
    delayMicroseconds(5000); 
}

/*...........................................................................
  Name:     lcd_write_string_4d
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_write_string_4d(uint8_t theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
    }
}

/*...........................................................................
  Name:     lcd_write_character_4d
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_write_character_4d(uint8_t  theData)
{
    digitalWrite(lcd_RS_ArdPin, HIGH);              // select the Data Register (RS high)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    lcd_write_4(theData);                           // write the upper 4-bits of the data
    lcd_write_4(theData << 4);                      // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_instruction_4d
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_write_instruction_4d(uint8_t  theInstruction)
{
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    lcd_write_4(theInstruction);                    // write the upper 4-bits of the data
    lcd_write_4(theInstruction << 4);               // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a nibble of information to the LCD module
  Entry:    (theByte) contains the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    the desired information (4-bits) must be in the upper half of (theByte)
            use either time delays
*/
void lcd_write_4(uint8_t  theByte)
{
    digitalWrite(lcd_D7_ArdPin, theByte & 1<<7);          
    digitalWrite(lcd_D6_ArdPin, theByte & 1<<6); 
    digitalWrite(lcd_D5_ArdPin, theByte & 1<<5);
    digitalWrite(lcd_D4_ArdPin, theByte & 1<<4);

    delayMicroseconds(45);                           //50uS delay
                                                    
    digitalWrite(lcd_E_ArdPin, HIGH);                // Enable pin high
    delayMicroseconds(10);                           
    digitalWrite(lcd_E_ArdPin, LOW);                 // Enable pin low
    delayMicroseconds(45);                         
}

