/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_PARSER_H
#define _AEJSON_PARSER_H

#include <ae/ae.h>

#include <aejson/loc.h>
#include <aejson/object.h>
#include <aejson/strlit.h>


/**
 * This class is used to parse json input
 */
typedef struct aejson_parser
{
     ae_res_t *e;               /**< holds parse error messages */
     ae_pool_t *pool;           /**< where memory is allocated from */

     aejson_object_t *result;   /**< where the parse tree resides */

     aejson_strlit_t strlit;    /**< used by the scanner to build
                                 * extract string literals */
} aejson_parser_t;


#ifdef __cplusplus
extern "C" {
#endif

     /** 
      * initializes
      */
     bool aejson_parser_init(ae_res_t *e, aejson_parser_t *self);

     /** 
      * Parses the @p json text
      *
      * @param pool where to allocate memory from
      * 
      * @param json json to parse
      * 
      * @param out where to put the parse tree
      */
     bool aejson_parser_parse(ae_res_t *e, aejson_parser_t *self,
                              ae_pool_t *pool,
                              const char *json,
                              aejson_object_t **out);


     /** 
      * Parses JSON from @p in file. 
      *
      * @param pool where to allocate memory from
      * 
      * @param in where to read JSON from
      * 
      * @param out where to put the parse tree
      */
     bool aejson_parser_parse_file(ae_res_t *e, aejson_parser_t *self,
                                   ae_pool_t *pool,
                                   FILE *in,
                                   aejson_object_t **out);

     /** 
      * This is used by the scanner to add a comment to the parse
      * tree.  Currently this does nothing.  In the future I might
      * retain the comments for future output.
      *
      * @param loc location of the comment
      * 
      * @param comment comment text
      */
     bool aejson_parser_comment_add(aejson_parser_t *self,
                                    const aejson_loc_t  *loc,
                                    const char *comment);

     /** 
      * Scanner and parser report errors here.
      *
      * @param loc where the error occurred
      * 
      * @param fmt printf style format string
      */
     void aejson_parser_error_set(aejson_parser_t *self,
                                  const aejson_loc_t  *loc,
                                  const char *fmt, ...)
#if __GNUC__
          __attribute__ ((format (printf, 3, 4)))
#endif
          ;

     /** 
      * This is used by the scanner to build a string literal.  This
      * is called when a string literal is first detected.
      *
      * @param loc where the string literal is located
      */
     bool aejson_parser_strlit_start(aejson_parser_t *self,
                                     const aejson_loc_t *loc);

     /** 
      * This is called to continue building a string literal one
      * character at a time.  This does no unescape anything -- the
      * scanner does that.
      *
      * @param loc where the character is located
      * 
      * @param c character to append
      */
     bool aejson_parser_strlit_add(aejson_parser_t *self,
                                   const aejson_loc_t *loc,
                                   char c);

     /** 
      * This is called when the end of a string literal is detected by
      * the scanner.  
      *
      * @param loc where the end was located
      * 
      * @param out where to put the string literal 
      */
     bool aejson_parser_strlit_end(aejson_parser_t *self,
                                   const aejson_loc_t *loc,
                                   char **out);
     
#ifdef __cplusplus
}
#endif


#endif
