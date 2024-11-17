#include "chunk.h"
#include "memory.h"
#include "value.h"

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->cap = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->line_count = 0;
  chunk->line_cap = 0;
  init_valuearray(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->count + 1 > chunk->cap) {
    int old_cap = chunk->cap;
    chunk->cap = GROW_CAPACITY(old_cap);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_cap, chunk->cap);
  }
  // run length encoding of line numbers
  if (chunk->line_count == 0 || chunk->lines[chunk->line_count - 2] != line) {
    if (chunk->line_count + 2 > chunk->line_cap) {
      int old_cap = chunk->line_cap;
      chunk->line_cap = GROW_CAPACITY(old_cap);
      chunk->lines = GROW_ARRAY(int, chunk->lines, old_cap, chunk->line_cap);
    }
    chunk->lines[chunk->line_count++] = line;
    chunk->lines[chunk->line_count++] = 1;
  } else {
    chunk->lines[chunk->line_count - 1] += 1;
  }

  chunk->code[chunk->count++] = byte;
}

int add_constant(Chunk *chunk, Value val) {
  write_valuearray(&chunk->constants, val);
  return chunk->constants.count - 1;
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->cap);
  FREE_ARRAY(int, chunk->lines, chunk->line_cap);
  free_valuearray(&chunk->constants);
  init_chunk(chunk);
}

int get_line(Chunk *chunk, int instruction_offset) {
  for (int i = 0; i < chunk->line_count; i += 2) {
    int line = chunk->lines[i];
    int count = chunk->lines[i + 1];
    instruction_offset -= count;
    if (instruction_offset < 0) {
      return line;
    }
  }
  return -1;
}
