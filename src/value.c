#include "value.h"
#include "memory.h"
#include "object.h"
#include <stdio.h>
#include <string.h>

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

void print_value(Value val) {
  switch (val.type) {
  case VAL_BOOL:
    printf(AS_BOOL(val) ? "true" : "false");
    break;
  case VAL_NIL:
    printf("nil");
    break;
  case VAL_NUMBER:
    printf("%g", AS_NUMBER(val));
    break;
  case VAL_OBJ:
    print_object(val);
    break;
  }
}

bool values_equal(Value a, Value b) {
  if (a.type != b.type)
    return false;
  switch (a.type) {
  case VAL_BOOL:
    return AS_BOOL(a) == AS_BOOL(b);
  case VAL_NIL:
    return true;
  case VAL_NUMBER:
    return AS_NUMBER(a) == AS_NUMBER(b);
  case VAL_OBJ: {
    ObjString *a_str = AS_STRING(a);
    ObjString *b_str = AS_STRING(b);
    return a_str->length == b_str->length &&
           memcmp(a_str->chars, b_str->chars, a_str->length) == 0;
  }
  default:
    return false;
  }
}
