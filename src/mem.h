/* vi:set ts=2 sts=2 sw=2 et:
 *
 * mem.c: A custom memory allocator that manages a pre-allocated memory pool.
 *
 * Part of mylib project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef MEM_H
#define MEM_H

#define MEMBLOCK_SIZE sizeof(struct sMemblock)
#define MEMPOOL 1 * 1024 * 1024 // 1MB

typedef struct sMemblock {
  size_t size;
  struct sMemblock *next;
  int free;
} *memblock;

/*
 * Initialize the memory pool
 */
void mem_init();

/*
 * Allocate a block of memory
 * @param size size of the block
 */
void *mem_alloc(size_t size);

/*
 * Free an allocated block.
 * @param ptr pointer of the block
 */
void mem_free(void *ptr);

/*
 * Free the entire memory pool
 */
void mem_free_all();

/*
 * Use `strdup` string function in memory allocator
 */
char* mem_strdup(const char* s);

#endif
