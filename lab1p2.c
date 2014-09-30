// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //
// Defines to simply UART's baud rate generator (BRG) regiser
// given the osicllator freqeuncy and PLLMODE.

#define XTFREQ          7372800         	  // On-board Crystal frequency
#define PLLMODE         4               	  // On-chip PLL setting (Fosc)
#define FCY             (XTFREQ*PLLMODE)/2    // Instruction Cycle Frequency (Fosc/2)

#define BAUDRATE         115200
#define BRGVAL          ((FCY/BAUDRATE)/16)-1

// ******************************************************************************************* //

// Variable utilized to store the count that is incremented within the Timer 1 interrupt
// service routine every second.
// Notes:
//        1. This variable is declared are volatile becuase it is updated in the interrupt
//           service routine but will be read in the main execution loop.
//        2. Declared as unsigned char as the varaible will only store the values between
//           0 and 10.
volatile unsigned char cnt;
unsigned char command;

volatile int m; // minutes
volatile int s; // seconds
volatile int f; // 1/100th of a second

enum State {RUN, STOP, RESET} state;
// ******************************************************************************************* //

/*****************************************************/


int main(void)
{

        RPOR4bits.RP8R = 3;
        U1BRG  = BRGVAL;
        U1MODE = 0x8000;


	// printf by default is mapped to serial communication using UART1.
	// NOTES:
	//        1. You must specify a heap size for printf. This is required
	//           becuase printf needs to allocate its own memory, which is
	//           allocated on the heap. This can be set in MPLAB by:
	//           a.) Selecting Build Options...->Project from the Project menu.
	//           b.) Selecting the MPLABLINK30 Tab.
	//           c.) Entering the size of heap, e.g. 512, under Heap Size
	//        2. printf function is advanced and using printf may require
	//           significant code size (6KB-10KB).
	printf("Lab 2: Debugging Statements\n\r");

        TMR1 = 0;
        // Calculate PR1 value for a 10ms delay
        //
        // 14745600/8 = 1843200 (prescaler of 8)
        //1843200/100 = 18432 (multiplying by 1ms to get PR1 value)

        PR1 = 18432;

        // Set TCON Register
        //
        //TON = 0
        //TCKPS1:TCKPS2 = 01 (set timer prescaler to 1:8)
        //TCS = 0 (Fosc/2)

        T1CON = 0x0010;

        m = 0; //minutes
        s = 0; //seconds
        f = 0; //1/100th seconds

	// The following code will not work until you have implemented the
	// the required LCD functions defined within lcd.c
	LCDInitialize();
//        LCDClear();
//        printf("clear");
/*******************************/
//        TRISBbits.TRISB15 = 0;

        LCDPrintString("Hello");
        DelayUs(1600);
        state = STOP;

        //WriteLCD(0x0F, 0, 40);
        int i = 0;
        int num = 0;
	while(1)
	{
            if(state==STOP){
                LCDClear();
                LCDMoveCursor(0,2);
                LCDPrintString("STOP");
                LCDMoveCursor(1,0);
                LCDPrintString("00:00:00");

            }
            else if (state == RUN){

            }
            else if (state == RESET){

            }
            i = 0;
            LCDMoveCursor(1,0);
            while (i < 6) {
                switch (i){
                    case 1:
                        num = (m\10)%10;
                        break;
                    case 2:
                        num = m%10;
                        break;
                    case 3:
                        LCDPrintChar(':');
                        num = (s/10)%10;
                        break;
                    case 4:
                        num = s%10;
                        break;
                    case 5:
                        LCDPrintChar(':');
                        num = (f/10)%10;
                        break;
                    case 6:
                        num = f%10;
                        break;
                }
                switch (num)
                {
                    case 1:
                        LCDPrintChar('1');
                        break;
                    case 2:
                        LCDPrintChar('2');
                        break;
                    case 3:
                        LCDPrintChar('3');
                        break;
                    case 4:
                        LCDPrintChar('4');
                        break;
                    case 5:
                        LCDPrintChar('5');
                        break;
                    case 6:
                        LCDPrintChar('6');
                        break;
                    case 7:
                        LCDPrintChar('7');
                        break;
                    case 8:
                        LCDPrintChar('8');
                        break;
                    case 9:
                        LCDPrintChar('9');
                        break;
                    case 0:
                        LCDPrintChar('0');
                        break;
                }
                i++;
            }
            
//            WriteLCD(0x41, 1, 46);
	}
	return 0;
}

// ******************************************************************************************* //
// Defines an interrupt service routine that will execute whenever Timer 1's
// count reaches the specfied period value defined within the PR1 register.
//
//     _ISR and _ISRFAST are macros for specifying interrupts that
//     automatically inserts the proper interrupt into the interrupt vector
//     table
//
//     _T1Interrupt is a macro for specifying the interrupt for Timer 1
//
// The functionality defined in an interrupt should be a minimal as possible
// to ensure additional interrupts can be processed.

void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
//void _ISR _T1Interrupt(void)
{
	// Clear Timer 1 interrupt flag to allow another Timer 1 interrupt to occur.
	IFS0bits.T1IF = 0;

	// Updates cnt to wraparound from 9 to 0 for this demo.
	cnt = (cnt<9)?(cnt+1):0;

        if (f <= 99) {
            f++;
        }
        else if (s <= 59){
            f = 0;
            s++;
        }
        else {
            f = 0;
            s = 0;
            m++;
        }

/*******************************/
	//make the LCD blink;
////	command ^= 0x4;
////	WriteLCD(command, 0, 40);
/*******************************/
}
//void _ISR _T1Interrupt(void)
//{
// IFS0bits.T1IF = 0;
//}
// ******************************************************************************************* //
