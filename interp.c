//
// Created by sumeet on 8/8/21.
//

#include "interp.h"
#include "parser.h"
#include "stb_ds.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

Value new_num_value(long num) {
  Value value = {.type = VALUE_TYPE_NUM, .num = num};
  return value;
}

Value new_list_value(ValueList *list) {
  Value value = {.type = VALUE_TYPE_LIST, .list = list};
  return value;
}

Value new_empty_list_value() {
  Value value = {.type = VALUE_TYPE_LIST, .list = NULL};
  return value;
}

ValueList value_as_list(Value value) { return *value.list; }

long value_as_num(Value value) { return value.num; }

char *value_as_word(Value value) { return value.word; }

void value_list_append(ValueList **list, Value value) {
  if (*list == NULL) {
    *list = malloc(sizeof(ValueList));
    (*list)->this = value;
    (*list)->next = NULL;
  } else {
    value_list_append(&((*list)->next), value);
  }
}

Value add(Value value) {
  long acc = 0;
  // TODO: i think this doesn't handle the case is value is empty
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

Interpreter init_interpreter() {
  Builtin *builtin_by_name = NULL;
  sh_new_strdup(builtin_by_name);
  shput(builtin_by_name, "+", add);
  Interpreter interp = {.builtins = builtin_by_name};
  return interp;
}

// returns NULL if not found
Function get_func(Interpreter interp, char *word) {
  return shget(interp.builtins, word);
}

Value eval(Interpreter interp, Expr expr) {
  Value value;
  switch (expr.type) {
  case EXPR_TYPE_WORD:
    errno = 0;
    char *endptr;
    long num = strtol(expr.word, &endptr, 10);
    if (errno == 0 && endptr != expr.word) {
      value.type = VALUE_TYPE_NUM;
      value.num = num;
    } else {
      value.type = VALUE_TYPE_WORD;
      value.word = expr.word;
    }
    break;

  case EXPR_TYPE_LIST:
    if (is_empty(expr.list)) {
      panic("list was empty. for an empty list, use the list constructor");
    }

    value = new_empty_list_value();
    ValueList *value_root = value.list;
    List *expr_root = expr.list;
    while (true) {
      value_list_append(&value_root, eval(interp, expr_root->this));

      if (expr_root->next == NULL) {
        break;
      } else {
        expr_root = expr_root->next;
      }
    }

    Value first_value = value_root->this;
    if (first_value.type != VALUE_TYPE_WORD) {
      panic("first element of list must be a word, but was a list");
    }
    Function func = get_func(interp, first_value.word);
    if (func == NULL) {
      panic("function %s not found", first_value.word);
    }
    value = func(new_list_value(value_root->next));
    break;
  }
  return value;
}

void print_value(Value value) {
  switch (value.type) {
  case VALUE_TYPE_NUM:
    printf("%ld", value_as_num(value));
    break;
  case VALUE_TYPE_WORD:
    printf("%s", value_as_word(value));
    break;
  case VALUE_TYPE_LIST:
    printf("(");

    ValueList list = value_as_list(value);
    print_value(list.this);
    while (list.next != NULL) {
      printf(" ");
      list = *list.next;
      print_value(list.this);
    }

    printf(")");
    break;
  }
}