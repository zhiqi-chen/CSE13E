/* 
 * File:   Player.c
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
#include "UNIXBOARD.h"

// Player Inventory
static uint8_t inventoryIndex = 0;
static uint8_t inventory[INVENTORY_SIZE] = {0};

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item) {
    // check if inventory is full
    //   if full, return standard error
    
    // add item to inventory at given index
    // increment index
    // return success
    
    for (inventoryIndex=0; inventoryIndex<INVENTORY_SIZE; inventoryIndex++) {
        if (inventory[inventoryIndex] == 0) {
            inventory[inventoryIndex] = item;
            inventoryIndex++;
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item) {
    // use a for loop to look if necessary item is in array
    // return success when found (stop iterating)
    
    // can't find it? return standard error
    
    for (inventoryIndex=0; inventoryIndex<INVENTORY_SIZE; inventoryIndex++) {
        if (inventory[inventoryIndex] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;
}

