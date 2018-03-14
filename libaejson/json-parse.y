 /* Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved */
%code top {
#include <stdio.h>
#include <ae/ae.h>

#include <malloc.h>     

#include <aejson/loc.h>     
#include <aejson/parser.h>
#include <aejson/value.h>
#include <aejson/pair.h>
#include <aejson/object.h>
     
#define JERROR_VERBOSE
#define JLTYPE aejson_loc_t     

#define P_TRY(expr) if(!(expr)) { YYABORT; }
}
%union {
	char *str;
     double dbl;
     int64_t integer;
     bool boolean;
     struct aejson_value *value;
     struct aejson_pair *pair;
     struct aejson_object *object;
     struct ae_ptrarray *array;
}
%{
struct aejson_parser;     
     
	int jlex(JSTYPE *foo,
              JLTYPE *loc,
              void *scanner,
              struct aejson_parser *parser);
	void jerror(JLTYPE *loc,
                 void *scanner,
                 struct aejson_parser *parser,
                 const char *fmt, ...)
#if __GNUC__
          __attribute__ ((format (printf, 4, 5)))
#endif
          ;

%}
/* %define api.value.type {union foo} */
%define api.prefix {j}
%define api.pure full
%define parse.error verbose
%locations
%lex-param {void *scanner}
%lex-param {struct aejson_parser *parser}
%parse-param {void *scanner}
%parse-param {struct aejson_parser *parser}

%token t_true
%token t_false
%token t_null
%token t_float
%type <dbl> t_float
%token t_integer
%type <integer> t_integer

%token t_string
%type <str> t_string

%type <value> value array elements
%type <boolean> boolean
%type <pair> pair
%type <object> object members


%% 


object
: '{' '}'
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     parser->result = $$;
}
| '{' members '}'
{
     $$ = $2;
     parser->result = $$;
}
;


pair
: t_string ':' value
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     $$->name = $1;
     $$->value = $3;
}
;


members
: pair
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_object_init(parser->e, $$, parser->pool));
     P_TRY(aejson_object_pair_add(parser->e, $$, $1));
}
| members ',' pair
{
     $$ = $1;
     P_TRY(aejson_object_pair_add(parser->e, $1, $3));
}
;


array
: '[' ']'
{
     $$ = NULL;
}
| '[' elements ']'
{
     $$ = $2;
}
;


elements
: value
| value ',' elements
{
     $$ = $1;
     $$->next = $3;
}
;


value
: t_string
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_STRING));
     $$->str = $1;
}
| t_float
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_DOUBLE));
     $$->dbl = $1;
}
| t_integer
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_INTEGER));
     $$->integer = $1;
}
| object
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_OBJECT));
     $$->object = $1;
}
| array
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_ARRAY));
     $$->array = $1;
}
| boolean
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_BOOLEAN));
     $$->boolean = $1;
}
| t_null
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_value_init(parser->e, $$, AEJSON_VALUE_TYPE_NULL));
}
;


boolean
: t_true
{
     $$ = true;
}
| t_false
{
     $$ = false;
}
;


%%
#include <stdio.h>
void jerror(JLTYPE *loc, void *scanner, struct aejson_parser *self,
           const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     aejson_parser_error_set(self, loc,
                             "%zd:%zd %s",
                             loc->first_line, loc->first_column, msg);
}
