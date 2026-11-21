#pragma once

#ifdef DA_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define da_append(xs, i)						\
  if ((xs)->count + 1 > (xs)->capacity) {				\
    if ((xs)->capacity == 0) (xs)->capacity = 256;			\
    else (xs)->capacity *= 2;						\
									\
    int *new_items = realloc((xs)->items, (xs)->capacity * sizeof(*(xs)->items)); \
    if (new_items == NULL) {						\
      fprintf(stderr, "Error in memory allocation.\n");			\
      free((xs)->items);						\
      return EXIT_FAILURE;						\
    }									\
									\
    (xs)->items = new_items;						\
  }									\
									\
  (xs)->items[(xs)->count] = i;						\
  (xs)->count++;

#endif /* DA_IMPLEMENTATION */
