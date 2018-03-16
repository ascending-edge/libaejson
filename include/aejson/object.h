/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_OBJECT_H
#define _AEJSON_OBJECT_H

#include <stdio.h>

#include <ae/ae.h>
#include <aejson/pair.h>


/**
 * A parsed json object
 */
typedef struct aejson_object
{
     ae_ptrarray_t members;     /**< array of json pairs */
} aejson_object_t;

#ifdef __cplusplus
extern "C" {
#endif

     /** 
      * Prepares the object
      * 
      * @param pool where to allocate memory from
      */
     bool aejson_object_init(ae_res_t *e, aejson_object_t *self,
                             ae_pool_t *pool);


     /** 
      * Appends a pair to the object
      *
      * @param p what to append
      */
     bool aejson_object_pair_add(ae_res_t *e, aejson_object_t *self,
                                 aejson_pair_t *p);


     /** 
      * A debugging tool to provide a textual display of the object to
      * a file stream.
      *
      * @param depth provides padding; increases as it recurses
      * 
      * @param out where to write to
      */
     void aejson_object_dump(const aejson_object_t *self,
                             int depth, FILE *out);


     /** 
      * Locates a value from a parsed object using standard dot
      * notation.  For example a.b.c[4].d.
      *
      * @param pool pool used for parsing @p path
      * 
      * @param path what to locate
      * 
      * @param out the located value is placed here.  It is valid as
      * long as the pool used when parsing the object is still valid.
      */
     bool aejson_object_find(ae_res_t *e, aejson_object_t *self,
                             ae_pool_t *pool, const char *path,
                             aejson_value_t **out);

#ifdef __cplusplus
}
#endif


#endif
