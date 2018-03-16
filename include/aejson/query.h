/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 *
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_QUERY_H
#define _AEJSON_QUERY_H

#include <ae/ae.h>
#include <aejson/strlit.h>
#include <aejson/node.h>


/**
 * This is used by the scanner and parser when parsing the query
 * "language".
 */
typedef struct aejson_query
{
     ae_res_t *e;               /**< holds errors and warnings */
     ae_pool_t *pool;           /**< where to allocate memory from */
     aejson_strlit_t strlit;    /**< used to build string literals */

     aejson_node_t *result;     /**< The parse result is stored here */

     aejson_node_t *node;       /**< Used by the scanner */
} aejson_query_t;

#ifdef __cplusplus
extern "C" {
#endif

     /** 
      * Prepares the query object for use.
      */
     bool aejson_query_init(ae_res_t *e, aejson_query_t *self);


     /** 
      * Parses the query.
      *
      * @param pool where to allocate from
      * 
      * @param in the query (ex: "a.b.c[3].a")
      * 
      * @param out where the result is stored
      */
     bool aejson_query_parse(ae_res_t *e, aejson_query_t *self,
                             ae_pool_t *pool, const char *in,
                             aejson_node_t **out);


     /** 
      * Used by the scanner and parser to set errors.
      *
      * @param loc where the error occured
      * 
      * @param fmt printf style format string
      */
     void aejson_query_error_set(aejson_query_t *self,
                                 const aejson_loc_t *loc,
                                 const char *fmt, ...);


     /** 
      * Adds a character to the current string literal.  All expansion
      * of escape sequences must be performed outside of this class.
      *
      * @param loc where the character was located in the input.
      * 
      * @param c the character to add
      */
     bool aejson_query_strlit_add(aejson_query_t *self,
                                  const aejson_loc_t *loc,
                                  char c);

     /** 
      * Ends the current string literal
      *
      * @param loc where the end was in the input stream
      * 
      * @param out the string literal pointer is copied here.  It is
      * valid as long as the memory pool that was used for parsing is
      * valid.
      */
     bool aejson_query_strlit_end(aejson_query_t *self,
                                  const aejson_loc_t *loc,
                                  char **out);

     /** 
      * Creates a new node.  This is used by the scanner.
      */
     bool aejson_query_new_node(aejson_query_t *self);

#ifdef __cplusplus
}
#endif


#endif
