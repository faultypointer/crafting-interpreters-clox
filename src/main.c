#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char *argv[]) {
  VM vm;
  init_vm(&vm);
  Chunk chunk;
  init_chunk(&chunk);

  int constant = add_constant(&chunk, 1.2);
  write_chunk(&chunk, CONSTANT, 10);
  write_chunk(&chunk, constant, 10);

  constant = add_constant(&chunk, 3.4);
  write_chunk(&chunk, CONSTANT, 10);
  write_chunk(&chunk, constant, 10);

  write_chunk(&chunk, ADD, 10);

  constant = add_constant(&chunk, 5.6);
  write_chunk(&chunk, CONSTANT, 10);
  write_chunk(&chunk, constant, 10);

  write_chunk(&chunk, DIV, 10);

  write_chunk(&chunk, NEGATE, 10);
  write_chunk(&chunk, RETURN, 11);

  vm_interpret(&vm, &chunk);

  free_vm(&vm);
  free_chunk(&chunk);
  return 0;
}
