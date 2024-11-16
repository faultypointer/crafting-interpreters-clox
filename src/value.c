#include "value.h"
#include "memory.h"
#include <stdio.h>

void init_valuearray(ValueArray *arr) {
  arr->count = 0;
  arr->cap = 0;
  arr->values = NULL;
}

void write_valuearray(ValueArray *arr, Value val) {
  if (arr->count + 1 > arr->cap) {
    int old_cap = arr->cap;
    arr->cap = GROW_CAPACITY(old_cap);
    arr->values = GROW_ARRAY(Value, arr->values, old_cap, arr->cap);
  }
  arr->values[arr->count++] = val;
}

void free_valuearray(ValueArray *arr) {
  FREE_ARRAY(Value, arr->values, arr->cap);
  init_valuearray(arr);
}

void print_value(Value val) { printf("%g", val); }
