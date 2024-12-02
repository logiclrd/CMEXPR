#include <stdio.h>

#include "parser.tab.h"

static char last_variable_name;

static FILE *scanner_file;

void init_scanner(FILE *file)
{
  scanner_file = file;
}

int yylex()
#ifdef DEBUG
{
  int dbglex();
  char scanner_get_last_variable_name();

  int value = dbglex();

  switch (value)
  {
    case 0:       printf("YYEOF\n"); break;
    case PLUS:    printf("PLUS\n");  break;
    case MINUS:   printf("MINUS\n"); break;
    case TIMES:   printf("TIMES\n"); break;
    case DIVIDE:  printf("DIVIDE\n"); break;
    case OPENP:   printf("OPENP\n"); break;
    case CLOSEP:  printf("CLOSEP\n"); break;
    case VARIABLENAME: printf("VARIABLENAME(%c)\n", scanner_get_last_variable_name());
  }

  return value;
}

int dbglex()
#endif
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

