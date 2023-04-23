// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Buttons.h"

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void ButtonsInit(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void) {

    uint8_t buttonsState = BUTTON_STATES();
    uint8_t Last_buttonsState = 0; // the buttons start in an off state with value 0
    uint8_t Return_buttonsState = 0;
    int Debouncing_Counter_1 = 0; // counter for button 1 debounce period
    int Debouncing_Counter_2 = 0; // counter for button 2 debounce period
    int Debouncing_Counter_3 = 0; // counter for button 3 debounce period
    int Debouncing_Counter_4 = 0; // counter for button 4 debounce period

    // if the current button states don't match with the last one
    if (buttonsState != Last_buttonsState) {

        // if button 1 state has been changed
        if ((buttonsState & BUTTON_STATE_1) != (Last_buttonsState & BUTTON_STATE_1)) {
            Debouncing_Counter_1++; // debouncing counter 1 increment
        }
        // if button 2 state has been changed
        if ((buttonsState & BUTTON_STATE_2) != (Last_buttonsState & BUTTON_STATE_2)) {
            Debouncing_Counter_2++; // debouncing counter 2 increment
        }
        // if button 3 state has been changed
        if ((buttonsState & BUTTON_STATE_3) != (Last_buttonsState & BUTTON_STATE_3)) {
            Debouncing_Counter_3++; // debouncing counter 3 increment
        }
        // if button 4 state has been changed
        if ((buttonsState & BUTTON_STATE_4) != (Last_buttonsState & BUTTON_STATE_4)) {
            Debouncing_Counter_4++; // debouncing counter 4 increment
        }

        // if the debounce period has been reached
        if (Debouncing_Counter_1 == BUTTONS_DEBOUNCE_PERIOD || Debouncing_Counter_2 == BUTTONS_DEBOUNCE_PERIOD || Debouncing_Counter_3 == BUTTONS_DEBOUNCE_PERIOD || Debouncing_Counter_4 == BUTTONS_DEBOUNCE_PERIOD) {
            // if the debounce period of button 1 has been reached
            if (Debouncing_Counter_1 == BUTTONS_DEBOUNCE_PERIOD) {
                // if buttons 1 is pressed down
                if (buttonsState & BUTTON_STATE_1) {
                    Return_buttonsState |= BUTTON_EVENT_1DOWN;
                }                    
                // else buttons 1 is released up
                else {
                    Return_buttonsState |= BUTTON_EVENT_1UP;
                }
                // initialize the counter for button 1 debounce period
                Debouncing_Counter_1 = 0;
            }
            // if the debounce period of button 2 has been reached
            if (Debouncing_Counter_2 == BUTTONS_DEBOUNCE_PERIOD) {
                // if buttons 2 is pressed down
                if (buttonsState & BUTTON_STATE_2) {
                    Return_buttonsState |= BUTTON_EVENT_2DOWN;
                }                    
                // else buttons 2 is released up
                else {
                    Return_buttonsState |= BUTTON_EVENT_2UP;
                }
                // initialize the counter for button 2 debounce period
                Debouncing_Counter_2 = 0;
            }
            // if the debounce period of button 3 has been reached
            if (Debouncing_Counter_3 == BUTTONS_DEBOUNCE_PERIOD) {
                // if buttons 3 is pressed down
                if (buttonsState & BUTTON_STATE_3) {
                    Return_buttonsState |= BUTTON_EVENT_3DOWN;
                }                    
                // else buttons 3 is released up
                else {
                    Return_buttonsState |= BUTTON_EVENT_3UP;
                }
                // initialize the counter for button 3 debounce period
                Debouncing_Counter_3 = 0;
            }
            // if the debounce period of button 4 has been reached
            if (Debouncing_Counter_4 == BUTTONS_DEBOUNCE_PERIOD) {
                // if buttons 4 is pressed down
                if (buttonsState & BUTTON_STATE_4) {
                    Return_buttonsState |= BUTTON_EVENT_4DOWN;
                }                    
                // else buttons 4 is released up
                else {
                    Return_buttonsState |= BUTTON_EVENT_4UP;
                }
                // initialize the counter for button 4 debounce period
                Debouncing_Counter_4 = 0;
            }
            Last_buttonsState = buttonsState;
        }            
        // else, the debounce period has not been reached
        else {
            Return_buttonsState = BUTTON_EVENT_NONE;
        }
    }        
    // else, the current button states match with the last one
    else {
        Return_buttonsState = BUTTON_EVENT_NONE;
    }
    return Return_buttonsState;
}

