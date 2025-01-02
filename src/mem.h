#ifndef MEM_H
#define MEM_H

typedef void (*cleanup_callback_t)(void *);

typedef struct sMem {
  void *ptr;
  cleanup_callback_t cleanup;
  struct sMem *next;
} *mem;

void mem_add(void *ptr, cleanup_callback_t cleanup);

void mem_free_all();

#endif

