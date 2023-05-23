#include "pseudo_malloc.h"
#include "buddy_allocator.h"

void * pseudo_malloc(size_t size){
    //TODO

    /*from man:
        If size is 0 malloc returns NULL
        On error returns NULL*/
}

void pseudo_free(void * ptr){
    //TODO

    /*from man:
        frees memory space pointed by *ptr
        which must have been returned by a previous
        malloc().
        if free(ptr) has already been called -> undefined behaviour
        if ptr is NULL no operation occurs
        on error returns NULL*/
}