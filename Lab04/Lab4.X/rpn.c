/* 
 * File:   rpn.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on April 25, 2021, 11:53 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"




// User libraries:
#include "rpn.h"
#include "stack.h"

/* RPN_Evaluate() parses and evaluates a string that contains 
 * a valid Reverse Polish Notation string (no newlines!)  
 * @param:  rpn_string - a string in polish notation.  Tokens must be either 
 *          arithmetic operators or numbers.
 * @param:  result - a pointer to a double that will be modified to contain
 *          the return value of the rpn expression.
 * @return: error - if the rpn expression is invalid, 
 *          an appropriate rpn_error value is returned.
 * 
 * RPN_Evaluate supports the following basic arithmetic operations:
 *   + : addition
 *   - : subtraction
 *   * : multiplication
 *   / : division
 * Numeric tokens can be positive or negative, and can be integers or 
 * decimal floats.  RPN_Evaluate should be able to handle strings of 
 * at least 255 length.
 * */
int RPN_Evaluate(char * rpn_string, double * result) {
    double number1;
    double number2;
    double value;
    double num;
    struct Stack stack = {};
    StackInit(&stack);
    // start from the first character
    char * token = strtok(rpn_string, " ");

    // loop for check and operate
    while (token != NULL) {
        // number
        float numbers = atof(token);
        if (numbers != 0 || strcmp(token, "0") == 0) {
            // push the numbers into stack
            int check_numbers = StackPush(&stack, numbers);
            // error: overflow
            if (check_numbers == STANDARD_ERROR) {
                return RPN_ERROR_STACK_OVERFLOW;
            }
        }            
        // operator
        else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            // pop out the previous two numbers from stack
            int check1 = StackPop(&stack, &number1);
            int check2 = StackPop(&stack, &number2);
            // error: underflow
            if (check1 == STANDARD_ERROR || check2 == STANDARD_ERROR) {
                return RPN_ERROR_STACK_UNDERFLOW;
            }                
            // no error
            else {
                // addition
                if (strcmp(token, "+") == 0) {
                    value = number2 + number1;
                    // push the result into stack
                    int check_addition = StackPush(&stack, value);
                    // error: overflow
                    if (check_addition == STANDARD_ERROR) {
                        return RPN_ERROR_STACK_OVERFLOW;
                    }
                }
                // subtraction
                if (strcmp(token, "-") == 0) {
                    value = number2 - number1;
                    // push the result into stack
                    int check_subtraction = StackPush(&stack, value);
                    // error: overflow
                    if (check_subtraction == STANDARD_ERROR) {
                        return RPN_ERROR_STACK_OVERFLOW;
                    }
                }
                // multiplication
                if (strcmp(token, "*") == 0) {
                    value = number2 * number1;
                    // push the result into stack
                    int check_multiplication = StackPush(&stack, value);
                    // error: overflow
                    if (check_multiplication == STANDARD_ERROR) {
                        return RPN_ERROR_STACK_OVERFLOW;
                    }
                }
                // division
                if (strcmp(token, "/") == 0) {
                    value = number2 / number1;
                    // push the result into stack
                    int check_division = StackPush(&stack, value);
                    // error: overflow
                    if (check_division == STANDARD_ERROR) {
                        return RPN_ERROR_STACK_OVERFLOW;
                    }
                    // error: divide by zero
                    if (number1 == 0.00) {
                        return RPN_ERROR_DIVIDE_BY_ZERO;
                    }
                }
            }
        }            
        // error: invalid token (neither numbers nor operators)
        else {
            return RPN_ERROR_INVALID_TOKEN;
        }
        // next character
        token = strtok(NULL, " ");
    }

    // loop done, finish all the checking and operation, 
    //exactly one item should left in stack as the final result

    // error: too few items remain
    if (StackGetSize(&stack) < 1) {
        return RPN_ERROR_TOO_FEW_ITEMS_REMAIN;
    }        // error: too many items remain
    else if (StackGetSize(&stack) > 1) {
        return RPN_ERROR_TOO_MANY_ITEMS_REMAIN;
    }        
    // no error
    else {
        StackPop(&stack, &num);
        *result = num;
        return RPN_NO_ERROR;
    }
}

/**
 * This function should read through an array of characters, checking for backspace characters. 
 * When it encounters a backspace character, it eliminates the backspace, the preceeding character
 * (if such a character exists), and shifts all subsequent characters as appropriate.
 * @param string_to_modify The string that will be processed for backspaces. 
 *        This string is modified "in place", so it is both an input and an output to the function.
 * @return Returns the size of the resulting string in "string_to_modify".
 *
 * ProcessBackspaces() should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings of at least 255 length.
 * 
 * */
int ProcessBackspaces(char *rpn_sentence) {
    int i;
    int j;
    int n;

    for (i = 0; i < strlen(rpn_sentence); i++) {
        while (rpn_sentence[i] == '\b') {
            if (i != 0 && rpn_sentence[i - 1] != '\b') {
                for (j = i; j < strlen(rpn_sentence); j++) {
                    rpn_sentence[j - 1] = rpn_sentence[j + 1];
                }
            } else {
                for (n = i; n < strlen(rpn_sentence); n++) {
                    rpn_sentence[n] = rpn_sentence[n + 1];
                }
            }
            i--;
        }
    }
    return sizeof (rpn_sentence);
}

