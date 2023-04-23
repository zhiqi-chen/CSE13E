/* 
 * File:   Negotiation.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on Jun 2, 2021, 20:50 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"

// User libraries
#include "Message.h"

// **** Set any macros or preprocessor directives here ****


// **** Set any local typedefs here ****
typedef enum {
    WAITING, PROCESSING_PAYLOAD, PROCESSING_CHECKSUM
} decodeState;

/**
 * Given a payload string, calculate its checksum
 * 
 * @param payload       //the string whose checksum we wish to calculate
 * @return   //The resulting 8-bit checksum 
 */
uint8_t Message_CalculateChecksum(const char* payload) {
    // declare loop var
    int i = 0;
    // declare uint8 checksum
    uint8_t checksum = 0b00000000;
    // loop through the payload and modify checksum
    for (i = 0; i < MESSAGE_MAX_PAYLOAD_LEN; i++) {
        if (payload[i] == 0) {
            break;
        }
        else {
            // checksum ^= payload[i]
            checksum ^= payload[i];
        }
    }
    // return checksum
    return checksum;
}

/**
 * ParseMessage() converts a message string into a BB_Event.  The payload and
 * checksum of a message are passed into ParseMessage(), and it modifies a
 * BB_Event struct in place to reflect the contents of the message.
 * 
 * @param payload       //the payload of a message
 * @param checksum      //the checksum (in string form) of a message,
 *                          should be exactly 2 chars long, plus a null char
 * @param message_event //A BB_Event which will be modified by this function.
 *                      //If the message could be parsed successfully,
 *                          message_event's type will correspond to the message type and 
 *                          its parameters will match the message's data fields.
 *                      //If the message could not be parsed,
 *                          message_events type will be BB_EVENT_ERROR
 * 
 * @return STANDARD_ERROR if:
 *              the payload does not match the checksum
 *              the checksum string is not two characters long
 *              the message does not match any message template
 *          SUCCESS otherwise
 * 
 * Please note!  sscanf() has a couple compiler bugs that make it a very
 * unreliable tool for implementing this function. * 
 */
int Message_ParseMessage(const char* payload,
        const char* checksum_string, BB_Event * message_event) {
    // check message length
    // get checksum using strtoul()
    // check the checksum
    // compare payload token with message ID
    // save parameters
    // for any errors checking the parameters and incoming message
    // return STANDARD_ERROR
    // else SUCCESS
    
    // char payload_cpy[Max_LEN]
    // strcpy(payload_cpy, payload)
    // check the length of the checksum
    //     return error if it does not match
    // strtok the payload and save the parameter count
    // check which message type is and set the type of the message event
    // if it is not of any of the types, set the message event type as error and return an error
    // loop until the parameter count
    //     get the token
    //     set the parameters in the message event
    // check if there is any token left, return error if there are
    // return success otherwise
    
    // check payload length
    char payload_cpy[MESSAGE_MAX_PAYLOAD_LEN + 1];
    strcpy(payload_cpy, payload);
    
    // check checksum length
    if (strlen(checksum_string) != MESSAGE_CHECKSUM_LEN) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }   
    // check if the payload match with checksum
    uint8_t checksum_ul = strtoul(checksum_string, NULL, 16);
    uint8_t checksum_cal = Message_CalculateChecksum(payload_cpy);
    if (checksum_ul != checksum_cal) {
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // message ID
    char * messageID = strtok(payload_cpy, ",");   
    if (strcmp(messageID, "CHA") == 0) {
        // message type
        message_event->type = BB_EVENT_CHA_RECEIVED;
        // data fields
        message_event->param0 = atoi(strtok(NULL, ","));
    }
    else if (strcmp(messageID, "ACC") == 0) {
        // message type
        message_event->type = BB_EVENT_ACC_RECEIVED;
        // data fields
        message_event->param0 = atoi(strtok(NULL, ","));
    }
    else if (strcmp(messageID, "REV") == 0) {
        // message type
        message_event->type = BB_EVENT_REV_RECEIVED;
        // data fields
        message_event->param0 = atoi(strtok(NULL, ","));
    }
    else if (strcmp(messageID, "SHO") == 0) {
        // message type
        message_event->type = BB_EVENT_SHO_RECEIVED;
        // data fields
        message_event->param0 = atoi(strtok(NULL, ","));
        message_event->param1 = atoi(strtok(NULL, ","));
    }
    else if (strcmp(messageID, "RES") == 0) {
        // message type
        message_event->type = BB_EVENT_RES_RECEIVED;
        // data fields
        message_event->param0 = atoi(strtok(NULL, ","));
        message_event->param1 = atoi(strtok(NULL, ","));
        message_event->param2 = atoi(strtok(NULL, ","));
    }
    else {
        // message type
        message_event->type = BB_EVENT_ERROR;
        return STANDARD_ERROR;
    }
    
    // check if there is any token left
    char * token = strtok(NULL, "\n");
    
    if (token != NULL) {
        return STANDARD_ERROR;
    }
    
    return SUCCESS;
}

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_*,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. 
 * 
 * The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * 
 * @param message            The character array used for storing the output. 
 *                              Must be long enough to store the entire string,
 *                              see MESSAGE_MAX_LEN.
 * @param message_to_encode  A message to encode
 * @return                   The length of the string stored into 'message_string'.
                             Return 0 if message type is MESSAGE_NONE.
 */
