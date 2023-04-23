
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>



//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"

// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
#define Tirgger_Frequency 38
#define LEFT 321
#define RIGHT 123
char Location = 0x01; // start from LED 1 (right most)
#define Rightmost 0x01
#define Leftmost 0x80
struct Timer timer = {FALSE, Tirgger_Frequency};

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();
    LEDS_INIT();

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
    printf("Welcome to zchen287's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);

    int state;

    while (1) {
        //poll timer events and react if any occur

        // if the timer event flag is set
        if (timer.event == TRUE) {
            // if we're at the rightmost LED
            if (Location == Rightmost) {
                // reverse direction
                state = LEFT;
                // trigger next LED
                Location = (Location << 1);
                LEDS_SET(Location);
            }                
            // if we're at the leftmost LED
            else if (Location == Leftmost) {
                // reverse direction
                state = RIGHT;
                // trigger next LED
                Location = (Location >> 1);
                LEDS_SET(Location);
            }                
            // else, not at the last LED 
            else {
                if (state == LEFT) {
                    // trigger next LED
                    Location = (Location << 1);
                    LEDS_SET(Location);
                } else if (state == RIGHT) {
                    // trigger next LED
                    Location = (Location >> 1);
                    LEDS_SET(Location);
                }
            }
            // clear the timer event flag
            timer.event = FALSE;
        }
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    // a bit is 1 if the button is being pressed and 0 if it's not
    // the buttons are ordered
    // get the status of all 4 switches
    uint8_t switchesState = SWITCH_STATES();

    timer.timeRemaining--;
    if (timer.timeRemaining < 0) {
        // min speed 1/38th when all switches are off
        // switchesState=0b0000, so speed=0b0000+1=1
        // max speed 16/38th when all switches are on
        // switchesState=0b1111, so speed=0b1111+1=16
        // use the SWITCH_STATES() macro to determine the appropriate timeRemaining
        timer.timeRemaining = switchesState + 1;
        // generate a Timer event
        timer.event = TRUE;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}