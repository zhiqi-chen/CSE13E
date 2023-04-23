// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CSE13E Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"


// **** Set any macros or preprocessor directives here ****


// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING, ALERT
} OvenState;

// make your own struct for the input selector (same as above)
// eum for time and eum for temperature

typedef enum {
    TIME, TEMPERATURE
} InputSelector;

typedef enum {
    BAKE, TOAST, BROIL
} CookingMode;

typedef enum {
    INVERTED, NORMAL
} AlertState;

typedef struct {
    OvenState state;
    //add more members to this struct 

    // add a cooktime variable
    uint16_t cooktime;
    // add a cooktime remaining variable
    uint16_t cooktime_remain;
    // add a temperature variable
    uint16_t temperature;

    InputSelector selector;
    CookingMode mode;
    AlertState alert;

} OvenData;

// **** Declare any datatypes here ****
OvenData ovenData = {SETUP, 0, 0, 300, TIME, BAKE, NORMAL};

// **** Define any module-level, global, or external variables here ****
static uint16_t freeRunningTimer = 0;
static uint16_t TimerTickEvent = FALSE;

static uint8_t buttonEvents;
static uint8_t buttonEvents_Flag;
static uint8_t adcEvent;
static uint8_t adcEvent_Flag;
static uint16_t adcReading;

#define LONG_PRESS 5

static uint16_t StartTime;
static uint16_t ElapsedTime;
static uint16_t TimerTick;

static char LEDs;
#define LEDs_ON 0xFF
#define LEDs_OFF 0x00

#define OVEN_TOP_ON_5 OVEN_TOP_ON OVEN_TOP_ON OVEN_TOP_ON OVEN_TOP_ON OVEN_TOP_ON
#define OVEN_TOP_OFF_5 OVEN_TOP_OFF OVEN_TOP_OFF OVEN_TOP_OFF OVEN_TOP_OFF OVEN_TOP_OFF
#define OVEN_BUTTOM_ON_5 OVEN_BOTTOM_ON OVEN_BOTTOM_ON OVEN_BOTTOM_ON OVEN_BOTTOM_ON OVEN_BOTTOM_ON
#define OVEN_BUTTOM_OFF_5 OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF OVEN_BOTTOM_OFF

char * TopOven;
char * ButtomOven;

#define INVERT_DISPLAY 2

// **** Put any helper functions here ****

