/* 
 * File:   FieldTest.c
 * Author: barry
 *
 * Created on June 4, 2021, 8:43 PM
 */

#include <stdio.h>
#include <stdint.h>
#include "Field.h"
#include "BOARD.h"

/*
 * 
 */
int main(int argc, char** argv) {
    int i;
    int j;
    Field *OwnField = malloc(sizeof (Field));
    Field *OpponentField = malloc(sizeof (Field));
    GuessData *gData = malloc(sizeof (GuessData));
    GuessData *gData_2 = malloc(sizeof (GuessData));
    Field *newField_1 = malloc(sizeof (Field));
    Field *newField_2 = malloc(sizeof (Field));
    Field *newField_3 = malloc(sizeof (Field));
    Field *newField_4 = malloc(sizeof (Field));
    Field *newField_5 = malloc(sizeof (Field));
    Field *newField_6 = malloc(sizeof (Field));

    printf("This is the test for Field.c\n");
    /********************************************/
    // Testing for FieldInit
    printf("    Test for FieldInit:\n");
    printf("    Expect field:\n");
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            printf(" 0 ");
        }
        printf("   |   ");
        for (j = 0; j < FIELD_COLS; j++) {
            printf(" 5 ");
        }
        printf("\n");
    }
    printf("\n");
    FieldInit(OwnField, OpponentField);
    printf("    After call FieldInit, it looks like:\n");
    FieldPrint_UART(OwnField, OpponentField);
    printf("\n");

    /********************************************/
    // Testing for FieldGetSquareStatus
    printf("    Test for FieldGetSquareStatus:\n");
    if (FieldGetSquareStatus(OwnField, 2, 3) == FIELD_SQUARE_EMPTY) {
        printf("    FieldGetSquareStatus test: PASSED! ");
    } else {
        printf("    FieldGetSquareStatus test: FAILED! ");
    }
    printf("\n");
    /********************************************/
    // Testing for FieldSetSquareStatus
    printf("    Test for FieldSetSquareStatus:\n");
    printf("    Expect field:\n");
    printf("    6  0  0  0  2  0  0  0  0  4\n");
    printf("    0  0  0  0  0  0  0  0  0  0\n");
    printf("    0  0  5  0  0  0  0  0  0  0\n");
    printf("    0  0  0  0  1  0  0  0  0  0\n");
    printf("    0  0  0  0  0  0  0  0  0  0\n");
    printf("    0  3  0  0  0  7  0  0  0  0\n");
    printf("    After call FieldSetSquareStatus, it looks like:\n");
    FieldSetSquareStatus(OwnField, 3, 4, FIELD_SQUARE_SMALL_BOAT);
    FieldSetSquareStatus(OwnField, 0, 4, FIELD_SQUARE_MEDIUM_BOAT);
    FieldSetSquareStatus(OwnField, 5, 1, FIELD_SQUARE_LARGE_BOAT);
    FieldSetSquareStatus(OwnField, 0, 9, FIELD_SQUARE_HUGE_BOAT);
    FieldSetSquareStatus(OwnField, 2, 2, FIELD_SQUARE_UNKNOWN);
    FieldSetSquareStatus(OwnField, 5, 5, FIELD_SQUARE_MISS);
    FieldSetSquareStatus(OwnField, 0, 0, FIELD_SQUARE_HIT);
    FieldPrint_UART(OwnField, OpponentField);
    FieldInit(OpponentField, OpponentField);
    printf("\n");

    /********************************************/
    // Testing for FieldAddBoat
    printf("    Test for FieldAddBoat:\n");
    printf("    Expect field:\n");
    printf("    3  3  3  0  0  0  3  0  0  0\n");
    printf("    4  4  4  4  0  0  3  0  0  0\n");
    printf("    0  0  5  0  0  0  3  0  0  0\n");
    printf("    0  0  0  0  0  0  0  0  0  0\n");
    printf("    0  0  0  0  0  0  0  0  0  0\n");
    printf("    0  0  0  0  0  0  0  0  0  0\n");
    printf("    After call FieldSetSquareStatus, it looks like:\n");
    FieldAddBoat(OwnField, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(OwnField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM);
    FieldAddBoat(OwnField, 1, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE);
    FieldAddBoat(OwnField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL);
    FieldPrint_UART(OwnField, OpponentField);
    FieldInit(OpponentField, OpponentField);
    printf("\n");

    /********************************************/
    // Testing for FieldRegisterEnemyAttack
    printf("    Test for FieldRegisterEnemyAttack:\n");
    gData->row = 0;
    gData->col = 0;
    FieldRegisterEnemyAttack(OwnField, gData);
    if (gData->result == RESULT_HIT) {
        printf("    FieldRegisterEnemyAttack test: PASSED! ");
    } else {
        printf("    FieldRegisterEnemyAttack test: FAILED! ");
    }
    printf("\n");

    /********************************************/
    // Testing for FieldUpdateKnowledge
    printf("    Test for FieldUpdateKnowledge:\n");
    gData_2->row = 0;
    gData_2->col = 0;
    gData_2->result = RESULT_HIT;
    FieldUpdateKnowledge(OwnField, gData_2);
    if (OwnField->grid[0][0] == FIELD_SQUARE_HIT) {
        printf("    FieldUpdateKnowledge test: PASSED! ");
    } else {
        printf("    FieldUpdateKnowledge test: FAILED! ");
    }
    printf("\n");

    /********************************************/
    // Testing for FieldUpdateKnowledge
    printf("    Test for FieldGetBoatStates:\n");
    FieldInit(newField_1, newField_2);
    FieldAddBoat(newField_1, 0, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL);
    FieldAddBoat(newField_1, 3, 0, FIELD_DIR_EAST, FIELD_BOAT_TYPE_LARGE);
    if (FieldGetBoatStates(newField_1) == 0x05) {
        printf("    FieldGetBoatStates test: PASSED! ");
    } else {
        printf("    FieldGetBoatStates test: FAILED! ");
    }
    printf("\n");

    /********************************************/
    // Testing for FieldAIPlaceAllBoats
    printf("    Test for FieldAIPlaceAllBoats:\n");
    FieldInit(newField_3, newField_4);
    FieldAIPlaceAllBoats(newField_3);
    FieldPrint_UART(newField_3, newField_4);
    printf("\n");

    /********************************************/
    // Testing for FieldAIDecideGuess
    printf("    Test for FieldAIDecideGuess:\n");
    FieldInit(newField_5, newField_6);
    FieldSetSquareStatus(newField_6, 0, 0, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 1, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 2, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 3, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 4, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 5, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 6, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 7, FIELD_SQUARE_EMPTY);
    FieldSetSquareStatus(newField_6, 0, 8, FIELD_SQUARE_EMPTY);
    FieldPrint_UART(newField_5, newField_6);
    GuessData guess = FieldAIDecideGuess(newField_6);
    if (guess.row == 0 && guess.col == 9) {
        printf("    FieldAIDecideGuess test: PASSED! ");
    } else {
        printf("    FieldAIDecideGuess test: FAILED! ");
    }
    
}

