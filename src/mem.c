/* vi:set ts=2 sts=2 sw=2 et:
 *
 * mem.c: A custom memory allocator that manages a pre-allocated memory pool.
 *
 * Part of mylib project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#include <stdint.h>
#include <stddef.h>

#include "mem.h"
#include "log.h"

static char mempool[MEMPOOL];
static memblock freel = (memblock) mempool;

void mem_init() {
  LOG("[mem] Memory pool initialized with %zu bytes\n", MEMPOOL);
  freel->size = MEMPOOL - MEMBLOCK_SIZE;
  freel->free = 1;
  freel->next = NULL;
}

void *mem_alloc(size_t size) {
  memblock current = freel;
  while(current != NULL) {
    if(current->free && current->size >= size) {
      // Splitting if block is too large
      if(current->size > MEMBLOCK_SIZE + size) {
        memblock newb = (memblock)((uint8_t*)current + MEMBLOCK_SIZE + size);
        newb->size = current->size - size - MEMBLOCK_SIZE;
        newb->free = 1;
        newb->next = current->next;
        current->size = size;
        current->next = newb;

        LOG("[mem] Allocated %zu bytes\n", size);
      }
      current->free = 0;
      return (void *)((uint8_t*)current + MEMBLOCK_SIZE);
    }
    current = current->next;
  }

  LOG("[mem] Failed to allocate. Out of memory.\n");
  return NULL;
}

void mem_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  memblock blk = (memblock)((uint8_t*)ptr - MEMBLOCK_SIZE);
  blk->free = 1;


  LOG("[mem] De-allocated %zu bytes\n", blk->size);

  // Coalesce adjacent free blocks
  memblock current = freel;
  while(current != NULL) {
    if(current->free && current->next && current->next->free) {
      current->size += MEMBLOCK_SIZE + current->next->size;
      current->next = current->next->next;
    }
    current = current->next;
  }
}

void mem_free_all() {
  LOG("[mem] De-allocated all memory\n");
  memblock current = freel;
  while(current != NULL) {
    current->free = 1;
    current = current->next;
  }

  freel->size = MEMPOOL - MEMBLOCK_SIZE;
  freel->next = NULL;
}
