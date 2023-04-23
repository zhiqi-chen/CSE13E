/* 
 * File:   part0.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on April 5, 2021, 2:54 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init();
    printf("Hello World\n");
    
    BOARD_End(); //handles cleanup of the system
    while(1); // if embedded we should never exit.
}