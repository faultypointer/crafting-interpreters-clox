#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl(VM *vm) {
  char line[1024];
  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
    // vm_interpret(vm, line);
  }
}

static char *read_file(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "could not open file \"%s\".\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t filesize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(filesize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "not enough memory to read \"%s\".\n", path);
    exit(74);
  }
  size_t bytes_read = fread(buffer, sizeof(char), filesize, file);
  if (bytes_read < filesize) {
    fprintf(stderr, "could not read file \"%s\".\n", path);
    exit(74);
  }
  buffer[bytes_read] = '\0';

  fclose(file);
  return buffer;
}

static void run_file(VM *vm, const char *path) {
  char *source = read_file(path);
  InterpretResult result = vm_interpret(vm, source);
  free(source);
  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[]) {
  VM vm;
  init_vm(&vm);
  if (argc == 1) {
    repl(&vm);
  } else if (argc == 2) {
    run_file(&vm, argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [filepath]\n");
    exit(64);
  }
  return 0;
}
