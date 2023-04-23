// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

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
#define Tirgger_Frequency_A 2*Tirgger_Frequency
#define Tirgger_Frequency_B 3*Tirgger_Frequency
#define Tirgger_Frequency_C 5*Tirgger_Frequency
struct Timer TimerA = {FALSE, Tirgger_Frequency_A};
struct Timer TimerB = {FALSE, Tirgger_Frequency_B};
struct Timer TimerC = {FALSE, Tirgger_Frequency_C};
#define Toggling_LED_1 0x01 // 0b001
#define Toggling_LED_2 0x02 // 0b010
#define Toggling_LED_3 0x04 // 0b100

// **** Declare function prototypes ****
void LEDS_INIT(void);
void LEDS_SET(char newPattern);
char LEDS_GET(void);

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
    printf("Welcome to zchen287's lab6 part1 (timers).  Compiled on %s %s.\n", __TIME__, __DATE__);

    while (1) {
        //poll timer A
        //react to timer A events
        //clear timer A event flag

        // if timerA event flag is set
        if (TimerA.event == TRUE) {
            // clear timerA event flag
            TimerA.event = FALSE;
            // print A
            printf("A");
            // toggle LED 1
            char LEDsPattern = LEDS_GET();
            LEDsPattern ^= Toggling_LED_1;
            LEDS_SET(LEDsPattern);
        }
        // if timerB event flag is set
        if (TimerB.event == TRUE) {
            // clear timerB event flag
            TimerB.event = FALSE;
            // print B
            printf("B");
            // toggle LED 2
            char LEDsPattern = LEDS_GET();
            LEDsPattern ^= Toggling_LED_2;
            LEDS_SET(LEDsPattern);
        }
        // if timerC event flag is set
        if (TimerC.event == TRUE) {
            // clear timerC event flag
            TimerC.event = FALSE;
            // print C
            printf("C");
            // toggle LED 3
            char LEDsPattern = LEDS_GET();
            LEDsPattern ^= Toggling_LED_3;
            LEDS_SET(LEDsPattern);
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

    //update timerA
    //if timerA has counted down,
    //generate timerA event
    //reset timerA

    // TimerA
    // Decrement timerA?s timeRemaining
    --TimerA.timeRemaining;
    // If TimerA?s timeRemaining is 0
    if (TimerA.timeRemaining == 0) {
        // Set TimerA?s event flag
        TimerA.event = TRUE;
        // Reset TimerA?s timeRemaining
        TimerA.timeRemaining = Tirgger_Frequency_A;
    }
    // TimerB
    // Decrement timerB?s timeRemaining
    --TimerB.timeRemaining;
    // If TimerB?s timeRemaining is 0
    if (TimerB.timeRemaining == 0) {
        // Set TimerB?s event flag
        TimerB.event = TRUE;
        // Reset TimerB?s timeRemaining
        TimerB.timeRemaining = Tirgger_Frequency_B;
    }
    // TimerC
    // Decrement timerC?s timeRemaining
    --TimerC.timeRemaining;
    // If TimerC?s timeRemaining is 0
    if (TimerC.timeRemaining == 0) {
        // Set TimerB?s event flag
        TimerC.event = TRUE;
        // Reset TimerB?s timeRemaining
        TimerC.timeRemaining = Tirgger_Frequency_C;
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}