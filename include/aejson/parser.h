#ifndef _PARSER_H
#define _PARSER_H

#include <ae/ae.h>

#include "object.h"

typedef struct parser_loc
{
     int first_line;
     int first_column;
     int last_line;
     int last_column;
} parser_loc_t;


typedef struct parser
{
     ae_res_t *e;
     ae_pool_t *pool;
     bool is_err;

     aejson_object_t *result;

     size_t string_literal_max;
     size_t string_literal_len;
     char *string_literal;
} parser_t;

#ifdef __cplusplus
extern "C" {
#endif

     bool parser_init(ae_res_t *e, parser_t *self);
     bool parser_parse(ae_res_t *e,
                       parser_t *self,
                       ae_pool_t *pool,
                       aejson_object_t **out);

     
     bool parser_comment_add(parser_t *self,
                             const parser_loc_t  *loc,
                             const char *comment);
     void parser_error_set(parser_t *self,
                           const parser_loc_t  *loc,
                           const char *fmt, ...)
#if __GNUC__
          __attribute__ ((format (printf, 3, 4)))
#endif
          ;

     bool parser_string_start(parser_t *self, const parser_loc_t *loc);
     bool parser_string_end(parser_t *self, const parser_loc_t *loc,
                            char **out);
     
     bool parser_string_add_char(parser_t *self,
                                 const parser_loc_t *loc, char c);
                              
     

#ifdef __cplusplus
}
#endif


#endif
