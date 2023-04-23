/* 
 * File:   NegotiationTest.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on Jun 4, 2021, 15:47 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"

// User libraries
#include "Negotiation.h"

int main() {
    // NegotiationHash
    printf("Test for NegotiationHash(): \n");
    NegotiationData hash_test = NegotiationHash(12345);
    if (hash_test == 43182) {
        printf("PASSED\n");
    }
    else {
        printf("FAILED\n");
    }
    
    // NegotiationVerify
    printf("Test for NegotiationVerify(): \n");
    int verify_test = NegotiationVerify(12345, 43182);
    if (verify_test == TRUE) {
        printf("PASSED\n");
    }
    else {
        printf("FAILED\n");
    }
    
    // NegotiateCoinFlip
    printf("Test for NegotiateCoinFlip(): \n");
    NegotiationOutcome coinflip_test = NegotiateCoinFlip(0b11101011, 0b01101011);
    if (coinflip_test == HEADS) {
        printf("PASSED\n");
    }
    else {
        printf("FAILED\n");
    }
}