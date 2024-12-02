#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

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

#define VARIABLENAME 1
#define PLUS 2
#define MINUS 3
#define TIMES 4
#define DIVIDE 5
#define OPENP 6
#define CLOSEP 7

typedef union YYSTYPE {
  struct node_t *node;
} YYSTYPE;

typedef enum { false, true } bool;

static char last_variable_name;

static FILE *scanner_file;

void init_scanner(FILE *file)
{
  scanner_file = file;
}

int yylex()
{
  while (1)
  {
    int ch = getc(scanner_file);

    switch (ch)
    {
      case '+': return PLUS;
      case '-': return MINUS;
      case '*': return TIMES;
      case '/': return DIVIDE;
      case '(': return OPENP;
      case ')': return CLOSEP;
      case '\n': return 0;
      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
      case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
      case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        last_variable_name = ch;
        return VARIABLENAME;
    }
  }
}

char scanner_get_last_variable_name()
{
  return last_variable_name;
}

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
        case '/':
          need_right_paren = (node->child[1]->type == nt_operator);
        case '*':
          if ((node->child[0]->type == nt_operator)
           && ((node->child[0]->name == '+') || (node->child[0]->name == '-')))
            need_left_paren = true;
          // fall through:
        case '-':
          if ((node->child[1]->type == nt_operator)
           && ((node->child[1]->name == '+')
            || (node->child[1]->name == '-')))
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
char *error_message;

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
      printf("Error parsing expression! Message: %s\n", error_message ? error_message : "(none)");
      return 1;
    }

    node = do_parse();

    display(node);

    printf("\n");
  }

  return 0;
}

int yyerror(char *message)
{
  error_message = message;
  longjmp(error_handler, 1);
}

/* A Bison parser, made by GNU Bison 1.875.  */

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

static node_t *root_node;

node_t *do_parse()
{
  yyparse();
  return root_node;
}

//#define YYSTACK_ALLOC __builtin_alloca
#define YYSTACK_ALLOC alloca

union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

#define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))

#define YYSTACK_RELOCATE(Stack)                                         \
    do                                                                  \
    {                                                                   \
      YYSIZE_T yynewbytes;                                              \
      YYCOPY (&yyptr->Stack, Stack, yysize);                            \
      Stack = &yyptr->Stack;                                            \
      yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
      yyptr += yynewbytes / sizeof (*yyptr);                            \
    }                                                                   \
    while (0)

typedef signed char yysigned_char;

#define YYFINAL  9
#define YYLAST   14

#define YYNTOKENS  10

#define YYUNDEFTOK  2
#define YYMAXUTOK   257

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

static const unsigned char yytranslate[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2
};

static const unsigned char yyr1[] =
{
       0,    10,    11,    12,    12,    12,    13,    13,    13,    14,
      14,    15
};

static const unsigned char yyr2[] =
{
       0,     2,     1,     3,     3,     1,     3,     3,     1,     1,
       1,     3
};

static const unsigned char yydefact[] =
{
       0,     9,     0,     0,     2,     5,     8,    10,     0,     1,
       0,     0,     0,     0,    11,     3,     4,     6,     7
};

static const yysigned_char yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7
};

#define YYPACT_NINF -5
static const yysigned_char yypact[] =
{
      -1,    -5,    -1,     6,     4,    -3,    -5,    -5,    -4,    -5,
      -1,    -1,    -1,    -1,    -5,    -3,    -3,    -5,    -5
};

static const yysigned_char yypgoto[] =
{
      -5,    -5,    10,     0,     1,    -5
};

#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      10,    11,     1,    12,    13,    14,     9,     2,    10,    11,
      15,    16,     8,    17,    18
};

static const unsigned char yycheck[] =
{
       4,     5,     3,     6,     7,     9,     0,     8,     4,     5,
      10,    11,     2,    12,    13
};

static const unsigned char yystos[] =
{
       0,     3,     8,    11,    12,    13,    14,    15,    12,     0,
       4,     5,     6,     7,     9,    13,    13,    14,    14
};

#define YYSIZE_T unsigned int

#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	return 0
#define YYABORT		return 1

#define YYTERROR	1

#define YYLEX yylex ()

#define YYINITDEPTH 200
#define YYMAXDEPTH 10000

int yychar;
YYSTYPE yylval;
int yynerrs;

int
yyparse ()
{
  register int yystate;
  register int yyn;
  int yyerrstatus;
  int yytoken = 0;

  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;
  YYSTYPE yyval;

  int yylen;


  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

 yynewstate:
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      YYSIZE_T yysize = yyssp - yyss + 1;

      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
      }

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  if (yychar == YYEMPTY)
      yychar = YYLEX;

  if (yychar <= YYEOF)
      yychar = yytoken = YYEOF;
  else
      yytoken = YYTRANSLATE (yychar);

  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;

  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

yyreduce:
  yylen = yyr2[yyn];
  yyval = yyvsp[1-yylen];

  switch (yyn)
    {
        case 2: root_node = yyvsp[0].node; break;

        case 5:
        case 8:
        case 10: yyval.node = yyvsp[0].node; break;

        case 3: yyval.node = make_operator_node(yyvsp[-2].node, '+', yyvsp[0].node); break;
        case 4: yyval.node = make_operator_node(yyvsp[-2].node, '-', yyvsp[0].node); break;
        case 6: yyval.node = make_operator_node(yyvsp[-2].node, '*', yyvsp[0].node); break;
        case 7: yyval.node = make_operator_node(yyvsp[-2].node, '/', yyvsp[0].node); break;

        case 9: yyval.node = make_variable_node(scanner_get_last_variable_name()); break;

        case 11: yyval.node = yyvsp[-1].node; break;
    }

  yyvsp -= yylen;
  yyssp -= yylen;

  *++yyvsp = yyval;

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;

yyerrlab:
  if (!yyerrstatus)
    {
      ++yynerrs;
	yyerror ("syntax error");
    }

  if (yyerrstatus == 3)
    {
      if (yychar == YYEOF)
        {
          YYPOPSTACK;
	  while (yyss < yyssp)
	      YYPOPSTACK;
	  YYABORT;
        }

      yychar = YYEMPTY;
    }

  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      if (yyssp == yyss)
	YYABORT;

      yyvsp--;
      yystate = *--yyssp;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yystate = yyn;
  goto yynewstate;

yyoverflowlab:
  yyerror ("parser stack overflow");
  return 2;
}