int Message_Encode(char *message_string, Message message_to_encode) {
    // char payload[MAX_PAYLOD_LEN + 1]
    // switch depending on the message to encode type CHA ACC REV SHO RES
    //     sprintf payload to the corresponding types
    // sprintf(message_string, MESSAGE_TEMPLATE, payload, CalculateChecksum(payload))
    // return the length of message_string
    
    char payload[MESSAGE_MAX_PAYLOAD_LEN + 1];
    switch(message_to_encode.type) {
        case MESSAGE_CHA:
            sprintf(payload, PAYLOAD_TEMPLATE_CHA, message_to_encode.param0);
            sprintf(message_string, MESSAGE_TEMPLATE, payload, Message_CalculateChecksum(payload));
            break;
        
        case MESSAGE_ACC:
            sprintf(payload, PAYLOAD_TEMPLATE_ACC, message_to_encode.param0);
            sprintf(message_string, MESSAGE_TEMPLATE, payload, Message_CalculateChecksum(payload));
            break;

        case MESSAGE_REV:
            sprintf(payload, PAYLOAD_TEMPLATE_REV, message_to_encode.param0);
            sprintf(message_string, MESSAGE_TEMPLATE, payload, Message_CalculateChecksum(payload));
            break;
            
        case MESSAGE_SHO:
            sprintf(payload, PAYLOAD_TEMPLATE_SHO, message_to_encode.param0, message_to_encode.param1);
            sprintf(message_string, MESSAGE_TEMPLATE, payload, Message_CalculateChecksum(payload));
            break;

        case MESSAGE_RES:
            sprintf(payload, PAYLOAD_TEMPLATE_RES, message_to_encode.param0, message_to_encode.param1, message_to_encode.param2);
            sprintf(message_string, MESSAGE_TEMPLATE, payload, Message_CalculateChecksum(payload));
            break;
            
        case MESSAGE_NONE:
            return 0;
            break;
            
        case MESSAGE_ERROR:
            return 0;
            break;   
    }
    return strlen(message_string);
}

/**
 * Message_Decode reads one character at a time.  If it detects a full NMEA message,
 * it translates that message into a BB_Event struct, which can be passed to other 
 * services.
 * 
 * @param char_in - The next character in the NMEA0183 message to be decoded.
 * @param decoded_message - a pointer to a message struct, used to "return" a message
 *                          if char_in is the last character of a valid message, 
 *                              then decoded_message
 *                              should have the appropriate message type.
 *                          if char_in is the last character of an invalid message,
 *                              then decoded_message should have an ERROR type.
 *                          otherwise, it should have type NO_EVENT.
 * @return SUCCESS if no error was detected
 *         STANDARD_ERROR if an error was detected
 * 
 * note that ANY call to Message_Decode may modify decoded_message.
 */
