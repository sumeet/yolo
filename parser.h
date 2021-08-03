#ifndef YOLO_PARSER_H
#define YOLO_PARSER_H

typedef enum ExprType {
  EXPR_TYPE_WORD,
  EXPR_TYPE_LIST,
} ExprType;

typedef struct List {
  struct Expr *this;
  struct List *next; // can be NULL
} List;

typedef struct Expr {
  ExprType type;
  char *word; // if EXPR_TYPE_WORD
  List *list; // if EXPR_TYPE_LIST
} Expr;

Expr *parse_exprs(FILE *);
void debug_expr(Expr *);
void panic(char *);

#endif // YOLO_PARSER_H
