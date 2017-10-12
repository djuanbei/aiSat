
/**
 * @file   bimap.c
 * @author Liyun Dai <dlyun2009@gmail.com>
 * @date   Sun Feb  8 13:56:30 2015
 *
 * @brief
 *
 *
 */

#include "bimap.h"
#include "search.h"

#include "selfmemutil.h"

Bimap *createBimap(void) {
  Bimap *re = (Bimap *)malloc_d(sizeof(Bimap));
  re->capacity = DEFAULT_CAP;
  re->values = (int *)malloc_d(2 * re->capacity * sizeof(int));
  re->keys = (int *)malloc_d(2 * re->capacity * sizeof(int));
  re->size = 0;
  return re;
}

static int compfun(const void *lhs, const void *rhs) {
  return ((int *)lhs)[0] - ((int *)rhs)[0];
}

static void enlargeBimap(Bimap *map) {
  map->capacity = map->capacity * ENLARGE_RAT + 1;
  map->values = (int *)realloc_d(map->values, 2 * map->capacity * sizeof(int));
  map->values = (int *)realloc_d(map->keys, 2 * map->capacity * sizeof(int));
}

/**
 *  have bugs
 *
 * @param map
 * @param value
 *
 * @return
 */
int addBimapElem(Bimap *map, int value) {
  if (map->size + 2 >= map->capacity) enlargeBimap(map);

  if (0 == map->size) {
    map->values[0] = value;
    map->values[1] = 0;
    map->keys[0] = 0;
    map->keys[1] = value;
    map->size++;
    return 0;
  }
  int loc = bisearchLocBettwen(map->values, &value, map->size, 2, compfun);
  // can not find
  if (loc < 0) {
    if (map->values[0] > value) {
      memmove(map->values + 2, map->values, 2 * map->size * sizeof(int));
      map->values[0] = value;
      map->values[1] = map->size;

    } else {
      map->values[2 * map->size] = value;
      map->values[2 * map->size + 1] = map->size;
    }

  } else if (map->values[2 * loc] == value) {
    return map->values[2 * loc + 1];
  } else {
    memmove(map->values + 2 * loc + 2, map->values + 2 * loc,
            2 * (map->size - loc) * sizeof(int));
    map->values[2 * loc] = value;
    map->values[2 * loc + 1] = map->size;
  }

  map->keys[2 * map->size] = map->size;
  map->keys[2 * map->size + 1] = value;
  map->size++;
  return map->size - 1;
}

/**
 *
 *
 * @param map
 * @param key
 *
 * @return loc >=0 if find key in map,
 * otherwise, -1
 */

int findBimapByKey(Bimap *map, const int key) {
  int loc = bisearchLocBettwen(map->keys, &key, map->size, 2, compfun);
  if (loc < 0) return -1;
  if (map->keys[2 * loc] != key) return -1;
  return map->keys[2 * loc];
}

int findBimapByValue(Bimap *map, const int value) {
  int loc = bisearchLocBettwen(map->values, &value, map->size, 2, compfun);
  if (loc < 0 || map->values[2 * loc] != value) return -1;
  return map->values[2 * loc + 1];
}

void deleteBimap(Bimap *map) {
  if (NULL != map) {
    free(map->values);
    free(map->keys);
    free(map);
    map = NULL;
  }
}
void clearBimap(Bimap *map) { map->size = 0; }
