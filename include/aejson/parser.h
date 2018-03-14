/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_PARSER_H
#define _AEJSON_PARSER_H

#include <ae/ae.h>

#include <aejson/object.h>

typedef struct aejson_parser_loc
{
     int first_line;
     int first_column;
     int last_line;
     int last_column;
} aejson_parser_loc_t;


typedef struct aejson_parser
{
     ae_res_t *e;
     ae_pool_t *pool;

     aejson_object_t *result;

     size_t string_literal_max;
     size_t string_literal_len;
     char *string_literal;
} aejson_parser_t;

#ifdef __cplusplus
extern "C" {
#endif

     bool aejson_parser_init(ae_res_t *e, aejson_parser_t *self);
     bool aejson_parser_parse(ae_res_t *e, aejson_parser_t *self,
                              ae_pool_t *pool, aejson_object_t **out);
     
     bool aejson_parser_comment_add(aejson_parser_t *self,
                                    const aejson_parser_loc_t  *loc,
                                    const char *comment);
     void aejson_parser_error_set(aejson_parser_t *self,
                                  const aejson_parser_loc_t  *loc,
                                  const char *fmt, ...)
#if __GNUC__
          __attribute__ ((format (printf, 3, 4)))
#endif
          ;

     bool aejson_parser_string_start(aejson_parser_t *self,
                                     const aejson_parser_loc_t *loc);
     bool aejson_parser_string_end(aejson_parser_t *self,
                                   const aejson_parser_loc_t *loc,
                                   char **out);
     
     bool aejson_parser_string_add_char(aejson_parser_t *self,
                                        const aejson_parser_loc_t *loc, char c);
                              
     

#ifdef __cplusplus
}
#endif


#endif
