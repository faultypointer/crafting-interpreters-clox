#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"

typedef enum { RETURN } OpCode;

typedef struct {
  int count;
  int cap;
  uint8_t *code;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte);
void free_chunk(Chunk *chunk);

#endif // !clox_chunk_h
