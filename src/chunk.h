#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"
#include "value.h"

typedef enum {
  CONSTANT,
  CONSTANT_LONG,

  ADD,
  SUB,
  MUL,
  DIV,

  NEGATE,

  RETURN,
} OpCode;

typedef struct {
  int count;
  int cap;
  uint8_t *code;
  int *lines;
  int line_count;
  int line_cap;
  ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, int line);
void write_constant_long(Chunk *chunk, Value val, int line);
int add_constant(Chunk *chunk, Value val);
void free_chunk(Chunk *chunk);
int get_line(Chunk *chunk, int instruction_offset);

#endif // !clox_chunk_h
