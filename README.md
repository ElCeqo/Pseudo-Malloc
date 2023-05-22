# Pseudo-Malloc  ù
  
This is a pseudo-malloc implementation. Request bigger than 1/4 of the page size (4096 bytes) will be handled by mmap. Smaller requests will be handled by the buddy allocator.
