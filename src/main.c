#include "chunk.h"
#include <stdio.h>

int main(int argc, const char *argv[]) {
  Chunk chunk;
  init_chunk(&chunk);
  write_chunk(&chunk, RETURN);
  write_chunk(&chunk, RETURN);
  write_chunk(&chunk, RETURN);
  for (int i = 0; i < chunk.count; i++) {
    printf("%d\n", chunk.code[i]);
  }
  free_chunk(&chunk);
  return 0;
}
