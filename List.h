
/**
 * file: List.h
 *
 * Purpose: prototypes for the List
 *
 */

 // Definition Element the List Entry Structure
struct SList
{
    void * Data;
    struct SList *Next;
    struct SList *Prev;
}; typedef struct SList SLIST;

 // Definition of Header Structure
struct SList_Header
{
    int Len;
    SLIST *First, *Last;
}; typedef struct SList_Header SLIST_HEADER;

// Method to create a List
extern SLIST_HEADER *createSList(void);

// Adds an element to the List
extern SLIST *insertLast(SLIST_HEADER*, void*);

// Release the memory allocation of a List
void freeSLIST(SLIST * lst);

// Release the memory allocation of an entire list
void deleteList(SLIST_HEADER * head);
