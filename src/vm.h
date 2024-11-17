#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256
typedef struct {
  Chunk *chunk;
  uint8_t *ip;
  Value stack[STACK_MAX];
  Value *stack_top;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void init_vm(VM *vm);
void free_vm(VM *vm);

InterpretResult vm_interpret(VM *vm, Chunk *chunk);
void vm_push(VM *vm, Value val);
Value vm_pop(VM *vm);

#endif /* ifndef clox_vm_h */
