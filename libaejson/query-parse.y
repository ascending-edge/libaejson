 /* -*-c-*- */
 /* Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved */
%code top {
#include <stdio.h>
#include <ae/ae.h>

#include <aejson/loc.h>     
#include <aejson/query.h>
#include <aejson/node.h>


struct aejson_query;
     
#define QERROR_VERBOSE
#define QLTYPE aejson_loc_t     

#define P_TRY(expr) if(!(expr)) { YYABORT; }
}
%union {
     int64_t integer;
     char *str;
     struct aejson_node *node;
}

%{
struct aejson_query;
     
	int qlex(QSTYPE *foo,
              QLTYPE *loc,
              void *scanner,
              struct aejson_query *parser);
	void qerror(QLTYPE *loc,
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


%token t_integer
%token t_string

%type <str> t_string
%type <integer> t_integer
%type <node> node query

%% 

query
: node
{
     $$ = $1;
     parser->result = $$;
}
| query '.' node
{
     aejson_node_t *x = $1;
     /* well this sucks....there's got to be a better way */
     while(x->next)
     {
          x = x->next;
     }
     x->next = $3;
     $$ = $1;
     parser->result = $$;
}
;

node
: t_string
{
     /* printf("sup"); */
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     $$->name = $1;
     $$->index = -1;
}
| t_string '[' t_integer ']'
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     $$->name = $1;
     $$->index = $3;
}
;



%%
#include <stdio.h>
void qerror(QLTYPE *loc, void *scanner, struct aejson_query *self,
           const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     aejson_query_error_set(self, loc,
                             "%d:%d %s",
                             loc->first_line, loc->first_column, msg);
}

