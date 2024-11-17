#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <stdio.h>

void disassemble_chunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

static int simple_instruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constant_instruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static int constant_long_instruction(const char *name, Chunk *chunk,
                                     int offset) {

  int index = chunk->code[offset + 1];
  index <<= 8;
  index += chunk->code[offset + 2];
  index <<= 8;
  index += chunk->code[offset + 3];
  printf("%-16s %4d '", name, index);
  print_value(chunk->constants.values[index]);
  printf("'\n");
  return offset + 3;
}

int disassemble_instruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);

  // if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
  //   printf("   | ");
  // } else {
  //   printf("%4d ", chunk->lines[offset]);
  // }

  printf("%4d ", get_line(chunk, offset));

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case RETURN:
    return simple_instruction("RETURN", offset);
  case CONSTANT:
    return constant_instruction("CONSTANT", chunk, offset);
  case CONSTANT_LONG:
    return constant_long_instruction("CONSTANT_LONG", chunk, offset);
  default:
    printf("unknown opcode %d\n", instruction);
    return offset + 1;
  }
}
