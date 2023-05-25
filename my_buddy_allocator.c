#include "my_buddy_allocator.h"

// Functions for bitmap indices

int levelIdx(size_t idx) { return (int)floor(log2(idx)); };

int buddyIdx(int idx) {
  if (idx & 0x1) {
    return idx - 1;
  }
  return idx + 1;
}

int parentIdx(int idx) { return idx / 2; }

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory){
    
    int number_of_bits = 1 << MAX_LEVELS +1;
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, bitmap);

    buddyAllocator->buffer = memory;

    // initialize the number of blocks at each level
    for(int i = 0; i < MAX_LEVELS; ++i){
        buddyAllocator->num_nodes[i] = (1 << MAX_LEVELS - i);
    }
}

// Returns a BuddyItem
// Parameters could be less -> TODO
MyBuddyItem *createBuddyItem(MyBuddyAllocator *alloc, int idx){
    MyBuddyItem *item = (MyBuddyItem *)PoolAllocator_getBlock(&alloc->items);
    item->idx = idx;
    item->level = levelIdx(idx);
    item->size = alloc->bitmap.buffer_size / alloc->num_nodes[item->level]; // Memory at level x is divided into num_nodes[x] parts
    item->start = alloc->buffer + ((1 << item->level) + (idx - (1 << levelIdx(idx))));
    return item;
}

void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int size){
    int level = ceil(log2(size));
    int num_nodes = buddyAllocator->num_nodes[level];

    // Find the first available block of the specified level
    // Check the bounds in the level, so from 2^level to (2^level+1)-1
    size_t start_index = 1 << level;
    while (start_index <= num_nodes && BitMap_bit(&buddyAllocator->bitmap, start_index)){
        start_index++;
    }

    // If no available block is found return NULL
    if (start_index > ((1 << (level +1)) -1)) return NULL;

    size_t available_bit = start_index;

    // Mark the block as allocated in the BitMap
    BitMap_setBit(&buddyAllocator->bitmap, start_index, 1);

    // If the buddy is also allocated set the parent to allocated
    if (BitMap_bit(&buddyAllocator->bitmap, buddyIdx(start_index))){
        BitMap_setBit(&buddyAllocator->bitmap, parentIdx(start_index), 1);
    }

    // Then set to allocated all the bits corresponding to subtree
    start_index*=2;

    while (start_index < &buddyAllocator->bitmap.num_bits){
        BitMap_setBit(&buddyAllocator->bitmap, start_index, 1);
        BitMap_setBit(&buddyAllocator->bitmap, buddyIdx(start_index), 1);
        start_index*=2;
    }

    // OR actually use a slab allocator and create BuddyItems structs to return that contain 
    // pointers to the memory region associated to them (which is fixed btw since on the same 
    // level there are fixed size memory regions)

    // Create the butty Item
    MyBuddyItem *mem = createBuddyItem(buddyAllocator, available_bit);
    return (void *)mem;
}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

   if (ptr == NULL) return;

   // Retrieve buddy from the system
   char *p = (char *)ptr;
   MyBuddyItem **buddy_ptr = (MyBuddyItem **)ptr;
   MyBuddyItem *buddy = *buddy_ptr;

   assert(buddy->start == p);

   
}