#include "chunk.h"
#include "value.h"

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->cap = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  init_valuearray(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->count + 1 > chunk->cap) {
    int old_cap = chunk->cap;
    chunk->cap = GROW_CAPACITY(old_cap);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_cap, chunk->cap);
    chunk->lines = GROW_ARRAY(int, chunk->lines, old_cap, chunk->cap);
  }
  chunk->lines[chunk->count] = line;
  chunk->code[chunk->count++] = byte;
}

int add_constant(Chunk *chunk, Value val) {
  write_valuearray(&chunk->constants, val);
  return chunk->constants.count - 1;
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->cap);
  FREE_ARRAY(int, chunk->lines, chunk->cap);
  free_valuearray(&chunk->constants);
  init_chunk(chunk);
}
