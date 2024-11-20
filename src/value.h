#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef enum {
  VAL_BOOL,
  VAL_NUMBER,
  VAL_NIL,
} ValueType;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
  } as;
} Value;

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

typedef struct {
  int cap;
  int count;
  Value *values;
} ValueArray;

void init_valuearray(ValueArray *arr);
void write_valuearray(ValueArray *arr, Value val);
void free_valuearray(ValueArray *arr);

void print_value(Value val);
bool values_equal(Value a, Value b);

#endif // !clox_value_h
