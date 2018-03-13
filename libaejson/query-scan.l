 /* -*-c-*- */
%option noyywrap
%option noinput
%option nounput
%option reentrant
%option bison-bridge
%option yylineno
%{
#include <ae/ae.h>
#include <aejson/parser.h>
struct aejson_query;
#include "query-parse.h"

#define L_TRY(expr) if(!(expr)) yyterminate()
     
#define YYLTYPE aejson_parser_loc_t

int yylex(YYSTYPE *foo, YYLTYPE *loc,
          void *scanner, struct aejson_query *parser);

void yyerror(YYLTYPE *loc, void *scanner, struct aejson_query *parser,
             const char *fmt, ...);

#define YY_DECL int yylex \
		(YYSTYPE * yylval_param, YYLTYPE *loc, yyscan_t yyscanner, \
           struct aejson_query *parser)

#define YY_USER_ACTION loc->first_line = loc->last_line = yylineno; \
    loc->first_column = yycolumn; loc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

%}


CTRL [\.\[\]]
INTEGER [1-9][0-9]*
INDEX {INTEGER}|\*

%%

{CTRL} {
	/* control characters like parens, brackets, commas, and dot */
	return yytext[0];
}

{INDEX} {
     if(yytext[0] == '*')
     {
          yylval->integer = -1;
     }
     yylval->integer = 0;
     return t_index;
}


. {
     /* process default here */
     yyerror(loc, yyscanner, parser, "scan error detected");
     yyterminate();
}


%%
