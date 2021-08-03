#include "parser.h"
#include "stb_ds.h"
#include <stdbool.h>
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
} ValueList;

ValueList value_as_list(Value value) { return *value.list; }
int64_t value_as_num(Value value) { return value.num; }

typedef Value (*Function)(Value);

typedef struct Builtin {
  char *key;
  Function value;
} Builtin;

Value add(Value value) {
  int64_t acc = 0;
  ValueList list = value_as_list(value);
  while (true) {
    acc += value_as_num(list.this);
    if (list.next == NULL) {
      break;
    } else {
      list = *list.next;
    }
  }
  return new_num_value(acc);
}

Builtin *init_builtins() {
  Builtin *builtin_by_name = NULL;
  shput(builtin_by_name, "+", add);
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
