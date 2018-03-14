/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 * 
 */
#include <aejson/parser.h>

#include "json-parse.h"

int jlex_init(void**);
int jlex_destroy(void*);


bool aejson_parser_init(ae_res_t *e, aejson_parser_t *self)
{
     return true;
}


void aejson_parser_error_set(aejson_parser_t *self,
                             const aejson_parser_loc_t *loc,
                             const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     ae_res_err(self->e, "%s", msg);
}


bool aejson_parser_parse(ae_res_t *e, aejson_parser_t *self,
                         ae_pool_t *pool, aejson_object_t **out)
{
     self->pool = pool;
     self->e = e;
     self->result = NULL;
     
     void* scanner;
     jlex_init(&scanner);

     jparse(scanner, self);
     jlex_destroy(scanner);     
     *out = self->result;
     return self->result != NULL;
}


bool aejson_parser_comment_add(aejson_parser_t *self,
                               const aejson_parser_loc_t  *loc,
                               const char *comment)
{
     /* printf("ignoring comment: %s\n", comment); */
     return true;
}


bool aejson_parser_string_start(aejson_parser_t *self,
                                const aejson_parser_loc_t *loc)
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


bool aejson_parser_string_end(aejson_parser_t *self,
                              const aejson_parser_loc_t *loc,
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


bool aejson_parser_string_add_char(aejson_parser_t *self,
                                   const aejson_parser_loc_t *loc, char c)
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
                              
