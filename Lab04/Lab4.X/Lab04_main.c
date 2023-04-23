/* 
 * File:   Lab04_main.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on April 25, 2021, 11:53 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"



// User libraries
#include "rpn.h"

// **** Set macros and preprocessor directives ****
// The lab calls for 60 characters of user input
#define MAX_INPUT_LENGTH 60

int main() {
    BOARD_Init();

    //What follows is starter code.  You will need to modify it!

    char rpn_sentence[MAX_INPUT_LENGTH + 2];
    double result;
    int error;

    printf("Welcome to zchen287'S RPN calculator.  Compiled on %s %s", __DATE__, __TIME__);
    while (1) {

        printf("\nEnter floats and + - / * in RPN format:\n");

        fgets(rpn_sentence, sizeof (rpn_sentence), stdin);

        if (rpn_sentence[strlen(rpn_sentence) - 1] == '\n') {
            rpn_sentence[strlen(rpn_sentence) - 1] = '\0';
        }

        error = RPN_Evaluate(rpn_sentence, &result);

        if (error == RPN_ERROR_STACK_OVERFLOW) {
            printf("Error: No more room on stack.");
        }
        if (error == RPN_ERROR_STACK_UNDERFLOW) {
            printf("Error: Not enough operands before operator.");
        }
        if (error == RPN_ERROR_INVALID_TOKEN) {
            printf("Error: Invalid character in RPN string.");
        }
        if (error == RPN_ERROR_DIVIDE_BY_ZERO) {
            printf("Error: Zero cannot be the denominator.");
        }
        if (error == RPN_ERROR_TOO_FEW_ITEMS_REMAIN) {
            printf("Error: Less than one item in the stack.");
        }
        if (error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN) {
            printf("Error: More than one item in the stack.");
        }
        if (error == RPN_NO_ERROR) {
            printf("result = %f", result);
        }
    }


    while (1);
}
