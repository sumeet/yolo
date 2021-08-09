#include "interp.h"
#include "parser.h"
#include "stb_ds.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    panic("usage: %s script.yolo\n", argv[0]);
  }

  FILE *yolo_file = fopen(argv[1], "r");
  if (yolo_file == NULL) {
    panic("unable to open file %s", argv[1]);
  }

  Expr expr = parse_exprs(yolo_file);
  printf("--- PARSED ---\n");
  debug_expr(expr);

  FILE *input_file = NULL;
  if (argc > 2) {
    input_file = fopen(argv[2], "r");
  }
  struct Interpreter interp = init_interpreter(input_file);

  printf("--- EVAL ---\n");
  struct Value value = eval(interp, expr);
  printf("Eval result: ");
  print_value(value);
  printf("\n");
  return 0;
}
