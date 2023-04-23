// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
uint8_t Button_Event_Flag = FALSE;
static uint8_t Button_Event;

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();
    LEDS_INIT();
    ButtonsInit();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to zchen287's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);

    // if a button event flag is set
    if (Button_Event_Flag == TRUE) {
        // store the current switch positions
        uint8_t switchesState = SWITCH_STATES();

        // If switch 1 is down, then each Button DOWN event toggles the appropriate LEDs, and button up events do nothing
        // If switch 1 is up, then each Button UP event toggles the appropriate LEDs, and button down events do nothing

        // if Switch 1 is on and Button up event
        if ((switchesState & SWITCH_STATE_SW1) && (Button_Event & BUTTON_EVENT_1UP)) {
            char LEDsPattern = LEDS_GET();
            // BTN1 - Toggles LD1 and LD2
            // 0b00000011
            LEDsPattern ^= 0x03;
            LEDS_SET(LEDsPattern);
        }            
        // else if Switch 1 is off and Button down event
        else if (!(switchesState & SWITCH_STATE_SW1) && (Button_Event & BUTTON_EVENT_1DOWN)) {
            char LEDsPattern = LEDS_GET();
            // BTN1 - Toggles LD1 and LD2
            // 0b00000011
            LEDsPattern ^= 0x03;
            LEDS_SET(LEDsPattern);
        }
        // if Switch 2 is on and Button up event
        if ((switchesState & SWITCH_STATE_SW2) && (Button_Event & BUTTON_EVENT_2UP)) {
            char LEDsPattern = LEDS_GET();
            // BTN2 - Toggles LD3 and LD4
            // 0b00001100
            LEDsPattern ^= 0x0C;
            LEDS_SET(LEDsPattern);
        }            
        // else if Switch 2 is off and Button down event
        else if (!(switchesState & SWITCH_STATE_SW2) && (Button_Event & BUTTON_EVENT_2DOWN)) {
            char LEDsPattern = LEDS_GET();
            // BTN2 - Toggles LD3 and LD4
            // 0b00001100
            LEDsPattern ^= 0x0C;
            LEDS_SET(LEDsPattern);
        }
        // if Switch 3 is on and Button up event
        if ((switchesState & SWITCH_STATE_SW3) && (Button_Event & BUTTON_EVENT_3UP)) {
            char LEDsPattern = LEDS_GET();
            // BTN3 - Toggles LD5 and LD6
            // 0b00110000
            LEDsPattern ^= 0x30;
            LEDS_SET(LEDsPattern);
        }            
        // else if Switch 3 is off and Button down event
        else if (!(switchesState & SWITCH_STATE_SW3) && (Button_Event & BUTTON_EVENT_3DOWN)) {
            char LEDsPattern = LEDS_GET();
            // BTN3 - Toggles LD5 and LD6
            // 0b00110000
            LEDsPattern ^= 0x30;
            LEDS_SET(LEDsPattern);
        }
        // if Switch 4 is on and Button up event
        if ((switchesState & SWITCH_STATE_SW4) && (Button_Event & BUTTON_EVENT_4UP)) {
            char LEDsPattern = LEDS_GET();
            // BTN4 - Toggles LD7 and LD8
            // 0b11000000
            LEDsPattern ^= 0xC0;
            LEDS_SET(LEDsPattern);
        }            
        // else if Switch 4 is off and Button down event
        else if (!(switchesState & SWITCH_STATE_SW4) && (Button_Event & BUTTON_EVENT_4DOWN)) {
            char LEDsPattern = LEDS_GET();
            // BTN4 - Toggles LD7 and LD8
            // 0b11000000
            LEDsPattern ^= 0xC0;
            LEDS_SET(LEDsPattern);
        }
    }
    // clear button event flag
    Button_Event_Flag = FALSE;

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    Button_Event = ButtonsCheckEvents();
    if (Button_Event != BUTTON_EVENT_NONE) {
        Button_Event_Flag = TRUE;
    } else {
        Button_Event_Flag = FALSE;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}