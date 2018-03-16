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


%token t_node
%type <node> query t_node

%% 

query
: t_node
{
     parser->result = $$;
}
| query t_node
{
     aejson_node_t *x = $1;
     /* well this sucks....there's got to be a better way */
     while(x->next)
     {
          x = x->next;
     }
     x->next = $2;
     $$ = $1;
     parser->result = $$;
}
;

 /* wrap */
 /* : t_node */
 /* { */
 /*      P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$))); */
 /*      memcpy($$, &$1, sizeof(*$$)); */
 /* } */
 /* ; */

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

