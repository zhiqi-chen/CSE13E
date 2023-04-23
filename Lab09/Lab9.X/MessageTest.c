/* 
 * File:   MessageTest.c
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
#include "Message.h"

// **** Define any module-level, global, or external variables here ****
// Message_ParseMessage
BB_Event message_event_test = {BB_EVENT_NO_EVENT, 0, 0, 0};
BB_Event message_event_check = {BB_EVENT_RES_RECEIVED, 1, 0, 3};

int main() {
    // Message_CalculateChecksum
    printf("Test for Message_CalculateChecksum(): \n");
    const char * payload_test = "RES,1,0,3";
    uint8_t checksum_test = Message_CalculateChecksum(payload_test);
    uint8_t checksum_check = 0x5A;
    if (checksum_test == checksum_check) {
        printf("PASSED\n");
    }
    else {
        printf("FAILED\n");
        printf("Your Answer: %u", checksum_test);
        printf("Correct Answer: %u", checksum_check);
    }
    
    // Message_ParseMessage
    printf("Test for Message_ParseMessage(): \n");
    const char * checksum_string_test = "5A";
    BB_Event message_event_test = {BB_EVENT_NO_EVENT, 0 ,0, 0};
    BB_Event message_event_check = {BB_EVENT_RES_RECEIVED, 1 ,0, 3};
    Message_ParseMessage(payload_test, checksum_string_test, &message_event_test);
    // type
    if (message_event_test.type == message_event_check.type) {
        printf("PASSED type\n");
    }
    else {
        printf("FAILED type\n");
    }
    // param0
    if (message_event_test.param0 == message_event_check.param0) {
        printf("PASSED param0\n");
    }
    else {
        printf("FAILED param0\n");
    }
    // param1
    if (message_event_test.param1 == message_event_check.param1) {
        printf("PASSED param1\n");
    }
    else {
        printf("FAILED param1\n");
    }
     // param2
    if (message_event_test.param2 == message_event_check.param2) {
        printf("PASSED param2\n");
    }
    else {
        printf("FAILED param2\n");
    }
    
    // Message_Encode
    printf("Test for Message_Encode(): \n");
    char message_string_test;
    char * message_string_check = "$RES,1,0,3*5A\n";
    Message message_to_encode = {MESSAGE_RES, 1, 0, 3};
    Message_Encode(&message_string_test, message_to_encode);
    printf("Your Answer: %s", &message_string_test);
    printf("Correct Answer: %s", message_string_check);
    
    // Message_Decode
    printf("Test for Message_Decode(): \n");
    unsigned char char_in = '$';
    BB_Event decoded_message_event = {BB_EVENT_RES_RECEIVED, 1 ,0, 3};
    if (Message_Decode(char_in, &decoded_message_event) == SUCCESS) {
        printf("PASSED\n");
    }
    else {
        printf("FAILED\n");
    }
}
