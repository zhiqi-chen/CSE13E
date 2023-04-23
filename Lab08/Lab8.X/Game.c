/* 
 * File:   Game.c
 * Author: Zhiqi Chen (zchen287@ucsc.edu)
 *
 * Created on May 23, 2021, 1:17 AM
 */

// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


//CSE013E Support Library
#include <stdint.h>

// User libraries
#include "Player.h"
#include "Game.h"
#include "UNIXBOARD.h"

// **** Define any global or external variables here ****


static struct {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1]; // "TITLE" + "\0"
    // description
    char description[GAME_MAX_ROOM_DESC_LENGTH + 1]; // "DESC" + "\0"
    // uint8_t fields for north, east, south, west
    uint8_t north;
    uint8_t east;
    uint8_t south;
    uint8_t west;
} currentRoom={};

// **** Put any helper functions here ****
static int LoadRoom(uint16_t roomNumber);
static int titleLength;
static int itemReqLength;
static int descLength;
static int itemsConatainedLength;



/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void) {
    if (currentRoom.north != 0) {
        return LoadRoom(currentRoom.north);
    }
    
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void) {
    if (currentRoom.east != 0) {
        return LoadRoom(currentRoom.east);
    }
    
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void) {
    if (currentRoom.south != 0) {
        return LoadRoom(currentRoom.south);
    }
    
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void) {
    if (currentRoom.west != 0) {
        return LoadRoom(currentRoom.west);
    }
    
    return STANDARD_ERROR;
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void) {
    return LoadRoom(STARTING_ROOM);
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title) {
    // use strncpy ... title + 1
    // return strlen
    strncpy(title, currentRoom.title, GAME_MAX_ROOM_TITLE_LENGTH + 1);
    return strlen(title + 1);
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc) {
    strncpy(desc, currentRoom.description, GAME_MAX_ROOM_DESC_LENGTH + 1);
    return strlen(desc + 1);
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void) {
    
    uint8_t exits = 0;
    
    // north
    if (currentRoom.north != 0) {
        exits = exits | GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    // east
    if (currentRoom.east != 0) {
        exits = exits | GAME_ROOM_EXIT_EAST_EXISTS;
    }
    // south
    if (currentRoom.south != 0) {
        exits = exits | GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    // west
    if (currentRoom.west != 0) {
        exits = exits | GAME_ROOM_EXIT_WEST_EXISTS;
    }
    
    return exits;
}




static int LoadRoom(uint16_t roomNumber) {
    FILE *room;
    
    char roomFileName[50];
    
    sprintf(roomFileName, "RoomFiles/room%d.txt", roomNumber);
    
    if ((room = fopen(roomFileName, "rb")) == NULL) {
        return STANDARD_ERROR;
    }
    
    
    // if not NULL
    
    fgetc(room); // skip r
    fgetc(room); // skip p
    fgetc(room); // skip g
    
    // **** title ****
    titleLength = fgetc(room);
    // check if EOP || 0 || NULL
    if (titleLength == EOF || titleLength == 0 || fgets(currentRoom.title, titleLength + 1, room) == NULL) { // fgets gets the title
        fclose(room);
        return STANDARD_ERROR;
    }
    
    currentRoom.title[titleLength + 1] = '\0';
    
    uint8_t check = FALSE;
    do {        
        // **** item req ****
        itemReqLength = fgetc(room);
        
        if (itemReqLength == 0) {
            check = TRUE;
        }
        
         // check item        
        int itemReqIndex;
        for (itemReqIndex=0; itemReqIndex < itemReqLength; itemReqIndex++) {
            uint8_t item = fgetc(room);
            // find in inventory
            if (FindInInventory(item) == SUCCESS) {
                check = TRUE;
            }
        }
               
        // **** description ****
        descLength = fgetc(room);
        
        // check if EOP || 0 || NULL
        if (descLength == EOF || descLength == 0 || fgets(currentRoom.description, descLength + 1, room) == NULL) { // fgets gets the description
            fclose(room);
            return STANDARD_ERROR;
        }
               
        // **** items contained ****
        itemsConatainedLength = fgetc(room); 
        

        
        // check item
        int itemsConatainedIndex;
        for (itemsConatainedIndex=0; itemsConatainedIndex < itemsConatainedLength; itemsConatainedIndex++) {
            uint8_t item = fgetc(room);
            // add to inventory
            if (AddToInventory(item) == STANDARD_ERROR) {
                fclose(room);
                return STANDARD_ERROR;
            }
        }
               
        // **** exits ****
        currentRoom.north = fgetc(room); // north exit
        currentRoom.east = fgetc(room); // east exit
        currentRoom.south = fgetc(room); // south exit
        currentRoom.west = fgetc(room); // west exit
                
    } while (check == FALSE);
    
    fclose(room);
    return SUCCESS;
    
}