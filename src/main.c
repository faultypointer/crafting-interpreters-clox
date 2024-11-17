#include "chunk.h"
#include "debug.h"

int main(int argc, const char *argv[]) {
  Chunk chunk;
  init_chunk(&chunk);

  int constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, CONSTANT, 10);
  write_chunk(&chunk, constant, 10);

  write_chunk(&chunk, RETURN, 11);
  write_chunk(&chunk, RETURN, 11);
  write_constant_long(&chunk, 123.34, 11);
  write_chunk(&chunk, RETURN, 12);
  write_chunk(&chunk, RETURN, 12);
  write_chunk(&chunk, RETURN, 13);

  disassemble_chunk(&chunk, "test chunk");
  free_chunk(&chunk);
  return 0;
}