/*This function will update your OLED to reflect the state.*/
void updateOvenOLED(OvenData ovenData) {
    //update OLED here
    static char display[OLED_NUM_LINES][OLED_CHARS_PER_LINE];
    OledClear(OLED_COLOR_BLACK);
    TopOven = OVEN_TOP_OFF_5;
    ButtomOven = OVEN_BUTTOM_OFF_5;
    if (ovenData.state == SETUP || ovenData.state == SELECTOR_CHANGE_PENDING || ovenData.state == ALERT) {
        // if state = alert
        if (ovenData.state == ALERT) {
            if (ovenData.alert == NORMAL) {
                OledSetDisplayInverted();
                ovenData.alert = INVERTED;
            } else if (ovenData.alert == INVERTED) {
                OledSetDisplayNormal();
                ovenData.alert = NORMAL;
            }
        }
        // if mode = bake
        if (ovenData.mode == BAKE) {
            char * mode = "Bake";
            // if selector = time
            if (ovenData.selector == TIME) {
                // print mode
                sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
                // print time
                sprintf(display[1], "|     |  >Time: %d:%02d\n", ovenData.cooktime / 60, ovenData.cooktime % 60);
                // print temp
                sprintf(display[2], "|-----|   Temp: %03d%sF\n", ovenData.temperature, DEGREE_SYMBOL);
                sprintf(display[3], "|%s|\n", ButtomOven);
            }                
            // else if selector = temperature
            else if (ovenData.selector == TEMPERATURE) {
                // print mode
                sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
                // print time
                sprintf(display[1], "|     |   Time: %d:%02d\n", ovenData.cooktime / 60, ovenData.cooktime % 60);
                // print temp
                sprintf(display[2], "|-----|  >Temp: %03d%sF\n", ovenData.temperature, DEGREE_SYMBOL);
                sprintf(display[3], "|%s|\n", ButtomOven);
            }
        }            
        // else if mode = toast
        else if (ovenData.mode == TOAST) {
            char * mode = "Toast";
            // print mode
            sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
            // print time
            sprintf(display[1], "|     |   Time: %d:%02d\n", ovenData.cooktime / 60, ovenData.cooktime % 60);
            // temperature is not displayed
            sprintf(display[2], "|%s|               ", "-----");
            sprintf(display[3], "|%s|\n", ButtomOven);
        }            
        // else if mode = broil
        else if (ovenData.mode == BROIL) {
            char * mode = "Broil";
            // print mode
            sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
            // print time
            sprintf(display[1], "|     |   Time: %d:%02d\n", ovenData.cooktime / 60, ovenData.cooktime % 60);
            // print temp
            sprintf(display[2], "|-----|   Temp: 500%sF\n", DEGREE_SYMBOL);
            sprintf(display[3], "|%s|\n", ButtomOven);
        }
    } else if (ovenData.state == COOKING) {
        if (ovenData.mode == BAKE) {
            char * mode = "Bake";
            TopOven = OVEN_TOP_ON_5;
            ButtomOven = OVEN_BUTTOM_ON_5;
            // print mode
            sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
            // print time
            sprintf(display[1], "|     |   Time: %d:%02d\n", ovenData.cooktime_remain / 60, ovenData.cooktime_remain % 60);
            // print temp
            sprintf(display[2], "|-----|   Temp: %03d%sF\n", ovenData.temperature, DEGREE_SYMBOL);
            sprintf(display[3], "|%s|\n", ButtomOven);
        }            
        // else if mode = toast
        else if (ovenData.mode == TOAST) {
            TopOven = OVEN_TOP_OFF_5;
            ButtomOven = OVEN_BUTTOM_ON_5;
            char * mode = "Toast";
            // print mode
            sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
            // print time
            sprintf(display[1], "|     |   Time: %d:%02d\n", ovenData.cooktime_remain / 60, ovenData.cooktime_remain % 60);
            // temperature is not displayed
            sprintf(display[2], "|%s|               ", "-----");
            sprintf(display[3], "|%s|\n", ButtomOven);
        }            
        // else if mode = broil
        else if (ovenData.mode == BROIL) {
            char * mode = "Broil";
            TopOven = OVEN_TOP_ON_5;
            ButtomOven = OVEN_BUTTOM_OFF_5;
            // print mode
            sprintf(display[0], "|%s|   Mode: %s\n", TopOven, mode);
            // print time
            sprintf(display[1], "|     |   Time: %d:%02d\n", ovenData.cooktime / 60, ovenData.cooktime % 60);
            // print temp
            sprintf(display[2], "|-----|   Temp: 500%sF\n", DEGREE_SYMBOL);
            sprintf(display[3], "|%s|\n", ButtomOven);
        }
    }
    // update OLED
    static char OLED_display[OLED_NUM_LINES * OLED_CHARS_PER_LINE];
    sprintf(OLED_display, "%s%s%s%s", display[0], display[1], display[2], display[3]);
    OledDrawString(OLED_display);
    OledUpdate();
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    //write your SM logic here.
    switch (ovenData.state) {
        case SETUP:
            // if ADCevent == TRUE
            //      update time
            //          only if the selector is at time selector
            //          total cook time gets saved here
            //      update temperature
            //          only if the selector is at temp selector
            //          temperature gets saved here
            // if buttonEvent & BTN_EVENT_4DOWN
            //      store free running timer to start time variable (static uint16_t)
            //      store total cook time
            //      ovenData.state=COOKING;
            // else if buttonEvent & BTN_EVENT_3DOWN
            //      store free running timer to a start time variable (static uint16_t)
            //      ovenData.state=SELECTOR_CHANGE_PENDING
            //

            // if ADCevent == TRUE
            if (adcEvent_Flag == TRUE) {
                // update time
                if (ovenData.selector == TIME || ovenData.mode != BAKE) {
                    ovenData.cooktime = (adcReading >> 2) + 1;
                    updateOvenOLED(ovenData);
                }                    
                // update temperature
                else if (ovenData.selector == TEMPERATURE && ovenData.mode == BAKE) {
                    ovenData.temperature = (adcReading >> 2) + 300;
                    updateOvenOLED(ovenData);
                }
            }
            // if buttonEvent & BTN_EVENT_4DOWN
            if (buttonEvents_Flag == TRUE && buttonEvents == BUTTON_EVENT_4DOWN) {
                // store free running time
                StartTime = freeRunningTimer;
                ovenData.cooktime_remain = ovenData.cooktime;
                // update LED bar
                LEDs = LEDs_ON;
                LEDS_SET(LEDs);
                // update OLED
                ovenData.state = COOKING;
                updateOvenOLED(ovenData);
            }                
            // else if buttonEvent & BTN_EVENT_3DOWN
            else if (buttonEvents_Flag == TRUE && buttonEvents == BUTTON_EVENT_3DOWN) {
                // store free running time
                StartTime = freeRunningTimer;
                TimerTick = freeRunningTimer;
                ovenData.state = SELECTOR_CHANGE_PENDING;
            }
            break;

        case SELECTOR_CHANGE_PENDING:
            // if buttonEvent & BTN_EVENT_3UP
            if (buttonEvents_Flag == TRUE && buttonEvents == BUTTON_EVENT_3UP) {
                ElapsedTime = freeRunningTimer - StartTime;
                // if elapsed time < LONG_PRESS
                if (ElapsedTime < LONG_PRESS) {
                    // switch to next mode
                    if (ovenData.mode == BAKE) {
                        ovenData.mode = TOAST;
                    } else if (ovenData.mode == TOAST) {
                        ovenData.mode = BROIL;
                    } else if (ovenData.mode == BROIL) {
                        ovenData.mode = BAKE;
                    }
                    // update OLED
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                } else {
                    // change Settings Selector
                    if (ovenData.selector == TIME) {
                        ovenData.selector = TEMPERATURE;
                    } else if (ovenData.selector == TEMPERATURE) {
                        ovenData.selector = TIME;
                    }
                    // update OLED
                    updateOvenOLED(ovenData);
                    ovenData.state = SETUP;
                }
            }
            break;

        case COOKING:
            // if buttonEvent & BTN_EVENT_4DOWN
            if (buttonEvents_Flag == TRUE && buttonEvents == BUTTON_EVENT_4DOWN) {
                // store free running time
                StartTime = freeRunningTimer;
                ovenData.state = RESET_PENDING;
            }
            // if TIMER_TICK
            if ((freeRunningTimer - TimerTick) > LONG_PRESS) {
                TimerTick = freeRunningTimer;
                if (TimerTickEvent == TRUE) {
                    // count down the time
                    ovenData.cooktime_remain -= 1;
                    // if cook time > 0
                    if (ovenData.cooktime_remain > 0) {
                        // update LED bar
                        ElapsedTime = freeRunningTimer - StartTime;
                        if (ElapsedTime >= (5 * ovenData.cooktime) / 8) {
                            LEDs = LEDs << 1;
                            LEDS_SET(LEDs);
                            StartTime = freeRunningTimer;
                        }
                        // update OLED
                        updateOvenOLED(ovenData);
                        ovenData.state = COOKING;
                    } else {
                        // reset settings
                        ovenData.cooktime_remain = ovenData.cooktime;
                        // update LED bar
                        LEDs = LEDs_OFF;
                        LEDS_SET(LEDs);
                        // update OLED
                        updateOvenOLED(ovenData);
                        StartTime = freeRunningTimer;
                        // alert
                        ovenData.state = ALERT;
                    }
                }
            }
            break;

        case RESET_PENDING:
            // if buttonEvent & BTN_EVENT_4UP
            if (buttonEvents_Flag == TRUE && buttonEvents == BUTTON_EVENT_4UP) {
                ovenData.state = COOKING;
            }
            // if TIMER_TICK
            if (TimerTickEvent == TRUE) {
                ElapsedTime = freeRunningTimer - StartTime;
                // if elapsed time >= LONG_PRESS
                if (ElapsedTime >= LONG_PRESS) {
                    // end cooking
                    ovenData.state = SETUP;
                    // reset settings
                    ovenData.cooktime_remain = ovenData.cooktime;
                    // update OLED
                    updateOvenOLED(ovenData);
                    // update LED bar
                    LEDs = LEDs_OFF;
                    LEDS_SET(LEDs);
                } else {
                    // update LED bar
                    LEDS_SET(LEDs);
                    // update OLED
                    updateOvenOLED(ovenData);
                    ovenData.state = RESET_PENDING;
                }
            }
            break;

        case ALERT:
            // if buttonEvent & BTN_EVENT_4DOWN
            if (buttonEvents_Flag == TRUE && buttonEvents == BUTTON_EVENT_4DOWN) {
                // stop alert
                OledSetDisplayNormal();
                ovenData.state = SETUP;
            } else {
                ElapsedTime = freeRunningTimer - StartTime;
                // invert the display at 2Hz after cooking completes
                if (ElapsedTime >= INVERT_DISPLAY) {
                    // start alert
                    updateOvenOLED(ovenData);
                    StartTime = freeRunningTimer;
                }
            }
            break;

        default:
            // print to OLED an error occurred
            OledDrawString("Error!\n");
            OledUpdate();
            // and to restart the oven
            ovenData.state = SETUP;
            while (1);
            break;
    }
}

