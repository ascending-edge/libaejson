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
     aejson_node_t *node;
     uint64_t index;
     char *str;
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


%token t_id t_index
%type <node> index indices node id query
%type <index> t_index
%type <str> t_id
%% 

start
: query
{
     parser->result = $1;
}
;

query
: node
| node '.' query
{
     P_TRY(aejson_query_node_append(parser, $3));
     $$ = $1;
}
;

node
: id indices
{
     $1->next = $2;
     $$ = $1;
}
| id 
;

id
: t_id
{
     P_TRY(ae_pool_alloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     $$->next = NULL;
     $$->type = AEJSON_NODE_TYPE_ID;
     $$->id = $1;
}
;

indices
: index
| index indices
{
     $1->next = $2;
     $$ = $1;
}
;

index
: t_index
{
     P_TRY(ae_pool_alloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     $$->next = NULL;
     $$->type = AEJSON_NODE_TYPE_INDEX;
     $$->index = $1;
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

