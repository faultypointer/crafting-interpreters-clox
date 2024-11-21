#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "memory.h"
#include "object.h"
#include "value.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

VM vm;

static void reset_stack() { vm.stack_top = vm.stack; }

static void runtime_error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm.ip - vm.chunk->code - 1;
  int line = get_line(vm.chunk, instruction);
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack();
}

void init_vm() {
  reset_stack();
  vm.objects = NULL;
}

void free_vm() { free_objects(); }

static Value peek(int distance) { return vm.stack_top[-1 - distance]; }

static bool is_falsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static inline Value read_constant(bool is_long) {
  if (!is_long)
    return vm.chunk->constants.values[*(vm.ip++)];
  int index = *(vm.ip++);
  index <<= 8;
  index += *(vm.ip++);
  index <<= 8;
  index += *(vm.ip++);
  return vm.chunk->constants.values[index];
}

static inline InterpretResult concatenate_strings() {
  ObjString *b = AS_STRING(vm_pop());
  ObjString *a = AS_STRING(vm_pop());

  int length = a->length + b->length;
  char *chars = ALLOCATE(char, length + 1);
  memcpy(chars, a->chars, a->length);
  memcpy(chars + a->length, b->chars, b->length);
  chars[length] = '\0';
  ObjString *result = take_string(chars, length);
  vm_push(OBJ_VAL(result));
}

static inline InterpretResult binary_operation(char op) {
  Value v_b = vm_pop();
  Value v_a = vm_pop();
  if (!IS_NUMBER(v_b) || !IS_NUMBER(v_a)) {
    runtime_error("Operands must be numbers.");
    return INTERPRET_RUNTIME_ERROR;
  }
  double a = AS_NUMBER(v_a);
  double b = AS_NUMBER(v_b);
  switch (op) {
  case '+':
    vm_push(NUMBER_VAL(a + b));
    break;
  case '-':
    vm_push(NUMBER_VAL(a - b));
    break;
  case '*':
    vm_push(NUMBER_VAL(a * b));
    break;
  case '/':
    vm_push(NUMBER_VAL(a / b));
    break;
  case '>':
    vm_push(BOOL_VAL(a > b));
    break;
  case '<':
    vm_push(BOOL_VAL(a < b));
    break;
  default:
    runtime_error("unknown binary operator %c\n", op);
    return INTERPRET_RUNTIME_ERROR;
  }
  return INTERPRET_OK;
}

static InterpretResult vm_run() {
  for (;;) {
#ifdef DEBUG_TRACE_EXEC
    printf("    ");
    for (Value *slot = vm.stack; slot < vm.stack_top; slot++) {
      printf("[ ");
      print_value(*slot);
      printf(" ]");
    }
    printf("\n");
    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    Value constant;
    switch (instruction = *(vm.ip++)) {
    case RETURN:
      print_value(vm_pop());
      printf("\n");
      return INTERPRET_OK;
    case CONSTANT:
      constant = read_constant(false);
      vm_push(constant);
      break;
    case CONSTANT_LONG:
      constant = read_constant(true);
      vm_push(constant);
      break;
    case NIL:
      vm_push(NIL_VAL);
      break;
    case TRUE:
      vm_push(BOOL_VAL(true));
      break;
    case FALSE:
      vm_push(BOOL_VAL(false));
      break;
    case EQUAL:
      Value b = vm_pop();
      Value a = vm_pop();
      vm_push(BOOL_VAL(values_equal(a, b)));
      break;
    case GREATER:
      if (binary_operation('>') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case LESS:
      if (binary_operation('<') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case ADD:
      if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
        concatenate_strings();
      } else if (binary_operation('+') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case SUB:
      if (binary_operation('-') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case MUL:
      if (binary_operation('*') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case DIV:
      if (binary_operation('/') == INTERPRET_RUNTIME_ERROR) {
        return INTERPRET_RUNTIME_ERROR;
      }
      break;
    case NOT:
      vm_push(BOOL_VAL(is_falsey(vm_pop())));
      break;
    case NEGATE:
      constant = peek(0);

      if (!IS_NUMBER(constant)) {
        runtime_error("Operand must be a number");
        return INTERPRET_RUNTIME_ERROR;
      }
      vm.stack_top[-1] = NUMBER_VAL(-AS_NUMBER(constant));

      break;
    }
  }
}

InterpretResult vm_interpret(const char *source) {
  Chunk chunk;
  init_chunk(&chunk);

  if (!compile(source, &chunk)) {
    free_chunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  InterpretResult res = vm_run();

  free_chunk(&chunk);
  return res;
}

void vm_push(Value val) {
  *(vm.stack_top) = val;
  vm.stack_top++;
}

Value vm_pop() {
  vm.stack_top--;
  return *vm.stack_top;
}
