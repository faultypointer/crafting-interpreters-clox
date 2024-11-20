#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include <stdarg.h>
#include <stdio.h>

static void reset_stack(VM *vm) { vm->stack_top = vm->stack; }

static void runtime_error(VM *vm, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm->ip - vm->chunk->code - 1;
  int line = get_line(vm->chunk, instruction);
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack(vm);
}

void init_vm(VM *vm) { reset_stack(vm); }

void free_vm(VM *vm) {}

static Value peek(VM *vm, int distance) { return vm->stack_top[-1 - distance]; }

static inline Value read_constant(VM *vm, bool is_long) {
  if (!is_long)
    return vm->chunk->constants.values[*(vm->ip++)];
  int index = *(vm->ip++);
  index <<= 8;
  index += *(vm->ip++);
  index <<= 8;
  index += *(vm->ip++);
  return vm->chunk->constants.values[index];
}

static inline InterpretResult binary_operation(VM *vm, char op) {
  Value v_b = vm_pop(vm);
  Value v_a = vm_pop(vm);
  if (!IS_NUMBER(v_b) || !IS_NUMBER(v_a)) {
    runtime_error(vm, "Operands must be numbers.");
    return INTERPRET_RUNTIME_ERROR;
  }
  double a = AS_NUMBER(v_a);
  double b = AS_NUMBER(v_b);
  switch (op) {
  case '+':
    vm_push(vm, NUMBER_VAL(a + b));
    break;
  case '-':
    vm_push(vm, NUMBER_VAL(a - b));
    break;
  case '*':
    vm_push(vm, NUMBER_VAL(a * b));
    break;
  case '/':
    vm_push(vm, NUMBER_VAL(a / b));
    break;
  default:
    runtime_error(vm, "unknown binary operator %c\n", op);
    return INTERPRET_RUNTIME_ERROR;
  }
  return INTERPRET_OK;
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
    Value constant;
    switch (instruction = *(vm->ip++)) {
    case RETURN:
      print_value(vm_pop(vm));
      printf("\n");
      return INTERPRET_OK;
    case CONSTANT:
      constant = read_constant(vm, false);
      vm_push(vm, constant);
      break;
    case CONSTANT_LONG:
      constant = read_constant(vm, true);
      vm_push(vm, constant);
      break;
    case ADD:
      binary_operation(vm, '+');
      break;
    case SUB:
      binary_operation(vm, '-');
      break;
    case MUL:
      binary_operation(vm, '*');
      break;
    case DIV:
      if (binary_operation(vm, '/') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case NEGATE:
      constant = peek(vm, 0);

      if (!IS_NUMBER(constant)) {
        runtime_error(vm, "Operand must be a number");
        return INTERPRET_RUNTIME_ERROR;
      }
      vm->stack_top[-1] = NUMBER_VAL(-AS_NUMBER(constant));

      break;
    }
  }
}

InterpretResult vm_interpret(VM *vm, const char *source) {
  Chunk chunk;
  init_chunk(&chunk);

  if (!compile(source, &chunk)) {
    free_chunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm->chunk = &chunk;
  vm->ip = vm->chunk->code;

  InterpretResult res = vm_run(vm);

  free_chunk(&chunk);
  return res;
}

void vm_push(VM *vm, Value val) {
  *(vm->stack_top) = val;
  vm->stack_top++;
}

Value vm_pop(VM *vm) {
  vm->stack_top--;
  return *vm->stack_top;
}
