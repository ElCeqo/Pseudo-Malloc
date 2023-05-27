#include "my_buddy_allocator.h"
#include <assert.h>
#include <math.h>
#include "pseudo_malloc.h"

// Functions for bitmap indices

void printBitMap(BitMap *bitmap){
    for(int i = 0; i < bitmap->num_bits; ++i){
        
        if(!BitMap_bit(bitmap, i)){
            printf("bit num %d   value = %d\n", i ,BitMap_bit(bitmap, i));
        }
    }
}

int levelIdx(size_t idx) { return (int)floor(log2(idx)); };

int buddyIdx(int idx) {
  if (idx & 0x1) {
    return idx - 1;
  }
  return idx + 1;
}

int parentIdx(int idx) { return (idx) / 2; }

// Offset of the level from the current idx
int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}

void BitMap_SetSubTreeToOne(BitMap *bitmap, int idx){

    BitMap_setBit(bitmap, idx, 1);

    int left_child = (idx * 2);
    int right_child = (idx* 2) + 1;

    if(left_child <= bitmap->num_bits){
        BitMap_SetSubTreeToOne(bitmap, left_child);
    }

    if(right_child <= bitmap->num_bits){
        BitMap_SetSubTreeToOne(bitmap, right_child);
    }

    return;
}

void BitMap_SetSubTreeToZero(BitMap *bitmap, int idx){
    
    BitMap_setBit(bitmap, idx, 0);

    int left_child = (idx * 2);
    int right_child = (idx* 2) + 1;

    if(left_child <= bitmap->num_bits){
        BitMap_SetSubTreeToZero(bitmap, left_child);
    }

    if(right_child <= bitmap->num_bits){
        BitMap_SetSubTreeToZero(bitmap, right_child);
    }

    return;   
}

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
    int level = MAX_LEVELS - ceil(log2(size)); // I need bigger space not smaller that's why ceil
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
    BitMap_ParentSetBitOne(&buddyAllocator->bitmap, available_bit);

    // Then set to allocated all the bits corresponding to subtree
    BitMap_SetSubTreeToOne(&buddyAllocator->bitmap, available_bit);

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

   // Set the bits in the bitmap back to 0
   BitMap_setBit(&buddyAllocator->bitmap, buddy->idx, 0);

   // If the buddy is also 0 set the parent to 0 and do this recursively
   BitMap_ParentSetBitZero(&buddyAllocator->bitmap, buddy->idx);

   // And set its subtree back to 0
   BitMap_SetSubTreeToZero(&buddyAllocator->bitmap, buddy->idx);

   // Now destroy the item
   PoolAllocator_releaseBlock(&buddyAllocator->items, buddy);

   printf("Free success!\n");
   return;
}