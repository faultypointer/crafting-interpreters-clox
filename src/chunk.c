#include "chunk.h"

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->cap = 0;
  chunk->code = NULL;
}

void write_chunk(Chunk *chunk, uint8_t byte) {
  if (chunk->count + 1 > chunk->cap) {
    int old_cap = chunk->cap;
    chunk->cap = GROW_CAPACITY(old_cap);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_cap, chunk->cap);
  }
  chunk->code[chunk->count++] = byte;
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->cap);
  init_chunk(chunk);
}
