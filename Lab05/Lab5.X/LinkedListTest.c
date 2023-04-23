/* 
 * File:   LinkedListTest.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on May 2, 2021, 9:53 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CMPE13 Support Library
#include "BOARD.h"


// User libraries:
#include "LinkedList.h"

/*
 * 
 */
int main() {
    BOARD_Init();
    
    printf("\n###### Beginning zchen287's LinkedList test harness: ####\n\n");
    
    // LinkedListNew
    printf("test for LinkedListNew():\n");
    char *check1="Hello";
    ListItem *test1=LinkedListNew(check1);
    if (strcmp(test1->data, check1)==0 && test1->nextItem==NULL && test1->previousItem==NULL) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    char *check2="World";
    ListItem *test2=LinkedListNew(check2);
    if (strcmp(test2->data, check2)==0 && test2->nextItem==NULL && test2->previousItem==NULL) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListCreateAfter
    printf("test for LinkedListCreateAfter():\n");
    char *check3="Hello ";
    char *check4="World";
    ListItem *test3=LinkedListNew(check3);
    ListItem *test4=LinkedListCreateAfter(test3, check4);
    if (strcmp(test3->data, check3)==0 && strcmp(test4->data, check4)==0 && test4->nextItem==NULL && test3->previousItem==NULL) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    char *check5="Zhiqi ";
    char *check6="Chen";
    ListItem *test5=LinkedListNew(check5);
    ListItem *test6=LinkedListCreateAfter(test5, check6);
    if (strcmp(test5->data, check5)==0 && strcmp(test6->data, check6)==0 && test6->nextItem==NULL && test5->previousItem==NULL) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListRemove
    printf("test for LinkedListRemove():\n");
    char *test7=LinkedListRemove(test3);
    if (strcmp(test7, check3)==0) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    char *test8=LinkedListRemove(test4);
    if (strcmp(test8, check4)==0) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListSize
    printf("test for LinkedListSize():\n");
    int test9=LinkedListSize(test6);
    if (test9==2) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    int test10=LinkedListSize(test5);
    if (test10==2) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListGetFirst
    printf("test for LinkedListGetFirst():\n");
    char *character1="Hello ";
    char *character2="World ";
    char *character3="zchen287";
    ListItem *first=LinkedListNew(character1);
    ListItem *second=LinkedListCreateAfter(first, character2);
    ListItem *third=LinkedListCreateAfter(second, character3);
    
    ListItem *GetFirst1=LinkedListGetFirst(second);
    if (GetFirst1==first) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    ListItem *GetFirst2=LinkedListGetFirst(third);
    if (GetFirst2==first) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListGetLast
    printf("test for LinkedListGetLast():\n");
    ListItem *GetLast1=LinkedListGetLast(third);
    if (GetLast1==third) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    ListItem *GetLast2=LinkedListGetLast(second);
    if (GetLast2==third) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListSwapData
    printf("test for LinkedListSwapData():\n");
    
    int Swap1=LinkedListSwapData(first, second);
    if (Swap1==SUCCESS) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    int Swap2=LinkedListSwapData(third, first);
    if (Swap2==SUCCESS) {
        printf("Passed\n");
    }
    else {
        printf("Failed\n");
    }
    
    // LinkedListPrint
    printf("test for LinkedListPrint():\n");
    
    char *character4="Hello ";
    char *character5="World ";
    char *character6="zchen287";
    ListItem *Print1=LinkedListNew(character4);
    ListItem *Print2=LinkedListCreateAfter(Print1, character5);
    ListItem *Print3=LinkedListCreateAfter(Print2, character6);
    
    int Print=LinkedListPrint(Print3);
    if (Print==SUCCESS) {
        printf("\nEXPECTED: [Hello, World, zchen287]\n");      
    }
    else {
        printf("\nFailed\n");
    } 
    
    BOARD_End();
    while (1);
}

