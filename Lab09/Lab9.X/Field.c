/* 
 * File:   Field.c
 * Author: barry
 *
 * Created on June 2, 2021, 1:10 PM
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include "Field.h"
#include "BOARD.h"
#include "Negotiation.h"
/*
 * 
 */
static int row = 0;

/**
 * This function is optional, but recommended.   It prints a representation of both
 * fields, similar to the OLED display.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     SquareStatus.
 */
void FieldPrint_UART(Field *own_field, Field * opp_field) {
    int i;
    int j;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            printf(" %d ", own_field -> grid[i][j]);
        }
        printf("   |   ");
        for (j = 0; j < FIELD_COLS; j++) {
            printf(" %d ", opp_field -> grid[i][j]);
        }
        printf("\n");
    }
}

/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus (
 * FIELD_SQUARE_EMPTY for your own field, FIELD_SQUARE_UNKNOWN for opponent's).
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be filled when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field * opp_field) {
    int i;
    int j;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            own_field->grid[i][j] = 0;
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
        }
    }
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
    row = 0;
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    if (row >= FIELD_ROWS || col >= FIELD_COLS) {
        return FIELD_SQUARE_INVALID;
    }
    return f->grid[row][col];
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col, SquareStatus p) {
    SquareStatus PreviousPosition = f->grid[row][col];
    f->grid[row][col] = p;
    return PreviousPosition;
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. 
 * 
 * All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 * 
 * In addition, this function should update the appropriate boatLives parameter of the field.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */

SquareStatus checkEmpty(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatSize size) {
    int i;
    switch (dir) {
        case FIELD_DIR_SOUTH:
            for (i = 0; i < size; i++) {
                if ((row + i) >= FIELD_ROWS) {
                    return FIELD_SQUARE_INVALID;
                }
                if (own_field->grid[row + i][col] != FIELD_SQUARE_EMPTY) {
                    return FIELD_SQUARE_INVALID;
                }
            }

            break;

        case FIELD_DIR_EAST:
            for (i = 0; i < size; i++) {
                if ((col + i) >= FIELD_COLS) {
                    return FIELD_SQUARE_INVALID;
                }
                if (own_field->grid[row][col + i] != FIELD_SQUARE_EMPTY) {
                    return FIELD_SQUARE_INVALID;
                }
            }

            break;

        default:
            break;
    }

    return FIELD_SQUARE_EMPTY;
}

uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    BoatSize size;
    SquareStatus check;
    SquareStatus status;
    int j;

    switch (boat_type) {
        case FIELD_BOAT_TYPE_SMALL:
            size = FIELD_BOAT_SIZE_SMALL;
            status = FIELD_SQUARE_SMALL_BOAT;
            break;
        case FIELD_BOAT_TYPE_MEDIUM:
            size = FIELD_BOAT_SIZE_MEDIUM;
            status = FIELD_SQUARE_MEDIUM_BOAT;
            break;

        case FIELD_BOAT_TYPE_LARGE:
            size = FIELD_BOAT_SIZE_LARGE;
            status = FIELD_SQUARE_LARGE_BOAT;
            break;
        case FIELD_BOAT_TYPE_HUGE:
            size = FIELD_BOAT_SIZE_HUGE;
            status = FIELD_SQUARE_HUGE_BOAT;
            break;
        default:
            break;
    }



    check = checkEmpty(own_field, row, col, dir, size);
    if (check != FIELD_SQUARE_EMPTY) {
        return STANDARD_ERROR;
    }


    if (dir == FIELD_DIR_SOUTH) {
        for (j = 0; j < size; j++) {
            own_field->grid[row + j][col] = status;
        }
    } else {
        for (j = 0; j < size; j++) {
            own_field->grid[row][col + j] = status;
        }
    }
    if (size == FIELD_BOAT_SIZE_SMALL) {
        own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    } else if (size == FIELD_BOAT_SIZE_MEDIUM) {
        own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    } else if (size == FIELD_BOAT_SIZE_LARGE) {
        own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    } else {
        own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
    }

    return SUCCESS;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess) {
    SquareStatus temp;
    temp = own_field->grid[opp_guess->row][opp_guess->col];

    switch (own_field->grid[opp_guess->row][opp_guess->col]) {

        case FIELD_SQUARE_EMPTY:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_MISS;
            opp_guess->result = RESULT_MISS;
            break;

        case FIELD_SQUARE_SMALL_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->smallBoatLives--;
            if (own_field->smallBoatLives == 0)
                opp_guess->result = RESULT_SMALL_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;

            break;

        case FIELD_SQUARE_MEDIUM_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->mediumBoatLives--;
            if (own_field->mediumBoatLives == 0)
                opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
            else

                break;

        case FIELD_SQUARE_LARGE_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->smallBoatLives--;
            if (own_field->largeBoatLives == 0)
                opp_guess->result = RESULT_LARGE_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;

            break;

        case FIELD_SQUARE_HUGE_BOAT:
            own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
            own_field->smallBoatLives--;
            if (own_field->hugeBoatLives == 0)
                opp_guess->result = RESULT_HUGE_BOAT_SUNK;
            else
                opp_guess->result = RESULT_HIT;

            break;

        default:
            break;
    }


    return temp;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess) {
    SquareStatus temp;
    temp = opp_field->grid[own_guess->row][own_guess->col];

    switch (own_guess->result) {
        case RESULT_HIT:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            break;

        case RESULT_MISS:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
            break;

        case RESULT_SMALL_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            opp_field->smallBoatLives = 0;
            break;

        case RESULT_MEDIUM_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            opp_field->mediumBoatLives = 0;
            break;

        case RESULT_LARGE_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            opp_field->largeBoatLives = 0;
            break;

        case RESULT_HUGE_BOAT_SUNK:
            opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
            opp_field->hugeBoatLives = 0;

            break;
    }

    return temp;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f) {
    int returnFlags = 0;
    if (f->smallBoatLives <= 0) {
        returnFlags |= FIELD_BOAT_STATUS_SMALL;
    }
    if (f->mediumBoatLives <= 0) {
        returnFlags |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->largeBoatLives <= 0) {
        returnFlags |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->hugeBoatLives <= 0) {
        returnFlags |= FIELD_BOAT_STATUS_HUGE;
    }
    return returnFlags;
}

