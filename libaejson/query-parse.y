%code top {
#include <ae/ae.h>
#include <aejson/parser.h>     
#define YYERROR_VERBOSE
#define YYLTYPE aejson_parser_loc_t     
#define P_TRY(expr) if(!(expr)) { YYABORT; }
struct aejson_query;
     
}
%union {
     int64_t integer;
}

%{
struct aejson_query;
     
     int yylex(YYSTYPE *foo,
               YYLTYPE *loc,
               void *scanner,
               struct aejson_query *parser);
	void yyerror(YYLTYPE *loc,
                  void *scanner,
                  struct aejson_query *parser,
                  const char *fmt, ...)
#if __GNUC__
          __attribute__ ((format (printf, 4, 5)))
#endif
          ;
     
%}
%define api.prefix {q}
%define api.pure full
%define parse.error verbose
%locations
%parse-param {void *scanner}
%parse-param {struct aejson_query *parser}
%lex-param {void *scanner}
%lex-param {struct aejson_query *parser}


%token t_index
%token t_string

%% 

query
: node
| query '.' node
;

node
: t_string
| t_string '[' t_index ']'
;



%%
#include <stdio.h>
void yyerror(YYLTYPE *loc, void *scanner, struct aejson_query *self,
           const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     /* aejson_query_error_set(self, loc, */
     /*                         "%d:%d %s", */
     /*                         loc->first_line, loc->first_column, msg); */

     fprintf(stderr, "%d:%d %s", 
             loc->first_line, loc->first_column, msg);
}

