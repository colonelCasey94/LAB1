// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
// This option can be set by selecting "Configuration Bits..." under the Configure
// menu in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
// This option can be set by selecting "Configuration Bits..." under the Configure
// menu in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

void DebounceDelay();

int main(void)
{
	// ****************************************************************************** //

	// TODO: Configure AD1PCFG register for configuring input pins between analog input
	// and digital IO.
    AD1PCFGbits.PCFG4 = 1;
    AD1PCFGbits.PCFG5 = 1;
	// TODO: Configure TRIS register bits for Right and Left LED outputs.
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;

	// TODO: Configure LAT register bits to initialize Right LED to on.
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;

	// TODO: Configure ODC register bits to use open drain configuration for Right
	// and Left LED output.
    ODCBbits.ODB2 = 1;
    ODCBbits.ODB3 = 1;

	// TODO: Configure TRIS register bits for swtich input.
    TRISBbits.TRISB5 = 1;

	// TODO: Configure CNPU register bits to enable internal pullup resistor for switch
	// input.
    CNPU2bits.CN27PUE = 1;

	// TODO: Setup Timer 1 to use internal clock (Fosc/2).
	// TODO: Setup Timer 1's prescaler to 1:256.
 	// TODO: Set Timer 1 to be initially off.
    T1CON = 0x0030;
	// TODO: Clear Timer 1 value and reset interrupt flag
    TMR1 = 0;
    IFS0bits.T1IF = 0;

IEC0bits.T1IE = 1;
	// TODO: Set Timer 1's period value register to value for 5 ms.
        //    14745600/256 = 57600
        //    57600*.005 = 288
        //    288-1=287
    PR1 = 287;

    int state = 0;
	while(1)
	{
           
            if(PORTBbits.RB5 == 0)
            {
                if (state == 0){
                    LATBbits.LATB2 = 1;
                    LATBbits.LATB3 = 0;
                    state = 1;
                }
                else {
                    LATBbits.LATB2 = 0;
                    LATBbits.LATB3 = 1;
                    state = 0;
                }
            
                DebounceDelay();
            }
		// TODO: For each distinct button press, alternate which
		// LED is illuminated (on).

		// TODO: Use DebounceDelay() function to debounce button press
		// and button release in software.
	}
	return 0;
}

void DebounceDelay(){
    T1CONbits.TON = 1;
    TMR1 = 0;
    while (IFS0bits.T1IF == 0 && PORTBbits.RB5 == 0);
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
}

// *******************************************************************************************