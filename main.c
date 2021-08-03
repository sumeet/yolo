#include "parser.h"
#include <stdio.h>

int main() {
  FILE *file = fopen("./hello.yolo", "r");
  if (file == NULL) {
    panic("unable to open file");
  }

  Expr *expr = parse_exprs(file);
  debug_expr(expr);
}
