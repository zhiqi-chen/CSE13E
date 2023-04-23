// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event;
    int16_t voltage;
};

// **** Define global, module-level, or external variables here ****
#define Initial_Raw_Value 0 // start from 0
#define Window_Size 5
struct AdcResult Adc = {FALSE, Initial_Raw_Value};
int16_t Max_Raw_Value = 1023;
int16_t New_Raw_Value;
int16_t Raw_Value;
int16_t Old_Raw_Value;
int16_t Percentage;
char OLED[60];

// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();
    LEDS_INIT();
    OledInit();

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
    printf("Welcome to zchen287's lab6 part3 (bounce_adc).  Compiled on %s %s.\n", __TIME__, __DATE__);
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

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the ADC1 peripheral. It will trigger whenever a new ADC reading is available
 * in the ADC buffer SFRs, ADC1BUF0 - ADC1BUF7.
 *
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_ADC_VECTOR, ipl2auto) AdcHandler(void) {
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