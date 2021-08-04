#ifndef YOLO_PARSER_H
#define YOLO_PARSER_H

#include <stdbool.h>
#include <stdio.h>

typedef enum ExprType {
  EXPR_TYPE_WORD,
  EXPR_TYPE_LIST,
} ExprType;

typedef struct Expr {
  ExprType type;
  char *word;        // if EXPR_TYPE_WORD
  struct List *list; // if EXPR_TYPE_LIST
} Expr;

typedef struct List {
  Expr this;
  struct List *next; // can be NULL
} List;

Expr parse_exprs(FILE *);
void debug_expr(Expr);
void panic(char *, ...);
bool is_empty(List *);

#endif // YOLO_PARSER_H
