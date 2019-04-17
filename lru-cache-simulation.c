#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct {
  int name;
  int lastUsedTime;
  uint64_t value;
} CacheItem;

int main(int argc, char** argv) {
  uint64_t values[6];

  for (int i = 0; i < 6; i++) {
    uint64_t randomLo = (uint64_t)arc4random();
    uint64_t randomHi = (uint64_t)arc4random();

    values[i] = (int64_t)((randomHi << 31) | (randomLo >> 1));

    printf("%d - %016llx\n", i, values[i]);
  }

  CacheItem cache[3] = {{-1, 0 , 0}, {-1, 0, 0}, {-1, 0, 0}};

  int n = 0;

  while (n < 100) {
    uint32_t r = arc4random_uniform(6);

    bool hit = false;

    for (int i = 0; i < 3; i++) {
      if (cache[i].name == r) {
        cache[i].lastUsedTime = n;

        hit = true;

        printf("Hit!  %d - %016llx        in cache slot %d\n", cache[i].name, cache[i].value, i);
      }
    }

    if (!hit) {
      int lru = 0;

      for (int i = 0; i < 3; i++) {
        if (cache[i].lastUsedTime < cache[lru].lastUsedTime) {
          lru = i;
        }
      }

      cache[lru].name = r;

      cache[lru].lastUsedTime = n;

      nanosleep((struct timespec[]){{0, 200000000}}, NULL);

      cache[lru].value = values[r];

      printf("Miss! %d - %016llx replacing cache slot %d\n", cache[lru].name, cache[lru].value, lru);
    }

    n++;
  }

  return 0;
}
