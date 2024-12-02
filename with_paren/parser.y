%{

#include "parser.h"

node_t arena[500];
int arena_ptr;

void reset_arena()
{
  arena_ptr = 0;
}

node_t *alloc_node()
{
  memset(&arena[arena_ptr], 0, sizeof(node_t));
  return &arena[arena_ptr++];
}

node_t *make_variable_node(char variable_name)
{
  node_t *ret = alloc_node();

  ret->type = nt_variable;
  ret->name = variable_name;

  return ret;
}

node_t *make_operator_node(node_t *left, char operator, node_t *right)
{
  node_t *ret = alloc_node();

  ret->type = nt_operator;
  ret->child[0] = left;
  ret->name = operator;
  ret->child[1] = right;

  return ret;
}

node_t *make_parentheses_node(node_t *embedded)
{
  node_t *ret = alloc_node();

  ret->type = nt_parentheses;
  ret->child[0] = embedded;

  return ret;
}

static node_t *root_node;

node_t *do_parse()
{
  yyparse();
  return root_node;
}

%}

%union {
  struct node_t *node;
}

%type <node> Expression AdditionExpression MultiplicationExpression VariableExpression ParenthesesExpression

%start Expression

%token VARIABLENAME 1
%token PLUS 2
%token MINUS 3
%token TIMES 4
%token DIVIDE 5
%token OPENP 6
%token CLOSEP 7

%left PLUS
%left MINUS
%left TIMES
%left DIVIDE

%%

Expression : AdditionExpression
             {
               root_node = $1;
             }

AdditionExpression : AdditionExpression PLUS MultiplicationExpression
                     {
                       $$ = make_operator_node($1, '+', $3);
                     }
                   | AdditionExpression MINUS MultiplicationExpression
                     {
                       $$ = make_operator_node($1, '-', $3);
                     }
                   | MultiplicationExpression
                     {
                       $$ = $1;
                     }

MultiplicationExpression : MultiplicationExpression TIMES VariableExpression
                           {
                             $$ = make_operator_node($1, '*', $3);
                           }
                         | MultiplicationExpression DIVIDE VariableExpression
                           {
                             $$ = make_operator_node($1, '/', $3);
                           }
                         | VariableExpression
                           {
                             $$ = $1;
                           }

VariableExpression : VARIABLENAME
                     {
                       $$ = make_variable_node(scanner_get_last_variable_name());
                     }
                   | ParenthesesExpression
                     {
                       $$ = $1;
                     }

ParenthesesExpression : OPENP AdditionExpression CLOSEP
                        {
                          $$ = make_parentheses_node($2);
                        }

