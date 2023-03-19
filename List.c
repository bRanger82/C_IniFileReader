/**
 * File: SList.c
 *
 * Purpose: Contains the methods to work with the list.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"

static SLIST * makeSListNodeLast (void * userData)
{
   SLIST * newPtr;

   if ((newPtr=(SLIST *) malloc(sizeof(SLIST))) != NULL)
   {
      newPtr->Data = userData;
      newPtr->Next = NULL;
   }
   return (newPtr);
}

/**
*  Global Operations
**/

// Frees the allocated memory for a list object
void freeSLIST(SLIST * lst)
{
    if (NULL == lst)
    {
        return;
    }
    SLIST * nextItem = lst->Next;
    free(lst);
    freeSLIST(nextItem);
}

// Frees the allocated memory for the entire List including the header
void deleteList(SLIST_HEADER * head)
{
    if (NULL == head)
    {
        return;
    }
    freeSLIST(head->First);
    free(head);
}

// Creates the Header element (including memory allocation)
SLIST_HEADER * createSList(void)
{
    SLIST_HEADER * p = (SLIST_HEADER *) malloc(sizeof(SLIST_HEADER));
    if (NULL == p) // Problem bei Speicher-Allokierung
    {
        return p;
    }

    // Standardwerte setzen
    p->Len = 0;
    p->First = NULL;
    p->Last = NULL;

    return p;
}

/* -- Adds an element to the list.
      The added element will be added to the end of the list.
    Parameters:
       - aList: List of items which userData will be added
       - userData: Item which is added to the list
    Returns:
       - If successful it returns a pointer to the new element
       - If NOT successful it returns NULL
*/
SLIST * insertLast(SLIST_HEADER* aList, void* userData)
{
    SLIST * newPtr = NULL;

    if (NULL == aList)
    {
        printf("ERROR: Cannot insert item as List is not initialized yet!\n");
        return NULL;
    }

    newPtr = makeSListNodeLast(userData);
    if (NULL == newPtr)
    {
        printf("ERROR: unable to add an entry to the list!\n");
        return NULL;
    }


    if (aList->Len == 0)
    {
        aList->Last = newPtr;
        aList->First = newPtr;
    } else
    {
        aList->Last->Next = newPtr;
        aList->Last = newPtr;
    }

    aList->Len++;

    return (newPtr);
}
