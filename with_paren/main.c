#include <setjmp.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"

void display(node_t *node, int indent)
{
  static char indent_chars[500];

  indent_chars[indent] = 0;

  printf("%s", indent_chars);

  if (indent >= 3)
    switch (indent_chars[indent - 3])
    {
      case '`':
        indent_chars[indent - 3] = ' ';
        // fall through:
      case '|':
        indent_chars[indent - 2] = ' ';
    }

  switch (node->type)
  {
    case nt_variable: printf("%c\n", node->name); break;
    case nt_operator:
      printf("%c\n", node->name);
      strcpy(indent_chars + indent, "|- "); display(node->child[0], indent + 3);
      strcpy(indent_chars + indent, "`- "); display(node->child[1], indent + 3);
      break;
    case nt_parentheses:
      printf("(\n");
      indent_chars[indent] = ' ';
      display(node->child[0], indent + 1);
      indent_chars[indent] = 0;
      printf("%s)\n", indent_chars);
      break;
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

    printf("before:\n");
    display(node, 0);

    simplify(node);

    printf("after:\n");
    display(node, 0);
  }

  return 0;
}

int yyerror()
{
  longjmp(error_handler, 1);
}

