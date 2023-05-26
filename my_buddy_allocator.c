#include "my_buddy_allocator.h"
#include <assert.h>
#include <math.h>

// Functions for bitmap indices

int levelIdx(size_t idx) { return (int)floor(log2(idx)); };

int buddyIdx(int idx) {
  if (idx & 0x1) {
    return idx - 1;
  }
  return idx + 1;
}

int parentIdx(int idx) { return idx / 2; }

// sets all the parent bits to 1 
void BitMap_ParentSetBitOne(BitMap *bitmap, int idx){
    if (idx == 0) return;

    BitMap_setBit(bitmap, parentIdx(idx), 1);
    BitMap_ParentSetBitOne(bitmap, parentIdx(idx));

    return;
}

// sets parent bit to 0 if both buddies are free
void BitMap_ParentSetBitZero(BitMap *bitmap, int idx){
    if (idx == 0) return;

    // If buddy is also 0 set parent to 0 and repeat
    if (!BitMap_bit(bitmap, idx) && !BitMap_bit(bitmap, buddyIdx(idx))){
        BitMap_setBit(bitmap, parentIdx(idx), 0);
        BitMap_ParentSetBitZero(bitmap, parentIdx(idx));
    }

    // else do nothing
    return;
}

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory, char *buffer){
    
    int number_of_bits = (1 << MAX_LEVELS) +1; // +1 for level 0
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, bitmap);

    buddyAllocator->memory = memory;
    buddyAllocator->buffer = buffer;

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
    item->size = alloc->bitmap.buffer_size / alloc->num_nodes[item->level]; // Memory at level x is divided into num_nodes[x] parts
    item->start = alloc->memory + item->idx;
    return item;
}

// Releases a buddy item
void destroyBuddyItem(MyBuddyAllocator *alloc, MyBuddyItem *item){
    PoolAllocatorResult release_result = PoolAllocator_releaseBlock(&alloc->items, item);
}

void *MyBuddyAllocator_malloc(MyBuddyAllocator *buddyAllocator, int size){
    int level = ceil(log2(size)); // I need bigger space not smaller that's why ceil
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

    // Set ALL the upper tree to allocated recursively
    BitMap_ParentSetBitOne(&buddyAllocator->bitmap, start_index);

    // Then set to allocated all the bits corresponding to subtree
    start_index*=2;

    while (start_index < buddyAllocator->bitmap.num_bits){
        BitMap_setBit(&buddyAllocator->bitmap, start_index, 1);
        BitMap_setBit(&buddyAllocator->bitmap, buddyIdx(start_index), 1);
        start_index*=2;
    }

    // Now that tree is correctly set, get the buddy Item
    MyBuddyItem *mem = createBuddyItem(buddyAllocator, available_bit);
    return (void *)mem->start;
}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

   if (ptr == NULL) return;

   // Retrieve buddy from the system
   char *p = (char *)ptr;
   MyBuddyItem **buddy_ptr = (MyBuddyItem **)ptr;
   MyBuddyItem *buddy = *buddy_ptr;

   assert(buddy->start == p);

   // Set the bits in the bitmap back to 0
   BitMap_setBit(&buddyAllocator->bitmap, buddy->idx, 0);

   // If the buddy is also 0 set the parent to 0 and do this recursively
   BitMap_ParentSetBitZero(&buddyAllocator->bitmap, buddy->idx);

   // Now destroy the item
   PoolAllocator_releaseBlock(&buddyAllocator->items, buddy);

   return;
}