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

typedef Value (*Function)(Value);

typedef struct Builtin {
  char *key;
  Function value;
} Builtin;

typedef struct Interpreter {
  Builtin *builtins;
} Interpreter;

Interpreter init_interpreter();
Value eval(Interpreter, Expr);
void print_value(Value);

#endif // YOLO_INTERP_H
