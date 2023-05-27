# Pseudo-Malloc
  
This is a pseudo-malloc implementation for the Operating Systema course at Sapienza Università di Roma.Memory requests bigger than 1/4 of the page size will be handled by mmap. Smaller requests will be handled by the buddy allocator.


## TODO List

- Add checks in buddy_allocator
- Implement mmap for requests > 1024
- Create more tests  