int main() {
    BOARD_Init();
    // initialize Leds
    LEDS_INIT();
    // initialize Button
    ButtonsInit();
    // initialize adc
    AdcInit();
    // initialize OLEDS
    OledInit();

    //initialize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to zchen287's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here

    while (1) {
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags
    
        // 3 different if statement
        // if ADC events {
        // on event, run runOvenSM()
        // clear event flags
        // updateLEDs();
        // updateOLED();
        // }
        if (adcEvent_Flag == TRUE) {
            runOvenSM();
            adcEvent_Flag = FALSE;
        }
        // if Button event {
        // on event, run runOvenSM()
        // clear event flags
        // updateLEDs();
        // updateOLED();
        // }
        if (buttonEvents_Flag == TRUE) {
            runOvenSM();
            buttonEvents_Flag = FALSE;
        }
        // if Time Ticker events {
        // on event, run runOvenSM()
        // clear event flags
        // updateLEDs();
        // updateOLED();
        // }
        if (TimerTickEvent == TRUE) {
            runOvenSM();
            TimerTickEvent = FALSE;
        }
    };
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    //add event-checking code here

    // free run timer
    freeRunningTimer++;
    // check TimerTickEvent (set it to TRUE)
    TimerTickEvent = TRUE;
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    //add event-checking code here

    // check button events
    buttonEvents = ButtonsCheckEvents();
    if (buttonEvents == BUTTON_EVENT_NONE) {
        buttonEvents_Flag = FALSE;
    } else {
        buttonEvents_Flag = TRUE;
    }
    // check adc events
    adcEvent = AdcChanged();
    if (adcEvent == TRUE) {
        // save adc reading to a variable
        adcReading = AdcRead();
        adcEvent_Flag = TRUE;
    } else {
        adcEvent_Flag = FALSE;
    }
}