int Message_Decode(unsigned char char_in, BB_Event * decoded_message_event) {
    // define a decode state, waiting, processing payload, processing checksum
    // local variables for payload, payload index, checksum_string, checksum_index
    // switch depending on decode state
    //      waiting state
    //           reset indexes
    //           check if char_in is $
    //                change state to processing payload
    //           return SUCCESS
    //      processing payload
    //           check if char_in == $ || char_in == \n
    //                change state
    //                message_event->type = BB_EVENT_ERROR
    //                message_event->parm0 = BB_ERROR
    //                return STANDARD_ERROR
    //           check if char_in == *
    //                change state to checking checksum
    //                payload[current_index] = '\0'
    //                return SUCCESS
    //           otherwise
    //                assign the next index in the payload to char_in
    //                check index is more than max payload length
    //                     reset state to first one
    //                     set decoded message_event with errors as before
    //                     return STANDARD_ERROR
    //                return SUCCESS
    //      processing checksum
    //           check if char_in == hex character (0-9 or A-F)
    //                assign the next index in the checksum to char_in
    //                check index is more than max checksum length
    //                     reset state to first one
    //                     set decoded message_event with errors as before
    //                     return STANDARD_ERROR
    //                return SUCCESS
    //           check if char_in == \n
    //                change state to first
    //                parse payload and checksum with ParseMessage()
    //                if payload is valid && checksum matches payload
    //                     generate MESSAGE_RECEIVED event
    //                     reset state to first one
    //                else
    //                     generate ERROR event
    //                     reset state to first one
    //                return SUCCESS
    //           otherwise
    //                change state
    //                message_event->type = BB_EVENT_ERROR
    //                message_event->parm0 = BB_ERROR
    //                return STANDARD_ERROR
    //      default return an error
    
    static char payload[MESSAGE_MAX_PAYLOAD_LEN + 1];
    static int payload_index;
    static char checksum_string[MESSAGE_CHECKSUM_LEN + 1];
    static int checksum_index;
    
    decodeState state = WAITING;
    
    switch(state) {
        // waiting for start delimeter
        case WAITING:
            payload_index = 0;
            checksum_index = 0;
            if (char_in == '$') {
                state = PROCESSING_PAYLOAD;
            }
            break;
        // recording payload
        case PROCESSING_PAYLOAD:
            if (char_in == '$' || char_in == '\n') {
                state = WAITING;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            }
            else if (char_in == '*') {
                state = PROCESSING_CHECKSUM;
                payload[payload_index] = '\0';
                return SUCCESS;
            }
            else {
                payload_index += 1;
                payload[payload_index] = char_in;                 
                if (payload_index > MESSAGE_MAX_PAYLOAD_LEN + 1) {
                    state = WAITING;
                    decoded_message_event->type = BB_EVENT_ERROR;
                    decoded_message_event->param0 = BB_ERROR_PAYLOAD_LEN_EXCEEDED;
                    return STANDARD_ERROR;
                }
                return SUCCESS;
            }
            break;
        // recording checksum
        case PROCESSING_CHECKSUM:
            if ((char_in > '0' || char_in < '9') && (char_in > 'A' || char_in < 'F')) {
                checksum_index += 1;
                checksum_string[checksum_index] = char_in;
                if (checksum_index > MESSAGE_CHECKSUM_LEN + 1) {
                    state = WAITING;
                    decoded_message_event->type = BB_EVENT_ERROR;
                    decoded_message_event->param0 = BB_ERROR_CHECKSUM_LEN_EXCEEDED;
                    return STANDARD_ERROR;
                }
                return SUCCESS; 
            }
            else if (char_in == '\n') {
                state = WAITING;
                if (Message_ParseMessage(payload, checksum_string, decoded_message_event) == SUCCESS) {
                    state = WAITING;
                    return SUCCESS;
                }
                else {
                    decoded_message_event->type = BB_EVENT_ERROR;
                    state = WAITING;
                    return STANDARD_ERROR;
                }
            }
            else {
                state = WAITING;
                decoded_message_event->type = BB_EVENT_ERROR;
                decoded_message_event->param0 = BB_ERROR_INVALID_MESSAGE_TYPE;
                return STANDARD_ERROR;
            }
            break;
        // default
        default: 
            return STANDARD_ERROR;
            break;
    }
}
