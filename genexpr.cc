#include <iostream>
#include <cstdlib>

using namespace std;

enum node_type
{
  nt_variable,
  nt_operator,
  nt_parentheses,
};

struct node_t
{
  node_type type;
  node_t *child[2];
  char name;

  node_t() { child[0] = child[1] = NULL; }
};

void output(node_t *root)
{
  switch (root->type)
  {
    case nt_variable: printf("%c", root->name); break;
    case nt_parentheses: printf("("); output(root->child[0]); printf(")"); break;
    case nt_operator: output(root->child[0]); printf("%c", root->name); output(root->child[1]); break;
  }
}

node_t *walk_to_leaf_node(node_t *root, int &leafs_to_skip)
{
  if (root->type == nt_variable)
  {
    if (leafs_to_skip > 0)
    {
      leafs_to_skip--;
      return NULL;
    }
    else
      return root;
  }

  if (root->child[0] != NULL)
  {
    node_t *ret = walk_to_leaf_node(root->child[0], leafs_to_skip);
    if (ret != NULL)
      return ret;
  }

  if (root->child[1] != NULL)
    return walk_to_leaf_node(root->child[1], leafs_to_skip);

  return NULL;
}

void gen_expr()
{
  node_t arena[2000];
  int arena_allocated = 0;

  node_t *root = &arena[arena_allocated++];

  root->type = nt_variable;
  root->name = 'a';

  int num_chars = 1;
  int num_leaf_nodes = 1;

  while (num_chars < 246)
  {
    int node_to_modify = rand() % num_leaf_nodes;

    node_t *node = walk_to_leaf_node(root, node_to_modify);

    if ((node == NULL) || (node->type != nt_variable)) // wtf?!
    {
      cerr << "OMG FAILED TO FIND LEAF NODE!" << endl;
      continue;
    }

    bool parentheses = (rand() < (RAND_MAX / 2));

    if (parentheses)
    {
      node_t *child = &arena[arena_allocated++];

      memcpy(child, node, sizeof(node_t));

      node->type = nt_parentheses;
      node->child[0] = child;

      num_chars += 2;
    }
    else
    {
      node_t *left, *right;

      left = &arena[arena_allocated++];
      right = &arena[arena_allocated++];

      memcpy(left, node, sizeof(node_t));
      memcpy(right, node, sizeof(node_t));
      right->name = 'a' + rand() % 26;

      node->type = nt_operator;
      node->child[0] = left;
      node->child[1] = right;
      node->name = "+-*/"[rand() & 3];
    }
  }

  output(root);
  printf("\n");
}

int main()
{
  cout << 10000 << endl;

  for (int i=0; i < 10000; i++)
    gen_expr();
}

