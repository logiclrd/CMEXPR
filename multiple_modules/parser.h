#ifndef PARSER_H
#define PARSER_H

typedef enum node_type
{
  nt_variable,
  nt_operator,
} node_type;

typedef struct node_t
{
  node_type type;
  struct node_t *child[2];
  char name;
} node_t;

void reset_arena();
struct node_t *do_parse();

#endif // PARSER_H
