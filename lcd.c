// ******************************************************************************************* //

#include "p24fj64ga002.h"

// ******************************************************************************************* //

// LCD Data connected to RB15 -> RB12, RS is connected to RB7, and E is connected to RB6
#define LCD_D   LATB
#define LCD_RS  LATBbits.LATB7
#define LCD_E   LATBbits.LATB6

// ******************************************************************************************* //

// TRIS register mappings for LCD signals
#define LCD_TRIS_D7  TRISBbits.TRISB15
#define LCD_TRIS_D6  TRISBbits.TRISB14
#define LCD_TRIS_D5  TRISBbits.TRISB13
#define LCD_TRIS_D4  TRISBbits.TRISB12
#define LCD_TRIS_RS  TRISBbits.TRISB7
#define LCD_TRIS_E   TRISBbits.TRISB6

// ******************************************************************************************* //

// Defines LCD write types for EnableLCD function that will be assigned to RS LCD input
// used to specific writing control instructions or data characters.
#define LCD_WRITE_DATA    1
#define LCD_WRITE_CONTROL 0

// ******************************************************************************************* //

// TODO: This function will use a 16-bit timer (Timer 3) to wait for approximately 0 to the 16000 us
// specfied by the input usDelay.
//
// Function Inputs:
//    unsigned usDelay     : Specifies the requested delay in microsecond

void DelayUs(unsigned int usDelay) {

    
        // Set Timer 3's period value regsiter to value for 250ms. Please note
	// T1CON's register settings below (internal Fosc/2 and 1:256 prescalar).
	//
	//    Fosc     = XTFREQ * PLLMODE
	//             = 7372800 * 4
	//             = 29491200
	//
	//    Fosc/2   = 29491200 / 2
	//             = 14745600
	//
	//    Timer 2 Freq = (Fosc/2) / Prescaler
	//                 = 14745600 /8
	//                 = 1843200
	//
	//    PR1 = 1 us / (1 / (T1 Freq))
	//        = 1e-6/ (1 / 1843200)
	//        = 1e-6 * 1843200
	//        = 1.84320

    PR3 = 1.8432*usDelay;

        IFS0bits.T3IF = 0;
        IEC0bits.T3IE = 1;
        // Setup Timer 3 control register (T3CON) to:
 	//     TON           = 1     (start timer)
	//     TCKPS1:TCKPS2 = 01    (set timer prescaler to 1:8)
	//     TCS           = 0     (Fosc/2)
    T3CON = 0x8010;


    while(IFS0bits.T3IF == 0);

/*****************************************************/
}

void _ISR _T3Interupt(){
    IFS0bits.T3IF = 0;
    T3CONbits.TON = 0;
}

// ******************************************************************************************* //

// EnableLCD function enables writing control instructions or data (ASCII characters) to the
// LCD, controlling the LCD_RS and LCD_E signals.
//
// If the commandType is 0 (LCD_WRITE_CONTROL), the 8-bit control instructions must be
// asserted using the D7-D4 inputs. The 8-bit control instructions must be written
// to the LCD using the D7-D4 inputs as two 4-bit writes with the most significant 4-bits
// (nibble) written first. This requires two calls to the EnableLCD() function.
//
// If the commandType is 1 (LCD_WRITE_DATA), the 8-bit ASCII characters written to the LCD
// will be displayed by the LCD at the current cursor location. The 8-bit characters must be
// written to the LCD using the D7-D4 inputs as two 4-bit writes with the most significant
// 4-bits (nibble) written first. This requires two calls to the EnableLCD() function.

void EnableLCD(unsigned char commandType, unsigned usDelay) {

	LCD_RS = commandType; DelayUs(usDelay);
	LCD_E = 1;  DelayUs(usDelay);
	LCD_E = 0;  DelayUs(usDelay);
}

// ******************************************************************************************* //

// TODO: WriteLCD function should write either control instruciton of data characters to the
// LCD display using the 4-bit interface. This function will handled the bit masking and shifting
// to write the individual 4-bit nibbles to the LCD data inputs. Control instructions
// are written in the commandType is 0 (LCD_WRITE_CONTROL), and ASCII characters are written
// if the commandType is 1 (LCD_WRITE_DATA).
//
// Function Inputs:
//    unsigned char word   : 8-bit control instruction of data character to be written
//    unsigned commandType : Specifies the type of write operation, see above
//    unsigned usDelay     : Specifies the write delay required for this operation

