/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _metadata_t {
  unsigned int size;     // The size of the memory block.
  unsigned char isUsed;  // 0 if the block is free; 1 if the block is used.
  struct _metadata_t *next;
  struct _metadata_t *prev;
} metadata_t;

metadata_t *l_head = NULL;
metadata_t *l_tail = NULL;
void *startOfHeap = NULL;


int print_heap(){
    // Print out data about each metadata chunk:
    metadata_t *curMeta = startOfHeap;
    void *endOfHeap = sbrk(0);
    printf("-- Start of Heap (%p) --\n", startOfHeap);
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
        printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
        curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
    }
    printf("-- End of Heap (%p) --\n\n", endOfHeap);
}


/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    // implement calloc:
    return malloc(num*size);
}


/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

/*
 * @TODO 
 * 
 * 
 */

void *malloc(size_t size) {
  //printf("Inside: malloc(%lu):\n", size);
  if (!size) {
    return NULL;
  }
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
    l_head = startOfHeap;
  }

  //block splitting
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
    if (!curMeta->isUsed) {
      if(curMeta->size < size) { //check if we can borrow from next
        metadata_t* n = (void *)curMeta + curMeta->size + sizeof(metadata_t);
        if (!n->isUsed){
          size_t spaceNeeded = size - (curMeta->size + sizeof(metadata_t)); //need: sizeof(allocation) - sum(current capacity + meta of next)
          if (spaceNeeded <= n->size) {
            curMeta->size = size;

            metadata_t *n_meta = (void *)curMeta + size + sizeof(metadata_t); //next meta
            n_meta->isUsed = 0;
            n_meta->size = n->size - spaceNeeded;  //sizeof(original next meta (n)) - the space needed for allocation
            
            curMeta->isUsed = 1;
            return (void *)curMeta + sizeof(metadata_t);
          }
        }
      } else { //current can fit the block
        //split off new block
        size_t new_block_size = curMeta->size - size - sizeof(metadata_t);
        if (new_block_size > 0) {
          metadata_t *new_metadata = (void *)curMeta + size + sizeof(metadata_t);;
          new_metadata->size = new_block_size;
          new_metadata->isUsed = 0;
          //printf("NEW BLOCK\t");
        }
        //printf("block split: old size %d, new block size %d\n", curMeta->size, new_block_size);

        if (new_block_size <= 0) {
          //printf("-----\n");
          size = curMeta->size;
        }
        //current block is marked as used and returned
        curMeta->isUsed = 1;
        curMeta->size = size;
        //printf("new size of cur: %d\n", curMeta->size);
        //printf("\tHEAP: %d\n", startOfHeap - sbrk(0));

        return (void *)curMeta + sizeof(metadata_t);  
      }
    } 
    //next step: move up size of data + size of metadata to access next meta
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);   
  }

  //alloc at the top of the heap
  
  metadata_t *meta = sbrk( sizeof(metadata_t) );
  meta->size = size;
  meta->isUsed = 1;

  // if (startOfHeap - sbrk(0) == sizeof(metadata_t)) { //first alloc head and tail init
  //   l_head = meta;
  //   l_head->next = l_tail;
  //   l_tail->prev = l_head;
  //   l_tail = meta;
  // } else { //any other alloc at the top of the heap
  //   meta->prev = l_tail;
  //   metadata_t* x = &l_tail;
  //   x->next = meta;
  //   l_tail = meta;
  // }

  void *ptr = sbrk(meta->size);

  return ptr;
}


/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
  // implement free
  metadata_t *meta = ptr - sizeof( metadata_t );
  metadata_t * next_m = ptr + meta->size;
  // printf("%d", next_m == meta->next);
  if (!next_m->isUsed) {
    meta->size += next_m->size + sizeof(metadata_t);
  }
  meta->isUsed = 0;
  //printf("im in freeeeeeee %d\n", meta->size);

}


/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
    // implement realloc:
     if (ptr == NULL) return malloc(size);

    void *block = ptr - sizeof(metadata_t);
    metadata_t *m = (metadata_t *) block;

    if (m->size >= size) return ptr;

    void *mem = malloc(size);
    
    if (mem == NULL) return NULL;

    memcpy(mem, ptr, m->size);
    free(ptr);

    return mem;
}
