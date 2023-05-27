#include "pseudo_malloc.h"
#include "my_buddy_allocator.h"
#include <stdio.h>

char memory[MEMORY_SIZE];
char buffer[BUFFER_SIZE];
uint8_t bitmap_arr[((1 << MAX_LEVELS) +1)];

MyBuddyAllocator alloc;

int main(int argc, char *argv){

    // Initialize buddy allocator
    MyBuddyAllocator_init(&alloc, bitmap_arr, memory, buffer);

    printf("[MAIN]: Initialized BuddyAllocator:\n\tBitMap length in bits ---> %d \n", (&alloc)->bitmap.num_bits);

    printf("[MAIN]: Beginning of memory: %p\n", memory);

    char *ptr1 = (char *)pseudo_malloc(256);
    char *ptr2 = (char *)pseudo_malloc(256);
    char *ptr3 = (char *)pseudo_malloc(256);
    char *ptr4 = (char *)pseudo_malloc(256);
    printf("[MAIN]: Allocated pointers:\n\tptr1: %p\n\tptr2: %p\n\tptr3: %p\n\tptr3: %p\n", ptr1, ptr2, ptr3, ptr4);
    
    char *ptrs[1023];
    for (int i = 0; i < 1023; ++i){
        ptrs[i] = (char *)pseudo_malloc(1024);
        printf("Allocated ptr %p\n", ptrs[i]);
    }

    /*void *ptr1 = pseudo_malloc(512);
    void *ptr2 = pseudo_malloc(256);
    pseudo_free(ptr2);
    void *ptr3 = pseudo_malloc(257);
    void *ptr4 = pseudo_malloc(257);
    printf("[MAIN]: Allocated pointers:\n\tptr1: %p\n\tptr2: %p\n\tptr3: %p\n\tptr4: %p\n", ptr1, ptr2, ptr3, ptr4);
*/
    char *ptr = (char *)pseudo_malloc(256);
    printf("Allocated ptr %p\n", ptr);

    //pseudo_free(ptr);
    printf("End of memory %p\n", memory + MEMORY_SIZE - 1024);

    //printf("Exceeding meomry returns null?\n");

    //char * oor = (char *)pseudo_malloc(256);
    //printf("Allocated ptr %p\n", oor);
    printBitMap(&alloc.bitmap);

}