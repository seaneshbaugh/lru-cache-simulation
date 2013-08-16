#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
  int name;
  int lastUsedTime;
  unsigned long long value;
} CacheItem;

int main() {
  unsigned long long values[6];

  FILE *random = fopen("/dev/random", "rb");

  if (!random) {
    printf("Could not open \"/dev/random\"\n");
  }

  for (int i = 0; i < 6; i++) {
    fread(&values[i], sizeof(values[i]), 1, random);

    printf("%d - %016llx\n", i, values[i]);
  }

  CacheItem cache[3] = {{-1, 0 , 0}, {-1, 0, 0}, {-1, 0, 0}};

  int n = 0;

  while (n < 100) {
    unsigned long long r;

    fread(&r, sizeof(r), 1, random);

    r = r % 6;

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
