 /* -*-c-*- */
 /* Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved */
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
#include <aejson/loc.h>
#include <aejson/query.h>
struct aejson_query;
#include "query-parse.h"

#define QLTYPE aejson_loc_t     

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
     
#define QLTYPE aejson_loc_t

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

DOT \.
INTEGER [0-9]+
HEXCHAR [0123456789abcdef]

%%

\[{INTEGER}\] {
     if(sscanf(&yytext[1], "%"SCNu64, &parser->node->index) != 1)
     {
          aejson_query_error_set(parser, loc,
                                 "%s is not an integer", &yytext[1]);
          yyterminate();
     }
}

{DOT} {
     L_TRY(aejson_query_strlit_end(parser, loc, &parser->node->name));
     yylval->node = parser->node;
     L_TRY(aejson_query_new_node(parser));
     return t_node;
}

\\[bfnrt\.\[\]] {
     char new_char = '\0';
     switch(yytext[1])
     {
     case '[':
     case ']':          
     case '.':          
          new_char = yytext[1];
          break;
     case 'b':
          new_char = '\b';
          break;
     case 'f':
          new_char = '\f';          
          break;
     case 'n':
          new_char = '\n';          
          break;
     case 'r':
          new_char = '\r';          
          break;
     case 't':
          new_char = '\t';          
          break;
     default:
          fprintf(stderr, "%s:%d impossible condition", __FILE__, __LINE__);
          abort();
          break;
     }
     L_TRY(aejson_query_strlit_add(parser, loc, new_char));
}

\\u{HEXCHAR}{HEXCHAR}{HEXCHAR}{HEXCHAR} {
     uint16_t uni = 0;
     if(sscanf(&yytext[2], "%"SCNx16, &uni) != 1)
     {
          char invalid[5];
          memcpy(invalid, &yytext[2], 4);
          invalid[4] = '\0';
          ae_res_err(parser->e,
                     "conversion error, invalid hex number: %s",
                     invalid);
          yyterminate();
     }
     L_TRY(aejson_query_strlit_add(parser, loc, uni));
}

. {
     switch(yytext[0])
     {
     default:
          L_TRY(aejson_query_strlit_add(parser, loc, yytext[0]));          
          break;
     }
}



<<EOF>> {
     L_TRY(aejson_query_strlit_end(parser, loc, &parser->node->name));
     if(parser->node->name[0] != '\0')
     {
          yylval->node = parser->node;
          AE_TRY(aejson_query_new_node(parser));
          return t_node;
     }
     yyterminate();
}

%%
