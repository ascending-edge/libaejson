 /* -*-c-*- */
%option noyywrap
%option noinput
%option nounput
%option reentrant
%option bison-bridge
%option yylineno
%option prefix="q"
%option outfile="query-scan.c"
%{
#include <ae/ae.h>
#include <aejson/parser.h>
struct aejson_query;
#include "query-parse.h"

#define QLTYPE aejson_parser_loc_t     

void qerror(QLTYPE *loc, void *scanner, struct aejson_query *self,
            const char *fmt, ...);


/* Substitute the type names.  */
#define YYSTYPE         QSTYPE
#define YYLTYPE         QLTYPE
/* Substitute the variable and function names.  */
#define yyparse         qparse
#define yylex           qlex
#define yyerror         qerror
#define yydebug         qdebug
#define yynerrs         qnerrs


#define L_TRY(expr) if(!(expr)) yyterminate()
     
#define QLTYPE aejson_parser_loc_t

int jlex(QSTYPE *foo, QLTYPE *loc,
          void *scanner, struct aejson_query *parser);

void jerror(QLTYPE *loc, void *scanner, struct aejson_query *parser,
             const char *fmt, ...);

#define YY_DECL int qlex \
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
     else
     {
          if(sscanf(yytext, "%d", &yylval->integer) != 1)
          {
               yyterminate();
          }
     }
     return t_index;
}

[[:alnum:]]+ {
     yylval->str = strdup(yytext);
     return t_string;
}

. {
     /* process default here */
     yyerror(loc, yyscanner, parser, "scan error detected");
     yyterminate();
}


%%
