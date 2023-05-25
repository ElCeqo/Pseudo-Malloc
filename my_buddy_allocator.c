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

// sets all the parent bits to 1 if both of the buddys are allocated
void BitMap_parentBits(BitMap *bitmap, int idx){
    if (idx == 0) return;

    // If buddy is set to 1 set parent and go recursively
    if(BitMap_bit(bitmap, buddyIdx(idx))){
        BitMap_setBit(bitmap, parentIdx(idx), 1);
        BitMap_parentBits(bitmap, parentIdx(idx));
    }
    
    // If buddy is not allocated do nothing
    return;
}

void MyBuddyAllocator_init(MyBuddyAllocator *buddyAllocator, uint8_t *bitmap, char *memory, char *buffer){
    
    int number_of_bits = 1 << MAX_LEVELS +1;
    BitMap_init(&buddyAllocator->bitmap, number_of_bits, bitmap);

    buddyAllocator->memory = memory;

    // initialize the number of blocks at each level
    for(int i = 0; i < MAX_LEVELS; ++i){
        buddyAllocator->num_nodes[i] = (1 << MAX_LEVELS - i);
    }


    PoolAllocatorResult init_result = PoolAllocator_init(&buddyAllocator->items, sizeof(MyBuddyItem), 1 << MAX_LEVELS, buddyAllocator->buffer, sizeof(MyBuddyItem) * (1 << MAX_LEVELS));
    printf("%s\n", PoolAllocator_strerror(init_result));
}

// recursively set parent pointer
MyBuddyItem *getBuddyItem(MyBuddyAllocator *alloc, int level, int idx){

    if (level < 0) return 0;

    MyBuddyItem *parent_ptr = getBuddyItem(alloc, level -1, parentIdx(idx));

}

// Returns a new BuddyItem
MyBuddyItem *createBuddyItem(MyBuddyAllocator *alloc, int idx, MyBuddyItem *parent_ptr){
    MyBuddyItem *item = (MyBuddyItem *)PoolAllocator_getBlock(&alloc->items);
    item->idx = idx;
    item->level = levelIdx(idx);
    item->size = alloc->bitmap.buffer_size / alloc->num_nodes[item->level]; // Memory at level x is divided into num_nodes[x] parts
    item->start = alloc->memory + item->idx;
    item->parent_ptr = parent_ptr;
    return item;
}

// Releases a buddy item
void destroyBuddyItem(MyBuddyAllocator *alloc, MyBuddyItem *item){
    PoolAllocatorResult release_result = PoolAllocator_releaseBlock(&alloc->items, item);
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

    // If the buddy is also allocated set the parent to allocated recursively
    if (BitMap_bit(&buddyAllocator->bitmap, buddyIdx(start_index))){
        BitMap_parentBits(&buddyAllocator->bitmap, start_index);
    }

    // Then set to allocated all the bits corresponding to subtree
    start_index*=2;

    while (start_index < &buddyAllocator->bitmap.num_bits){
        BitMap_setBit(&buddyAllocator->bitmap, start_index, 1);
        BitMap_setBit(&buddyAllocator->bitmap, buddyIdx(start_index), 1);
        start_index*=2;
    }    //If buddy is set set parent_ptr else null
    //PUT PARENT_PTR TODO

    // Create the buddy Item
    MyBuddyItem *mem = getBuddyItem(buddyAllocator, level, available_bit);
    return (void *)mem->start;
}

void MyBuddyAllocator_free(MyBuddyAllocator *buddyAllocator, void *ptr){

   if (ptr == NULL) return;

   // Retrieve buddy from the system
   char *p = (char *)ptr;
   MyBuddyItem **buddy_ptr = (MyBuddyItem **)ptr;
   MyBuddyItem *buddy = *buddy_ptr;

   assert(buddy->start == p);

   // need to recursively set the parent to 0 if both buddies are free
   // Need to implement buddy and parents in the Item structure 

   // Set the bits in the bitmap back to 0
   BitMap_setBit(&buddyAllocator->bitmap, buddy->idx, 0);

   if (!buddy->parent_ptr) return;

   

}