/**
 * This function is responsible for placing all four of the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field *own_field) {
    int col;
    int row;
    int dir;
    uint8_t check;

    col = rand() % FIELD_COLS;
    row = rand() % FIELD_COLS;
    dir = rand() & 0x01;

    check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_HUGE);
    while (check == STANDARD_ERROR) {
        col = rand() % FIELD_COLS;
        row = rand() % FIELD_COLS;
        dir = rand() & 0x01;
        check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_HUGE);

    }

    col = rand() % FIELD_COLS;
    row = rand() % FIELD_COLS;
    dir = rand() & 0x01;

    check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_LARGE);
    while (check == STANDARD_ERROR) {
        col = rand() % FIELD_COLS;
        row = rand() % FIELD_COLS;
        dir = rand() & 0x01;
        check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_LARGE);

    }

    col = rand() % FIELD_COLS;
    row = rand() % FIELD_COLS;
    dir = rand() & 0x01;

    check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_MEDIUM);
    while (check == STANDARD_ERROR) {
        col = rand() % FIELD_COLS;
        row = rand() % FIELD_COLS;
        dir = rand() & 0x01;
        check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_MEDIUM);

    }

    col = rand() % FIELD_COLS;
    row = rand() % FIELD_COLS;
    dir = rand() & 0x01;

    check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_SMALL);
    while (check == STANDARD_ERROR) {
        col = rand() % FIELD_COLS;
        row = rand() % FIELD_COLS;
        dir = rand() & 0x01;
        check = FieldAddBoat(own_field, row, col, dir, FIELD_BOAT_TYPE_SMALL);

    }

    return SUCCESS;
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field *opp_field) {
    int col;
    int row;
    int unknowCheck = 0;
    int i;
    for (i = 0; i < FIELD_COLS; i++) {
        if (opp_field->grid[row][i] == FIELD_SQUARE_UNKNOWN) {
            unknowCheck += 1;
            break;
        }
    }
    if (unknowCheck == 0)
        row++;

    col = rand() % FIELD_COLS;
    while (opp_field->grid[row][col] == FIELD_SQUARE_EMPTY || opp_field->grid[row][col] == FIELD_SQUARE_HIT) {
        col = rand() % FIELD_COLS;
    }
    GuessData OwnGuess = {row, col};
    return OwnGuess;
}

