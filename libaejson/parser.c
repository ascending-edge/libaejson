#include <aejson/parser.h>

#include "json-parse.h"

int yylex_init(void**);
int yylex_destroy(void*);


bool parser_init(ae_res_t *e, parser_t *self)
{
     return true;
}


void parser_error_set(parser_t *self, const parser_loc_t *loc,
                      const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     ae_res_err(self->e, "%s", msg);
     self->is_err = true;
}


bool parser_parse(ae_res_t *e, parser_t *self, ae_pool_t *pool,
                  aejson_object_t **out)
{
     self->pool = pool;
     self->e = e;
     self->is_err = false;
     
     void* scanner;
     yylex_init(&scanner);

     if(yyparse(scanner, self) != 0)
     {
          self->is_err = true;
     }
     *out = self->result;
     yylex_destroy(scanner);
     return !self->is_err;
}


bool parser_comment_add(parser_t *self,
                        const parser_loc_t  *loc,
                        const char *comment)
{
     /* printf("ignoring comment: %s\n", comment); */
     return true;
}

bool parser_string_start(parser_t *self, const parser_loc_t *loc)
{
     if(self->string_literal)
     {
          ae_res_err(self->e, "must not start a literal while processing one!");
          return false;
     }
     self->string_literal_max = 512;
     AE_TRY(ae_pool_calloc(self->e, self->pool,
                           &self->string_literal,
                           self->string_literal_max));
     return true;
}

bool parser_string_end(parser_t *self, const parser_loc_t *loc,
                       char **out)
{
     if(!self->string_literal)
     {
          ae_res_err(self->e, "not currently processing a string literal");
     }
     *out = self->string_literal;
     self->string_literal = NULL;
     self->string_literal_len = 0;
     return true;
}
     
bool parser_string_add_char(parser_t *self,
                            const parser_loc_t *loc, char c)
{
     /* Do we need to resize? */
     if(self->string_literal_len == self->string_literal_max - 1)
     {
          size_t new_max = self->string_literal_max + 512;
          char *new_str = NULL;
          AE_TRY(ae_pool_realloc(self->e, self->pool,
                                 self->string_literal, self->string_literal_max,
                                 &new_str, new_max));
          
          self->string_literal = new_str;
          self->string_literal_max = new_max;
     }
     self->string_literal[self->string_literal_len++] = c;
     return true;
}
                              
