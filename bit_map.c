#include "bit_map.h"
#include <assert.h>

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits) { return bits / 8 + (bits % 8 != 0); }

// initializes a bitmap on an external array
void BitMap_init(BitMap *bit_map, int num_bits, uint8_t *buffer) {
  bit_map->buffer = buffer;
  bit_map->num_bits = num_bits;
  bit_map->buffer_size = BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap *bit_map, int bit_num, int status) {
  // get byte
  int byte_num = bit_num >> 3;
  assert(byte_num <= bit_map->buffer_size);
  int bit_in_byte = bit_num % 8;
  if (status) {
    bit_map->buffer[byte_num] |= (1 << bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1 << bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap *bit_map, int bit_num) {
  int byte_num = bit_num >> 3;
  assert(byte_num <= bit_map->buffer_size);
  int bit_in_byte = bit_num % 8;
  return (bit_map->buffer[byte_num] & (1 << bit_in_byte)) != 0;
}


void printBitMap(BitMap *bitmap){
    for(int i = 0; i < bitmap->num_bits; ++i){
        
        if(BitMap_bit(bitmap, i)){
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

int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}

void BitMap_SetSubTree(BitMap *bitmap, int idx, int status){

    BitMap_setBit(bitmap, idx, status);

    int left_child = (idx * 2);
    int right_child = (idx* 2) + 1;

    if(left_child <= bitmap->num_bits){
        BitMap_SetSubTree(bitmap, left_child, status);
    }

    if(right_child <= bitmap->num_bits){
        BitMap_SetSubTree(bitmap, right_child, status);
    }

    return;
}

void BitMap_ParentSetBit(BitMap *bitmap, int idx, int status){
    
    if (status == 1){
      if (idx == 1) return;
      
      BitMap_setBit(bitmap, parentIdx(idx), 1);
      BitMap_ParentSetBit(bitmap, parentIdx(idx), status);
      
      return;
    }

    if (status == 0){
      if(idx == 0){
        BitMap_setBit(bitmap, idx, 0);
        return;
      }

      // If buddy is also 0 set parent to 0 and repeat
      if (!BitMap_bit(bitmap, idx) && !BitMap_bit(bitmap, buddyIdx(idx))){
        BitMap_setBit(bitmap, parentIdx(idx), 0);
        BitMap_ParentSetBit(bitmap, parentIdx(idx), status);
      }

      // else do nothing
      return;
    }
}