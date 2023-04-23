/* 
 * File:   rpn_test.c
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

int main() {
    BOARD_Init();
    int error;

    printf("\n###### Beginning zchen287's rpn test harness: ####\n\n");

    //What follows is starter code.  You will need to modify it!

    char test0[] = "1 1 +";
    double result0;
    double expected0 = 2;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test0);
    error = RPN_Evaluate(test0, &result0);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result0 != expected0) {
        printf("   Failed, expected = %f , result = %f\n", expected0, result0);
    } else {
        printf("   Success!\n");
    }

    char test1[] = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21";
    double result1;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test1);
    error = RPN_Evaluate(test1, &result1);
    if (error == RPN_ERROR_STACK_OVERFLOW) {
        printf("   Success!\n");
    } else {
        printf("   Failed!\n");
    }

    char test2[] = "5 -";
    double result2;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test2);
    error = RPN_Evaluate(test2, &result2);
    if (error == RPN_ERROR_STACK_UNDERFLOW) {
        printf("   Success!\n");
    } else {
        printf("   Failed!\n");
    }

    char test3[] = "4 g *";
    double result3;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test3);
    error = RPN_Evaluate(test3, &result3);
    if (error == RPN_ERROR_INVALID_TOKEN) {
        printf("   Success!\n");
    } else {
        printf("   Failed!\n");
    }

    char test4[] = "3 0 /";
    double result4;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test4);
    error = RPN_Evaluate(test4, &result4);
    if (error == RPN_ERROR_DIVIDE_BY_ZERO) {
        printf("   Success!\n");
    } else {
        printf("   Failed!\n");
    }

    char test5[] = "6 8 9 +";
    double result5;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test5);
    error = RPN_Evaluate(test5, &result5);
    if (error == RPN_ERROR_TOO_MANY_ITEMS_REMAIN) {
        printf("   Success!\n");
    } else {
        printf("   Failed!\n");
    }

    char test6[] = "8 9 + -";
    double result6;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test6);
    error = RPN_Evaluate(test6, &result6);
    if (error == RPN_ERROR_STACK_UNDERFLOW) {
        printf("   Success!\n");
    } else {
        printf("   Failed!\n");
    }

    char test7[] = "1 9 + 6 - 2 / 8 *";
    double result7;
    double expected7 = 16.0;
    printf("Testing RPN_Evaluate with \"%s\"... \n ", test7);
    error = RPN_Evaluate(test7, &result7);
    if (error) {
        printf("   Failed, RPN_Evaluate produced an error\n");
    } else if (result7 != expected7) {
        printf("   Failed, expected = %f , result = %f\n", expected7, result7);
    } else {
        printf("   Success!\n");
    }

    printf("Testing ProcessBackspaces:\n");
    char test_pb1[] = "123\b34";
    ProcessBackspaces(test_pb1);
    printf("result    : %s\n", test_pb1);
    printf("should be : 1234\n");

    printf("Testing ProcessBackspaces:\n");
    char test_pb2[] = "123\b\b\b\b\b4";
    ProcessBackspaces(test_pb2);
    printf("result    : %s\n", test_pb2);
    printf("should be : 4\n");

    printf("Testing ProcessBackspaces:\n");
    char test_pb3[] = "\b\b\b\b\b123\b";
    ProcessBackspaces(test_pb3);
    printf("result    : %s\n", test_pb3);
    printf("should be : 12\n");

    BOARD_End();
    while (1);
}