void WriteLCD(unsigned char word, unsigned commandType, unsigned usDelay) {

	// TODO: Using bit masking and shift operations, write most significant bits to correct
	// bits of the LCD_D signal (i.e. #define used to map internal name to LATB)
	// and enable the LCD for the correct command.

	// TODO: Using bit masking and shift operations, write least significant bits to correct
	// bits of the LCD_D signal (i.e. #define used to map internal name to LATB)
	// and enable the LCD for the correct command.
}

// ******************************************************************************************* //

// The initialization routine is utulized to start the LCD operation and configure the
// LCD interface to utilize 4-bit interface. Please reference the data sheet for the LCD
// for further details.

void LCDInitialize(void) {

	// Setup D, RS, and E to be outputs (0).
    
    LCD_D = 0; LCD_RS = 0; LCD_E = 0;

	// Initilization sequence utilizes specific LCD commands before the general configuration
	// commands can be utilized. The first few initilition commands cannot be done using the
	// WriteLCD function. Additionally, the specific sequence and timing is very important.

    int i = 0;
    while (i<10){
    DelayUs(16000);
    i++;
    }
	// Enable 4-bit interface
    LCD_TRIS_E = 1;

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 1;
    LCD_TRIS_D4 = 1;DelayUs(4100)

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 1;
    LCD_TRIS_D4 = 1;DelayUs(100);

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;
    
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 1;
    LCD_TRIS_D4 = 1;

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 1;
    LCD_TRIS_D4 = 0; DelayUs();
  //Function set
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 1;
    LCD_TRIS_D4 = 0;DelayUs(4100);

  //Display control
  //display off
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);
    
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 1;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);
    
  //Display Clear
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);
    
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 1;DelayUs(4100);
    
  //Entry Mode Set
  //increment display, no shift (ie cursor move)
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);
    
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 1;
    LCD_TRIS_D5 = 1;
    LCD_TRIS_D4 = 0;DelayUs(4100);

  //Display control
  //display on, cursor off, blink off
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);

    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 1;
    LCD_TRIS_D6 = 1;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);
    
  //end initialization
    LCD_TRIS_RS = 0;
    LCD_TRIS_D7 = 0;
    LCD_TRIS_D6 = 0;
    LCD_TRIS_D5 = 0;
    LCD_TRIS_D4 = 0;DelayUs(4100);
}

// ******************************************************************************************* //

// TODO: LCDClear sends a clear screen command to the LCD tht will both clear the screen
// and return the cursor to the origin (0,0).

void LCDClear(void) {

	// TODO: Write the proper control instruction to clear the screen ensuring
	// the proper delay is utilized.
}

// ******************************************************************************************* //

// TODO: LCDMoveCursor should move to the cursor to the specified (x,y) coordinate. Note that
// as the LCD controller is a generic interface for many LCD displays or varying size,
// some (x,y) cooridnates may not be visible by the LCD display.
//
// Function Inputs:
//    unsigned char x : coordinate for LCD row (0 or 1)
//    unsigned char y : coordinate for LCD column (0 to 7)

void LCDMoveCursor(unsigned char x, unsigned char y) {

	// TODO: Write the propoer control instruction to move the cursor to the specified
	// (x,y) coordinate. This operation should be performance as a single control
	// control instruction, i.e. a single call the WriteLCD() function.

}

// ******************************************************************************************* //

// TODO: LCDPrintChar should print a single ASCII character to the LCD diplay at the
// current cursor position.
//
// Function Inputs:
//    char c : ASCII character to write to LCD

void LCDPrintChar(char c) {

	// TODO: Write the ASCII character provide as input to the LCD display ensuring
	// the proper delay is utilized.

}

// ******************************************************************************************* //

// LCDPrintString will print a null-terminated string to the LCD at the current cursor
// location.
//
// Notes:
//       1. If the string is longer than the length of a row, the entire string may not
//          appear on the LCD.
//       2. If the string is not null-terminated, random data within the memory pointed to
//          by the pointer will be displayed. This process will only terminate when a null
//          characters if found.

void LCDPrintString(const char* s) {

}

// ******************************************************************************************* //
