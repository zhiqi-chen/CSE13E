// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


//CSE013E Support Library
#include <stdint.h>



// User libraries
#include "UNIXBOARD.h"
#include "Player.h"
#include "Game.h"


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****
static char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
#define NORTH 'n'
#define EAST 'e'
#define SOUTH 's'
#define WEST 'w'
#define QUIT 'q'

// **** Declare any function prototypes here ****
// helper function
void displayRoom() {
    
    uint8_t roomExits = GameGetCurrentRoomExits();
        
    // print title
    GameGetCurrentRoomTitle(title);
    printf("title: %s\n", title);
    
        
    // print description
    GameGetCurrentRoomDescription(desc);
    printf("description: %s\n", desc);
        
    printf("Available directions: ");
    if (roomExits & GAME_ROOM_EXIT_NORTH_EXISTS) {
        printf("n ");
    }
    if (roomExits & GAME_ROOM_EXIT_EAST_EXISTS) {
        printf("e ");
    }
    if (roomExits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        printf("s ");
    }
    if (roomExits & GAME_ROOM_EXIT_WEST_EXISTS) {
        printf("w ");
    }
    printf("\nPlease choose your direction or quit by entering n, e, s, w, or q: ");
}


int main()
{
   
    

    /******************************** Your custom code goes below here ********************************/

    uint8_t dir = 0;
    uint8_t roomExits = 0;
    
    if (GameInit() == STANDARD_ERROR) {
        printf("Failed to Load Starting Room\n");
    }
    
    // function to display the room
    displayRoom();
    
    while (1) {
        
        dir = getchar();
        
        roomExits = GameGetCurrentRoomExits();
        
        switch (dir) {
            // north
            case NORTH:
                if (roomExits & GAME_ROOM_EXIT_NORTH_EXISTS) {
                    if (GameGoNorth()) {
                        // do something reference code does nothing
                        displayRoom();
                    }
                    else {
                        FATAL_ERROR();
                    }
                }
            break;
            // east
            case EAST:
                if (roomExits & GAME_ROOM_EXIT_EAST_EXISTS) {
                    if (GameGoEast()) {
                        // do something reference code does nothing
                        displayRoom();
                    }
                    else {
                        FATAL_ERROR();
                    }
                }
            break;
            // south
            case SOUTH:
                if (roomExits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
                    if (GameGoSouth()) {
                        // do something reference code does nothing
                        displayRoom();
                    }
                    else {
                        FATAL_ERROR();
                    }
                }
            break;
            // west
            case WEST:
                if (roomExits & GAME_ROOM_EXIT_WEST_EXISTS) {
                    if (GameGoWest()) {
                        // do something reference code does nothing
                        displayRoom();
                    }
                    else {
                        FATAL_ERROR();
                    }
                }
            break;
            // exit
            case QUIT:
                printf("exiting...\n");
                FATAL_ERROR();
            break; 
            // respond only to valid directions (n,e,s,w) or q and ignore all other inputs
            // set invalid direction entered as default
            default:
                if (dir != '\n') {
                    printf("Invalid direction entered\n");
                    displayRoom();
                }
            break;
        }
        

    }

    /**************************************************************************************************/
}

