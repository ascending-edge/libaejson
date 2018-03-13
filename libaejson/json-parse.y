%code top {
#include <stdio.h>
#include <ae/ae.h>

#include <malloc.h>     
     
#include <aejson/parser.h>
#include <aejson/value.h>
#include <aejson/pair.h>
#include <aejson/object.h>
     
#define YYERROR_VERBOSE
#define YYLTYPE parser_loc_t     

#define P_TRY(expr) if(!(expr)) { parser->is_err = true; YYABORT; }
}
%union {
	char *str;
     double dbl;
     int64_t integer;
     bool boolean;
     struct value *value;
     struct pair *pair;
     struct aejson_object *object;
     struct ae_ptrarray *array;
}
%{
struct parser;     
     
	int yylex(YYSTYPE *foo,
               YYLTYPE *loc,
               void *scanner,
               struct parser *parser);
	void yyerror(YYLTYPE *loc,
           void *scanner,
           struct parser *parser,
           const char *fmt, ...)
#if __GNUC__
          __attribute__ ((format (printf, 4, 5)))
#endif
          ;

%}


/* %define api.value.type {union foo} */
%define api.pure full
%define parse.error verbose
%locations
%lex-param {void *scanner}
%lex-param {struct parser *parser}
%parse-param {void *scanner}
%parse-param {struct parser *parser}

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

%% /* The grammar follows.  */


object
: '{' '}'
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     parser->result = $$;
     printf("empty object\n");     
}
| '{' members '}'
{
     $$ = $2;
     parser->result = $$;
     printf("populated object\n");
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
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(aejson_object_init(parser->e, $$, parser->pool));
     P_TRY(aejson_object_pair_add(parser->e, $$, $1));
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
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_STRING));
     $$->str = $1;
}
| t_float
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_DOUBLE));
     $$->dbl = $1;
}
| t_integer
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_INTEGER));
     $$->integer = $1;
}
| object
{
     printf("object value\n");
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_OBJECT));
     $$->object = $1;
}
| array
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_ARRAY));
     $$->array = $1;
}
| boolean
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_BOOLEAN));
     $$->boolean = $1;
}
| t_null
{
     P_TRY(ae_pool_calloc(parser->e, parser->pool, &$$, sizeof(*$$)));
     P_TRY(value_init(parser->e, $$, VALUE_TYPE_NULL));
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
void yyerror(YYLTYPE *loc, void *scanner, struct parser *self,
           const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     parser_error_set(self, loc,
                      "%d:%d %s", loc->first_line, loc->first_column, msg);
}

