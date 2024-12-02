#include <setjmp.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"

typedef enum { false, true } bool;

void display(node_t *node)
{
  switch (node->type)
  {
    case nt_variable: printf("%c", node->name); return;
    case nt_operator:
    {
      bool need_left_paren = false;
      bool need_right_paren = false;

      switch (node->name)
      {
        case '*':
        case '/':
          if ((node->child[0]->type == nt_operator)
           && ((node->child[0]->name == '+') || (node->child[0]->name == '-')))
            need_left_paren = true;
          // fall through:
        case '-':
          if ((node->child[1]->type == nt_operator)
           && ((node->child[1]->name == '+')
            || (node->child[1]->name == '-')
            || ((node->name == '/') && (node->child[1]->name == '/'))))
            need_right_paren = true;
      }

      if (need_left_paren)
        printf("(");
      display(node->child[0]);
      if (need_left_paren)
        printf(")");
      printf("%c", node->name);
      if (need_right_paren)
        printf("(");
      display(node->child[1]);
      if (need_right_paren)
        printf(")");
    }
  }
}

jmp_buf error_handler;

int main()
{
  int num_expressions;
  int i;

  scanf("%d%*[\n]", &num_expressions);

  init_scanner(stdin);

  for (i = 0; i < num_expressions; i++)
  {
    node_t *node;

    if (setjmp(error_handler))
    {
      printf("Error parsing expression!\n");
      return 1;
    }

    node = do_parse();

    display(node);

    printf("\n");
  }

  return 0;
}

int yyerror()
{
  longjmp(error_handler, 1);
}

