#include <stdlib.h>

#include "mem.h"
#include "log.h"

mem head = NULL;

/*
 * Add a pointer to list
 * @param ptr The pointer to add
 */
void mem_add(void *ptr, cleanup_callback_t cleanup) {
  mem node = malloc(sizeof(struct sMem));
  if(node == NULL) {
    LOG("Failed to allocate memory for mem");
    return;
  }

  node->ptr = ptr;
  node->cleanup = cleanup;
  node->next = head;
  head = node;
}

/*
 * Free all pointers
 */
void mem_free_all() {
  mem ptr = head;
  while(ptr != NULL) {
    if(ptr->ptr != NULL) {
      if(ptr->cleanup) {
        ptr->cleanup(ptr->ptr);
      } else {
        free(ptr->ptr);
      }
    }
    mem tmp = ptr;
    ptr = ptr->next;
    free(tmp);
  }
  head = NULL;
}
