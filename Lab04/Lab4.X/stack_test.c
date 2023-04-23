/* 
 * File:   stack_test.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on April 25, 2021, 11:53 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries
#include "stack.h"

int main() {
    BOARD_Init();

    printf("\n###### Beginning zchen287's stack test harness: ####\n\n");

    // What follows is starter code.  You will need to modify it!

    // test StackInit:
    printf("StackInit():\n");
    struct Stack stack1 = {};
    StackInit(&stack1);
    if (stack1.currentItemIndex == -1) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    // test StackPush:
    printf("StackPush():\n");
    struct Stack stack2 = {};
    StackInit(&stack2);
    int check1 = StackPush(&stack2, -9);
    float value_push = stack2.stackItems[stack2.currentItemIndex];
    if (value_push == -9.0 && check1 == SUCCESS) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    // test StackPop:
    printf("StackPop():\n");
    struct Stack stack3 = {};
    double value_pop;
    StackInit(&stack3);
    StackPush(&stack3, -9);
    StackPush(&stack3, 6);
    int check2 = StackPop(&stack3, &value_pop);
    if (value_pop == 6.0 && check2 == SUCCESS) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    // test StackIsEmpty:
    printf("StackIsEmpty():\n");
    struct Stack stack4 = {};
    StackInit(&stack4);
    int check4 = StackIsEmpty(&stack4);
    if (check4 == TRUE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    // test StackIsFull:
    printf("StackIsFull():\n");
    struct Stack stack5 = {};
    StackInit(&stack5);
    int i;
    for (i = 0; i < STACK_SIZE; i++) {
        StackPush(&stack5, 8);
    }
    int check5 = StackIsFull(&stack5);
    if (check5 == TRUE) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    // test StackGetSize:
    printf("StackGetSize():\n");
    struct Stack stack6 = {};
    StackInit(&stack6);
    int j;
    for (j = 0; j < 13; j++) {
        StackPush(&stack6, 8);
    }
    int check6 = StackGetSize(&stack6);
    if (check6 == 13) {
        printf("passed\n");
    } else {
        printf("failed\n");
    }

    BOARD_End();
    while (1);
    return 0;
}


