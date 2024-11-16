#ifndef clox_value_h
#define clox_value_h

typedef double Value;

typedef struct {
  int cap;
  int count;
  Value *values;
} ValueArray;

void init_valuearray(ValueArray *arr);
void write_valuearray(ValueArray *arr, Value val);
void free_valuearray(ValueArray *arr);

void print_value(Value val);

#endif // !clox_value_h
