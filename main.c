#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void panic(char *msg) {
  perror(msg);
  exit(1);
}

// 20 KB max word, should be enough.
const size_t WORD_BUFFER_SIZE = 20 * 1024;
const size_t DEBUG_PADDING = 1;

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

List *new_list(Expr *expr) {
  List *list = malloc(sizeof(List));
  list->this = expr;
  list->next = NULL;
  return list;
}

Expr *new_expr_list(List *list) {
  Expr *expr = malloc(sizeof(Expr));
  expr->type = EXPR_TYPE_LIST;
  expr->list = list;
  return expr;
}

Expr *new_expr_word(char *begin, size_t len) {
  char *new_guy = malloc(sizeof(char) * (len + 1));
  memcpy(new_guy, begin, len);
  new_guy[len] = '\0';

  Expr *expr = malloc(sizeof(Expr));
  expr->type = EXPR_TYPE_WORD;
  expr->word = new_guy;
  return expr;
}

void append(List **list, Expr *expr) {
  if (*list == NULL) {
    *list = new_list(expr);
  } else {
    append(&((*list)->next), expr);
  }
}

void append_word(List **exprs, char *begin, size_t *len) {
  if (*len > 0) {
    append(exprs, new_expr_word(begin, *len));
    *len = 0;
  }
}

Expr *parse_exprs_rec(FILE *file, bool is_inside_list) {
  List *exprs = NULL;
  size_t current_word_len = 0;
  char current_word[WORD_BUFFER_SIZE];

  while (true) {
    char c = (char)getc(file);
    if (c == EOF) {
      if (is_inside_list) {
        panic("was expecting a ) but reached end of stream");
      } else {
        append_word(&exprs, current_word, &current_word_len);
        break;
      }
    }

    if (c == '(') {
      append(&exprs, parse_exprs_rec(file, true));
    } else if (c == ')') {
      if (is_inside_list) {
        append_word(&exprs, current_word, &current_word_len);
        break;
      } else {
        panic("got a ) and wasn't expecting one");
      }
    } else if (isspace(c)) {
      // TODO: duplicated
      append_word(&exprs, current_word, &current_word_len);
    } else {
      current_word[current_word_len] = c;
      current_word_len++;
    }
  }

  return new_expr_list(exprs);
}

Expr *parse_exprs(FILE *file) { return parse_exprs_rec(file, false); }

void debug_padding(size_t depth) {
  for (size_t i = 0; i < (depth * DEBUG_PADDING); i++) {
    putchar((int)' ');
  }
}

void debug_expr_rec(Expr *expr, size_t current_depth) {
  switch (expr->type) {
  case EXPR_TYPE_WORD:
    debug_padding(current_depth * DEBUG_PADDING);
    printf("%s", expr->word);
    break;
  case EXPR_TYPE_LIST:
    debug_padding(current_depth * DEBUG_PADDING);
    printf("(\n");
    List *head = expr->list;
    while (head != NULL) {
      debug_expr_rec(head->this, current_depth + 1);
      head = head->next;
    }
    debug_padding(current_depth * DEBUG_PADDING);
    putchar(')');
  }

  putchar((int)'\n');
}

void debug_expr(Expr *expr) { debug_expr_rec(expr, 0); }

int main() {
  FILE *file = fopen("./hello.yolo", "r");
  if (file == NULL) {
    panic("unable to open file");
  }

  Expr *expr = parse_exprs(file);
  debug_expr(expr);
}
