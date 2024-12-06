#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

typedef struct {
  ObjString *key;
  Value value;
} Entry;

typedef struct {
  int count;
  int capacity;
  Entry *entries;
} Table;

void init_table(Table *table);
void free_table(Table *table);

bool table_set(Table *table, ObjString *key, Value value);
void table_add_all(Table *from, Table *to);
bool table_get(Table *table, ObjString *key, Value *value);
bool table_delete(Table *table, ObjString *key);

#endif /* ifndef clox_table_h */