// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "OledDriver.h"
#include "Oled.h"
#include "Ascii.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"
#include "Leds_Lab06.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

struct AdcResult {
    uint8_t event;
    int16_t voltage;
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

#define LEFT 321
#define RIGHT 123
char Location = 0x01; // start from LED 1 (right most)
#define Rightmost 0x01
#define Leftmost 0x80
struct Timer timer = {FALSE, Tirgger_Frequency};

#define Initial_Raw_Value 0 // start from 0
#define Window_Size 5
struct AdcResult Adc = {FALSE, Initial_Raw_Value};
int16_t Max_Raw_Value = 1023;
int16_t New_Raw_Value;
int16_t Raw_Value;
int16_t Old_Raw_Value;
int16_t Percentage;
char OLED[60];

uint8_t Button_Event_Flag = FALSE;
static uint8_t Button_Event;
char LEDsPattern = 0;

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();
    OledInit();
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


    // Enable interrupts for the ADC
    IPC6bits.AD1IP = 2;
    IPC6bits.AD1IS = 0;
    IEC1bits.AD1IE = 1;

    // Set B2 to an input so AN2 can be used by the ADC.
    TRISBbits.TRISB2 = 1;

    // Configure and start the ADC
    AD1CHSbits.CH0SA = 2; // add B2 to the mux
    AD1PCFGbits.PCFG2 = 0; // add b2 to the ADC
    AD1CSSLbits.CSSL2 = 1; // and add b2 to the scanner

    AD1CON1 = 0; // start with 0
    AD1CON1bits.SSRC = 0b111; // auto conversion
    AD1CON1bits.ASAM = 1; // auto sample

    AD1CON2 = 0; // start with 0
    AD1CON2bits.SMPI = 7; // one interrupt per 8 samples

    AD1CON3 = 0; // start with 0
    AD1CON3bits.SAMC = 29; // long sample time
    AD1CON3bits.ADCS = 50; // long conversion time

    AD1CON1bits.ADON = 1; // and finally turn it on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to CRUZID's lab6 part6 (Extra Credit).  Compiled on %s %s.\n", __TIME__, __DATE__);

    // if the ADC event flag is set
    if (Adc.event == TRUE) {
        // update the ADC raw value
        New_Raw_Value = Adc.voltage;
        // update the ADC percentage
        Percentage = (Adc.voltage * 100) / Max_Raw_Value;
        // update the OLED 
        sprintf(OLED, "ADC raw value: %d \nADC percentage: %d", New_Raw_Value, Percentage);
        OledDrawString(OLED);
        OledUpdate();
        // clear the ADC event flag
        Adc.event = FALSE;
    }

    int state;
    // if the timer event flag is set
    if (timer.event == TRUE || Button_Event_Flag == TRUE) {
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
            if (state = LEFT) {
                // trigger next LED
                Location = (Location << 1);
                LEDS_SET(Location);
            } else if (state = RIGHT) {
                // trigger next LED
                Location = (Location >> 1);
                LEDS_SET(Location);
            }
        }
        // clear the timer event flag
        timer.event = FALSE;

        // if a button event flag is set
        if (Button_Event_Flag == TRUE) {
            // store the current switch positions
            uint8_t switchesState = SWITCH_STATES();

            // If switch 1 is down, then each Button DOWN event toggles the appropriate LEDs, and button up events do nothing
            // If switch 1 is up, then each Button UP event toggles the appropriate LEDs, and button down events do nothing

            // if Switch 1 is on and Button up event
            if ((switchesState & SWITCH_STATE_SW1) && (Button_Event & BUTTON_EVENT_1UP)) {
                // BTN1 - Toggles LD1 and LD2
                // 0b00000011
                LEDsPattern ^= 0x03;
            }                
            // else if Switch 1 is off and Button down event
            else if (!(switchesState & SWITCH_STATE_SW1) && (Button_Event & BUTTON_EVENT_1DOWN)) {
                // BTN1 - Toggles LD1 and LD2
                // 0b00000011
                LEDsPattern ^= 0x03;
            }
            // if Switch 2 is on and Button up event
            if ((switchesState & SWITCH_STATE_SW2) && (Button_Event & BUTTON_EVENT_2UP)) {
                // BTN2 - Toggles LD3 and LD4
                // 0b00001100
                LEDsPattern ^= 0x0C;
            }                
            // else if Switch 2 is off and Button down event
            else if (!(switchesState & SWITCH_STATE_SW2) && (Button_Event & BUTTON_EVENT_2DOWN)) {
                // BTN2 - Toggles LD3 and LD4
                // 0b00001100
                LEDsPattern ^= 0x0C;
            }
            // if Switch 3 is on and Button up event
            if ((switchesState & SWITCH_STATE_SW3) && (Button_Event & BUTTON_EVENT_3UP)) {
                // BTN3 - Toggles LD5 and LD6
                // 0b00110000
                LEDsPattern ^= 0x30;
            }                
            // else if Switch 3 is off and Button down event
            else if (!(switchesState & SWITCH_STATE_SW3) && (Button_Event & BUTTON_EVENT_3DOWN)) {
                // BTN3 - Toggles LD5 and LD6
                // 0b00110000
                LEDsPattern ^= 0x30;
            }
            // if Switch 4 is on and Button up event
            if ((switchesState & SWITCH_STATE_SW4) && (Button_Event & BUTTON_EVENT_4UP)) {
                // BTN4 - Toggles LD7 and LD8
                // 0b11000000
                LEDsPattern ^= 0xC0;
            }                
            // else if Switch 4 is off and Button down event
            else if (!(switchesState & SWITCH_STATE_SW4) && (Button_Event & BUTTON_EVENT_4DOWN)) {
                // BTN4 - Toggles LD7 and LD8
                // 0b11000000
                LEDsPattern ^= 0xC0;
            }
            // clear button event flag
            Button_Event_Flag = FALSE;
        }
        LEDS_SET(Location | LEDsPattern);
    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void) {
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

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

    Button_Event = ButtonsCheckEvents();
    if (Button_Event != BUTTON_EVENT_NONE) {
        Button_Event_Flag = TRUE;
    } else {
        Button_Event_Flag = FALSE;
    }

}

/**
 * This is the ISR for the ADC1 peripheral.
 */
void __ISR(_ADC_VECTOR, IPL2AUTO) AdcHandler(void) {
    // Clear the interrupt flag.
    IFS1bits.AD1IF = 0;

    // Read the 8 buffered ADC values in the ADC1BUF0 through ADC1BUF7 SFR?s and average all of them
    Raw_Value = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7) / 8;
    // If the ADC reading exits the window
    if (abs(Raw_Value - Old_Raw_Value) > Window_Size) {
        // generate an ADC event
        Adc.event = TRUE;
        // update the window center (voltage)
        Adc.voltage = Raw_Value;
        Old_Raw_Value = Raw_Value;
    }

}