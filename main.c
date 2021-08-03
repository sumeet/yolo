#include "parser.h"
#include "stb_ds.h"
#include <stdio.h>

typedef struct Interpreter {
} Interpreter;

typedef enum ValueType {
  VALUE_TYPE_NUM,
  VALUE_TYPE_WORD,
  VALUE_TYPE_LIST,
} ValueType;

typedef struct Value {
  ValueType type;
  int64_t num;
  char *word;
  struct ValueList *list;
} Value;

Value new_num_value(int64_t num) {
  Value value = {.type = VALUE_TYPE_NUM, .num = num};
  return value;
}

typedef struct ValueList {
  Value this;
  struct ValueList *next; // can be NULL
};

typedef Value (*Function)(Value);

typedef struct Builtin {
  char *key;
  Function func;
} Builtin;

Value add(Value value) {}

Builtin *init_builtins() {
  Builtin *builtin_by_name = NULL;
  return builtin_by_name;
}

int main() {
  FILE *file = fopen("./hello.yolo", "r");
  if (file == NULL) {
    panic("unable to open file");
  }

  Expr *expr = parse_exprs(file);
  printf("--- PARSED ---");
  debug_expr(expr);
}
