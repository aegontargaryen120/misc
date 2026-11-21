#pragma once

#ifdef HM_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define HASH(key) ((size_t)(uint32_t)(key))
#define HASH_INDEX(key, capacity) (HASH(key) & ((capacity) - 1))

#define hm_resize(map, new_capacity)					\
  do {									\
    Bucket *new_items = calloc(new_capacity, sizeof(Bucket));		\
    if (new_items == NULL) {						\
      fprintf(stderr, "Error in memory allocation.\n");			\
      return EXIT_FAILURE;						\
    }									\
									\
    for (size_t i = 0; i < (map)->capacity; ++i) {			\
      if (!(map)->items[i].occupied) continue;				\
									\
      size_t index = HASH_INDEX(					\
									(map)->items[i].key, new_capacity \
			       );					\
									\
      while (new_items[index].occupied)					\
        index = (index + 1) & (new_capacity - 1);			\
									\
      new_items[index] = (map)->items[i];				\
    }									\
									\
    free((map)->items);							\
    (map)->items = new_items;						\
    (map)->capacity = new_capacity;					\
  } while (0)

#define hm_insert(map, k, v)				\
  do {							\
    if ((map)->capacity == 0) {				\
      hm_resize(map, 256);				\
    }							\
							\
    if ((map)->count >= (map)->capacity * 0.7) {	\
      hm_resize(map, (map)->capacity * 2);		\
    }							\
							\
    size_t index = HASH_INDEX(k, (map)->capacity);	\
							\
    while ((map)->items[index].occupied) {		\
      if ((map)->items[index].key == (k)) {		\
        (map)->items[index].value = (v);		\
        break;						\
      }							\
							\
      index = (index + 1) & ((map)->capacity - 1);	\
    }							\
							\
    if (!(map)->items[index].occupied) {		\
      (map)->items[index].key = (k);			\
      (map)->items[index].value = (v);			\
      (map)->items[index].occupied = true;		\
      (map)->count++;					\
    }							\
  } while (0)

#define hm_get(map, k, out)				\
  ({							\
    bool found = false;					\
							\
    if ((map)->capacity != 0) {				\
      size_t index = HASH_INDEX(k, (map)->capacity);	\
							\
      for (size_t i = 0; i < (map)->capacity; ++i) {	\
        if (!(map)->items[index].occupied) break;	\
							\
        if ((map)->items[index].key == (k)) {		\
          *(out) = (map)->items[index].value;		\
          found = true;					\
          break;					\
        }						\
							\
        index = (index + 1) & ((map)->capacity - 1);	\
      }							\
    }							\
							\
    found;						\
  })

#endif /* HM_IMPLEMENTATION */
