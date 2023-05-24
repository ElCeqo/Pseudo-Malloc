# Pseudo-Malloc
  
This is a pseudo-malloc implementation. Request bigger than 1/4 of the page size will be handled by mmap. Smaller requests will be handled by the buddy allocator.


## TODO List
- Finish header and implementation files
- Implement buddy_allocator with bit_map
- Implement Makefile
- Create tests 