/* 
 * File:   Calculator.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on April 15, 2021, 9:53 AM
 */

// Standard libraries
#include <stdio.h>
#include <math.h>


//User libraries:
#include "BOARD.h"




// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Average(double operand1, double operand2);
double Tangent(double operand);
double Round(double operand);
//add more prototypes here

void CalculatorRun(void) {
    printf("\n\nWelcome to Zhiqi's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);

    // Your code here
    while (1) // this while(1) can be removed as needed.
    {
        printf("\n\nEnter a mathematical operation to perform (*,/,+,-,m,a,c,f,t,q,r): ");
        char operator;
        scanf("%1c", &operator);

        // if binary operator
        if (operator == '*' || operator == '/' || operator == '+' || operator == '-' || operator == 'm') {
            printf("   Enter the first operand: "); // enter first operand
            double operand1;
            scanf("%lf", &operand1);
            printf("   Enter the second operand: "); // enter second operand
            double operand2;
            scanf("%lf", &operand2);
            // multiply
            if (operator == '*') {
                double result = Multiply(operand1, operand2);
                printf("   Result of (%lf", operand1);
                printf(" * %lf", operand2);
                printf("): %lf", result);
            }
            // divide
            if (operator == '/') {
                // divided by 0
                if (operand2 == 0) {
                    printf("   Divided by zero error!");
                }                    
                // not divided by 0
                else {
                    double result = Divide(operand1, operand2);
                    printf("   Result of (%lf", operand1);
                    printf(" / %lf", operand2);
                    printf("): %lf", result);
                }
            }
            // add
            if (operator == '+') {
                double result = Add(operand1, operand2);
                printf("   Result of (%lf", operand1);
                printf(" + %lf", operand2);
                printf("): %lf", result);
            }
            // subtract
            if (operator == '-') {
                double result = Subtract(operand1, operand2);
                printf("   Result of (%lf", operand1);
                printf(" - %lf", operand2);
                printf("): %lf", result);
            }
            // average
            if (operator == 'm') {
                double result = Average(operand1, operand2);
                printf("   Result of average(%lf", operand1);
                printf(",%lf", operand2);
                printf("): %lf", result);
            }
        }
            // if unary operator
        else if (operator == 'a' || operator == 'c' || operator == 'f' || operator == 't' || operator == 'r') {
            printf("   Enter the first operand: "); // enter operand
            double operand;
            scanf("%lf", &operand);
            // absolute value
            if (operator == 'a') {
                double result = AbsoluteValue(operand);
                printf("   Result of |%lf", operand);
                printf("|: %lf", result);
            }
            // Fahrenheit to Celsius
            if (operator == 'c') {
                double result = FahrenheitToCelsius(operand);
                printf("   Result of (%lf", operand);
                printf(" deg->C): %lf", result);
            }
            // Celsius to Fahrenheit
            if (operator == 'f') {
                double result = CelsiusToFahrenheit(operand);
                printf("   Result of (%lf", operand);
                printf(" deg->F): %lf", result);
            }
            // tangent
            if (operator == 't') {
                double result = Tangent(operand);
                printf("   Result of tan(%lf", operand);
                printf("): %lf", result);
            }
            // round
            if (operator == 'r') {
                double result = Round(operand);
                printf("   Result of round(%lf", operand);
                printf("): %lf", result);
            }
        }
        // quit
        else if (operator == 'q') {
            break;
        }
        // invalid operator    
        else {
            operator = 0;
            printf("   Error, not a valid operator");
        }

    }
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    double result;
    result = operand1 + operand2;
    return result;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    double result;
    result = operand1 - operand2;
    return result;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    double result;
    result = operand1*operand2;
    return result;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    double result;
    result = operand1 / operand2;
    return result;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand) {
    double result;
    result = fabs(operand);
    return result;
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand) {
    double result;
    result = (operand - 32)*5.0 / 9.0;
    return result;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand) {
    double result;
    result = (operand * 9.0 / 5.0) + 32.0;
    return result;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    double result;
    result = (operand1 + operand2) / 2.0;
    return result;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand) {
    double result;
    result = tan(operand / 180 * M_PI);
    return result;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    double result;
    double fraction;
    double integer;

    fraction = modf(operand, &integer);

    // if operand>0
    if (integer > 0) {
        // if fraction<0.5
        if (fabs(fraction) < 0.5) {
            result = floor(operand);
        }            
        // if fraction>=0.5
        else {
            result = ceil(operand);
        }
    }        
    // if operand<=0
    else {
        // if fraction<0.5
        if (fabs(fraction) < 0.5) {
            result = ceil(operand);
        }            
        // if fraction>=0.5
        else {
            result = floor(operand);
        }
    }
    return result;
}