#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

static void reset_stack(VM *vm) { vm->stack_top = vm->stack; }

void init_vm(VM *vm) { reset_stack(vm); }

void free_vm(VM *vm) {}

static inline Value read_constant(VM *vm) {
  return vm->chunk->constants.values[*(vm->ip++)];
}

static inline void binary_operation(VM *vm, char op) {
  Value b = vm_pop(vm);
  Value a = vm_pop(vm);
  switch (op) {
  case '+':
    vm_push(vm, a + b);
    break;
  case '-':
    vm_push(vm, a - b);
    break;
  case '*':
    vm_push(vm, a * b);
    break;
  case '/':
    vm_push(vm, a / b);
    break;
  default:
    printf("unknown binary operator %c\n", op);
  }
}

static InterpretResult vm_run(VM *vm) {
  for (;;) {
#ifdef DEBUG_TRACE_EXEC
    printf("    ");
    for (Value *slot = vm->stack; slot < vm->stack_top; slot++) {
      printf("[ ");
      print_value(*slot);
      printf(" ]");
    }
    printf("\n");
    disassemble_instruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = *(vm->ip++)) {
    case RETURN:
      print_value(vm_pop(vm));
      printf("\n");
      return INTERPRET_OK;
    case CONSTANT:
      Value constant = read_constant(vm);
      vm_push(vm, constant);
      break;
    case ADD:
      binary_operation(vm, '+');
      break;
    case SUB:
      binary_operation(vm, '-');
      break;
    case MUL:
      binary_operation(vm, '-');
      break;
    case DIV:
      binary_operation(vm, '/');
      break;
    case NEGATE:
      vm_push(vm, -vm_pop(vm));
      break;
    }
  }
}

InterpretResult vm_interpret(VM *vm, Chunk *chunk) {
  vm->chunk = chunk;
  vm->ip = vm->chunk->code;
  return vm_run(vm);
}

void vm_push(VM *vm, Value val) {
  *(vm->stack_top) = val;
  vm->stack_top++;
}

Value vm_pop(VM *vm) {
  vm->stack_top--;
  return *vm->stack_top;
}
