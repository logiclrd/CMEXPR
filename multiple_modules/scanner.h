#ifndef SCANNER_H
#define SCANNER_H

void init_scanner();
int yylex();
char scanner_get_last_variable_name();

#endif // SCANNER_H
