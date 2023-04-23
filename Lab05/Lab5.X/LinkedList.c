/* 
 * File:   LinkedList.c
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

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *             pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data) {
    ListItem *new = malloc(sizeof (ListItem));
    if (new == NULL) {
        return NULL;
    }
    new->data = data;
    new->nextItem = 0;
    new->previousItem = 0;
    return new;
}

/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, CreateAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    ListItem *new = LinkedListNew(data);
    if (new == NULL) {
        return NULL;
    }
    if (item == NULL) {
        return new;
    } else {
        if (item->nextItem == NULL) {
            item->nextItem = new;
            new->previousItem = item;
            new->nextItem = NULL;
            return new;
        } else {
            new->nextItem = item->nextItem;
            new->previousItem = item;
            item->nextItem->previousItem = new;
            item->nextItem = new;
            return new;
        }
    }
}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item) {
    if (item == NULL) {
        return NULL;
    }
    char *remove = item->data;
    if (item->previousItem != NULL) {
        item->previousItem->nextItem = item->nextItem;
    }
    if (item->nextItem != NULL) {
        item->nextItem->previousItem = item->previousItem;
    }
    free(item);
    return remove;
}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list) {
    int counter = 0;
    ListItem *list1 = list;
    ListItem *list2 = list;
    if (list == NULL) {
        return 0;
    }
    // count from current to head
    while (list1->previousItem != NULL) {
        list1 = list1->previousItem;
        ++counter;
    }
    // count from current to tail
    while (list2->nextItem != NULL) {
        list2 = list2->nextItem;
        ++counter;
    }
    return counter + 1;
}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return A pointer to the first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list) {
    if (list == NULL) {
        return NULL;
    }
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}

/**
 * This function operates identically to LinkedListGetFirst(), but returns
 * the tail of a linked list.
 * 
 * @param list An element in a list.
 * @return A pointer to the last element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetLast(ListItem *list) {
    if (list == NULL) {
        return NULL;
    }
    while (list->nextItem != NULL) {
        list = list->nextItem;
    }
    return list;
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    if (firstItem == NULL || secondItem == NULL) {
        return STANDARD_ERROR;
    } else {
        char *first = firstItem->data;
        char *second = secondItem->data;
        firstItem->data = second;
        secondItem->data = first;
        return SUCCESS;
    }
}

/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list) {
    if (list == NULL) {
        return STANDARD_ERROR;
    } else {
        // move to head
        list = LinkedListGetFirst(list);
        // start printing
        printf("[");
        while (list->nextItem != NULL) {
            printf("%s, ", list->data);
            list = list->nextItem;
        }
        if (list->nextItem == NULL) {
            printf("%s", list->data);
        }
        printf("]");
        // end printing
        return SUCCESS;
    }
}
