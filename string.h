#pragma once

#ifdef STR_IMPLEMENTATION

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  const char *data;
  size_t count;
} String_View;

#define sv_from_parts(s, n) ((String_View) { .data = (s), .count = (n) })
#define sv_from_cstr(s) sv_from_parts((s), strlen(s))

#define sv_eq(a, b)				\
  ((a).count == (b).count &&			\
   memcmp((a).data, (b).data, (a).count) == 0)

#define sv_substr(sv, start, len)		\
  sv_from_parts((sv).data + (start), (len))

typedef struct {
  char *items;
  size_t count;
  size_t capacity;
} String_Builder;

#define sb_init(sb)						\
  ((sb)->items = NULL, (sb)->count = 0, (sb)->capacity = 0)

#define sb_append_char(sb, c)						\
  do {									\
    if ((sb)->count + 2 > (sb)->capacity) {				\
      size_t new_capacity = (sb)->capacity == 0				\
        ? 256								\
        : (sb)->capacity * 2;						\
      									\
      char *new_items = realloc(					\
									(sb)->items, new_capacity \
			       );					\
      									\
      if (new_items == NULL) {						\
        fprintf(stderr, "Memory allocation failed.\n");			\
        return EXIT_FAILURE;						\
      }									\
      									\
      (sb)->items = new_items;						\
      (sb)->capacity = new_capacity;					\
    }									\
    									\
    (sb)->items[(sb)->count++] = (c);					\
    (sb)->items[(sb)->count] = '\\0';					\
  } while (0)

#define sb_append_sv(sb, sv)						\
  do {									\
    size_t required = (sb)->count + (sv).count + 1;			\
									\
    if (required > (sb)->capacity) {					\
      size_t new_capacity = (sb)->capacity == 0				\
        ? 256								\
        : (sb)->capacity;						\
									\
      while (new_capacity < required)					\
        new_capacity *= 2;						\
									\
      char *new_items = realloc(					\
									(sb)->items, new_capacity \
			       );					\
									\
      if (new_items == NULL) {						\
        fprintf(stderr, "Memory allocation failed.\n");			\
        return EXIT_FAILURE;						\
      }									\
									\
      (sb)->items = new_items;						\
      (sb)->capacity = new_capacity;					\
    }									\
									\
    memcpy(								\
							(sb)->items + (sb)->count, \
							(sv).data,	\
							(sv).count	\
	  );								\
									\
    (sb)->count += (sv).count;						\
    (sb)->items[(sb)->count] = '\0';					\
  } while (0)

#define sb_to_sv(sb)				\
  sv_from_parts((sb)->items, (sb)->count)

#define sb_free(sb)				\
  do {						\
    free((sb)->items);				\
    (sb)->items = NULL;				\
    (sb)->count = 0;				\
    (sb)->capacity = 0;				\
  } while (0)

#endif /* STR_IMPLEMENTATION */
