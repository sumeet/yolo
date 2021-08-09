#ifndef YOLO_INTERP_H
#include "parser.h"

#define YOLO_INTERP_H
typedef enum ValueType {
  VALUE_TYPE_NUM,
  VALUE_TYPE_WORD,
  VALUE_TYPE_LIST,
} ValueType;

typedef struct Value {
  ValueType type;
  long num;
  char *word;
  struct ValueList *list;
} Value;

typedef struct ValueList {
  Value this;
  struct ValueList *next; // can be NULL
} ValueList;

typedef struct Interpreter {
  struct Builtin *builtins;
  FILE *input;
} Interpreter;

typedef Value (*Function)(Interpreter, Value);

typedef struct Builtin {
  char *key;
  Function value;
} Builtin;

Interpreter init_interpreter(FILE *input);
Value eval(Interpreter, Expr);
void print_value(Value);

#endif // YOLO_INTERP_H
