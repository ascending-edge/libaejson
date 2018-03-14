 /* Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved */
%code top {
#include <stdio.h>
#include <ae/ae.h>

#include <malloc.h>     
     
#include <aejson/parser.h>
#include <aejson/value.h>
#include <aejson/pair.h>
#include <aejson/object.h>
     
#define JERROR_VERBOSE
#define JLTYPE aejson_parser_loc_t     

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

%type <value> value
%type <boolean> boolean
%type <pair> pair
%type <object> object members
%type <array> array elements

%start object
%% /* The grammar follows.  */


object
: '{' '}'
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     parser->result = $$;
     printf("empty object: %p\n", $$);
}
| '{' members '}'
{
     $$ = $2;
     parser->result = $$;
     printf("populated object: %p\n", $$);
}
;

pair
: t_string ':' value
{
     printf("pair: (%s)\n", $1);
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     $$->name = $1;
     $$->value = $3;
}
;

members
: pair
{
     printf("members:\n");
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_object_init(parser->e, $$, parser->pool));
     P_TRY(aejson_object_pair_add(parser->e, $$, $1));
     printf("members:\n");
     
}
| members ',' pair
{
     printf("second pair\n");
     $$ = $1;
     P_TRY(aejson_object_pair_add(parser->e, $1, $3));
}
;

array
: '[' ']'
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(ae_ptrarray_init(parser->e, $$, parser->pool, 16));
}
| '[' elements ']'
{
     $$ = $2;
}
;

elements
: value
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(ae_ptrarray_init(parser->e, $$, parser->pool, 16));
     P_TRY(ae_ptrarray_append(parser->e, $$, $1));
}
| value ',' elements
{
     $$ = $3;
     P_TRY(ae_ptrarray_append(parser->e, $$, $1));
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
     printf("object value\n");
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
                             "%d:%d %s",
                             loc->first_line, loc->first_column, msg);
}

