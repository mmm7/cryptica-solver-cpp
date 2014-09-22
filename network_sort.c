#include "network_sort.h"
#include "stddef.h"

#define min(x, y) (x<y?x:y)
#define max(x, y) (x<y?y:x)
#define SWAP(x,y) { const int a = min(d[x], d[y]); const int b = max(d[x], d[y]); d[x] = a; d[y] = b;}
//#define SWAP(x,y) { if (d[x] > d[y]) { const int tmp = d[x]; d[x] = d[y]; d[y] = tmp;} }

static void sort0(int *d) {
}

static void sort2(int *d) {
  SWAP(0, 1);
}

static void sort3(int *d) {
  SWAP(0, 1);
  SWAP(0, 2);
  SWAP(1, 2);
}

static void sort4(int *d) {
  SWAP(0, 1);
  SWAP(2, 3);
  SWAP(0, 2);
  SWAP(1, 3);
  SWAP(1, 2);
  //printf("%d-%d-%d-%d\n", d[0], d[1], d[2], d[3]);
}

static void sort5(int *d) {
  SWAP(0, 1);
  SWAP(3, 4);
  SWAP(0, 2);
  SWAP(1, 2);
  SWAP(0, 3);
  SWAP(2, 3);
  SWAP(1, 4);
  SWAP(1, 2);
  SWAP(3, 4);
}

static void sort6(int *d) {
  SWAP(1, 2);
  SWAP(4, 5);
  SWAP(0, 2);
  SWAP(3, 5);
  SWAP(0, 1);
  SWAP(3, 4);
  SWAP(1, 4);
  SWAP(0, 3);
  SWAP(2, 5);
  SWAP(1, 3);
  SWAP(2, 4);
  SWAP(2, 3);
}

SortFn sorters[7] = {
  &sort0,
  &sort0,
  &sort2,
  &sort3,
  &sort4,
  &sort5,
  &sort6,
};

