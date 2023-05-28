#include "my_buddy_allocator.h"
#include <assert.h>
#include <math.h>
#include "pseudo_malloc.h"

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory, char *buffer){
    
    int number_of_bits = (1 << MAX_LEVELS + 1) - 1;
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, bitmap);

    buddyAllocator->memory = memory;
    buddyAllocator->buffer = buffer;
    buddyAllocator->num_levels = MAX_LEVELS;

    // initialize the number of blocks at each level
    for(int i = 0; i <= MAX_LEVELS; ++i){
        buddyAllocator->num_nodes[i] = (1 << i);
    }


    PoolAllocatorResult init_result = PoolAllocator_init(&buddyAllocator->items, sizeof(MyBuddyItem), (1 << MAX_LEVELS) +1, buddyAllocator->buffer, (sizeof(MyBuddyItem) + sizeof(int))* ((1 << MAX_LEVELS)+1));
    printf("%s\n", PoolAllocator_strerror(init_result));
}

// Returns a new BuddyItem
MyBuddyItem *createBuddyItem(MyBuddyAllocator *alloc, int idx){
    MyBuddyItem *item = (MyBuddyItem *)PoolAllocator_getBlock(&alloc->items);
    item->idx = idx;
    item->level = levelIdx(idx);
    item->size = 1 << (alloc->num_levels - item->level);
    item->start = alloc->memory + (startIdx(item->idx) * item->size);
    return item;
}

// Releases a buddy item
void destroyBuddyItem(MyBuddyAllocator *alloc, MyBuddyItem *item){
    PoolAllocatorResult release_result = PoolAllocator_releaseBlock(&alloc->items, item);
}

void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int size){
    int level = MAX_LEVELS - ceil(log2(size));
    int num_nodes = buddyAllocator->num_nodes[level];

    // Find the first available block of the specified level
    size_t offset = 0;

    while (offset < num_nodes && BitMap_bit(&buddyAllocator->bitmap, (1 << level) + offset)){
        offset++;
    }

    // If no available block is found return NULL --insert some error message
    if (offset >= num_nodes){
        printf("[BuddyMalloc]: No more memory available, returning NULL\n");
        return NULL;
    }

    size_t available_bit = (1 << level) + offset;

    // Mark the block as allocated in the BitMap
    BitMap_setBit(&buddyAllocator->bitmap, available_bit, 1);

    // Set ALL the upper tree to allocated recursively
    BitMap_ParentSetBit(&buddyAllocator->bitmap, available_bit, 1);

    // Then set to allocated all the bits corresponding to subtree
    BitMap_SetSubTree(&buddyAllocator->bitmap, available_bit, 1);

    // Now that tree is correctly set, get the buddy Item
    MyBuddyItem *mem = createBuddyItem(buddyAllocator, available_bit);

    MyBuddyItem **target = (MyBuddyItem **)(mem->start);
    *target = mem;
    return (void *)mem->start;
}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

   if (ptr == NULL) return;

   // Retrieve buddy from the system
   char *p = (char *)ptr;
   MyBuddyItem **buddy_ptr = (MyBuddyItem **)p;
   MyBuddyItem *buddy = (MyBuddyItem *)buddy_ptr;
   buddy = *buddy_ptr;
   assert(buddy->start == p);

   // Set the bit in the bitmap back to 0
   BitMap_setBit(&buddyAllocator->bitmap, buddy->idx, 0);

   // If the buddy is also 0 set the parent to 0 and do this recursively
   BitMap_ParentSetBit(&buddyAllocator->bitmap, buddy->idx, 0);

   // And set its subtree back to 0
   BitMap_SetSubTree(&buddyAllocator->bitmap, buddy->idx, 0);

   // Now destroy the item
   PoolAllocatorResult result = PoolAllocator_releaseBlock(&buddyAllocator->items, buddy);
   printf("%s\n", PoolAllocator_strerror(result));

   return;
}