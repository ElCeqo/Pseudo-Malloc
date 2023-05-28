#pragma once
#include <stdint.h>
#include <math.h>
#include <stdio.h>

// simple bit array
typedef struct {
  uint8_t *buffer; // externally allocated buffer
  int buffer_size; // in bytes
  int num_bits;
} BitMap;

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits);

// initializes a bitmap on an external array
void BitMap_init(BitMap *bit_map, int num_bits, uint8_t *buffer);

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap *bit_map, int bit_num, int status);

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap *bit_map, int bit_num);

void printBitMap(BitMap *bitmap);

// index of the level
int levelIdx(size_t idx);

// index of the other child
int buddyIdx(int idx);

// index of the parent node
int parentIdx(int idx);

// offset of the index from tne start of the level
int startIdx(int idx);

// sets the subtree of a node to status
void BitMap_SetSubTree(BitMap *bitmap, int idx, int status);

// sets all parent bits of a node to status
void BitMap_ParentSetBit(BitMap *bitmap, int idx, int status);