// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"


// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    // Declare Variables
    float fahr, celsius;
    int lower, upper, step;

    // Initialize Variables
    lower = 0; // lower limit of temperature
    upper = 300; // upper limit
    step = 20; // step size
    fahr = lower;

    // Print Column Headers
    printf("%5s %5s\n", "F", "C");

    // Print out table
    while (fahr <= upper) {
        celsius = (5.0 / 9.0)*(fahr - 32.0);
        printf("%7.1f %04.0f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;
    }

    printf("\n");

    float kelv = lower;

    // Print Column Headers
    printf("%6s %5s\n", "K", "F");

    // Print out table
    while (kelv <= upper) {
        fahr = (9.0 / 5.0)*(kelv - 273.15) + 32.0;
        printf("%03.3f %5f\n", (double) kelv, (double) fahr);
        kelv = kelv + step;
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    BOARD_End();
    while (1);
}
