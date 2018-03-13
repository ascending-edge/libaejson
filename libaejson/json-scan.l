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
#include "json-parse.h"

#define L_TRY(expr) if(!(expr)) yyterminate()
     
#define YYLTYPE parser_loc_t
     
int yylex(YYSTYPE *foo, YYLTYPE *loc, void *scanner, struct parser *parser);
void yyerror(YYLTYPE *loc, void *scanner, struct parser *parser,
             const char *fmt, ...)
     ;


#define YY_DECL int yylex \
		(YYSTYPE * yylval_param, YYLTYPE *loc, yyscan_t yyscanner, struct parser *parser)

#define YY_USER_ACTION loc->first_line = loc->last_line = yylineno; \
    loc->first_column = yycolumn; loc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

     bool expand_string(struct parser *parser,
                        const parser_loc_t *loc,
                        const char *in, size_t in_len,
                        char **out);

%}


DIGIT1to9 [1-9]
DIGIT [0-9]
DIGITS {DIGIT}+

HEXCHAR [0123456789abcdef]

E [eE]+[\+\-]?{DIGIT}+

DBL_QUOTE \"

CTRL [\{\}\[\],\:]

COMMENT_SINGLE_LINE \/\/.*\n

%x COND_STRING
%%

{CTRL} {
	/* control characters like parens, brackets, commas, and dot */
	return yytext[0];
}

-?{DIGITS} {
     if(sscanf(yytext, "%"SCNd64, &yylval->integer) != 1)
     {
          parser_error_set(parser, loc, "integer scan error: %s", yytext);
          yyterminate();
     }
     return t_integer;
}

-?{DIGITS}+\.{DIGITS}+{E}?  {
     /* simple float -xxx.xxx */
     if(sscanf(yytext, "%lg", &yylval->dbl) != 1)
     {
          parser_error_set(parser, loc, "double scan error: %s", yytext);
          yyterminate();
     }
     return t_float;
}


true {
     return t_true;
}

false {
     return t_false;
}

null {
     return t_null;
}

{DBL_QUOTE} {
     BEGIN(COND_STRING);
     L_TRY(parser_string_start(parser, loc));
}


<COND_STRING>\\[bfnrt\"\\/] {
     char new_char = '\0';
     switch(yytext[1])
     {
     case '"':
     case '/':
     case '\\':
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
     L_TRY(parser_string_add_char(parser, loc, new_char));
}

<COND_STRING>[[:print:]] {
     switch(yytext[0])
     {
     case '"':
          L_TRY(parser_string_add_char(parser, loc, yytext[0]));
          BEGIN(INITIAL);
          return t_string;
          break;
     case '\n':
          yyerror(loc, yyscanner, parser, "scan error detected");
          yyterminate();
          break;
     default:
          L_TRY(parser_string_add_char(parser, loc, yytext[0]));
          break;
     }
}

<COND_STRING>\\u{HEXCHAR}{HEXCHAR}{HEXCHAR}{HEXCHAR} {
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
     L_TRY(parser_string_add_char(parser, loc, uni));
}


{COMMENT_SINGLE_LINE} {
     L_TRY(parser_comment_add(parser, loc, &yytext[2]));
}

[ \t\n] {
	/* whitespace */
}

. {
     /* process default here */
     yyerror(loc, yyscanner, parser, "scan error detected");
     yyterminate();
}


%%
