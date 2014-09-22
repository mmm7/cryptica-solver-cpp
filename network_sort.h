typedef void (*SortFn)(int *data);
extern SortFn sorters[7];

inline void network_sort(int *data, int num) {
  sorters[num](data);
}
