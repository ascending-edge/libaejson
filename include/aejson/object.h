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
 * A parsed json object.  This is a pointer array of aejson_pair_t
 * objects.  A pair consists of a name and a value.
 *
 * The parser returns one object.
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
                             ae_pool_t *pool, aejson_value_t **out,
                             const char *fmt, ...)
#ifdef __GNUC__
          __attribute__((format (printf, 5, 6)))
#endif          
          ;

     /** 
      * Finds an integer, returns an error if not found or if the
      * value is not an integer.
      *
      * @param pool pool used for parsing query string
      * 
      * @param out if found this contains the integer located at the
      * specified path.
      *
      * @param fmt printf style format string which specifies the
      * query path.
      */
     bool aejson_object_find_int64(ae_res_t *e, aejson_object_t *self,
                                   ae_pool_t *pool, int64_t *out,
                                   const char *fmt, ...)
#ifdef __GNUC__
          __attribute__((format (printf, 5, 6)))
#endif          
          ;

     bool aejson_object_find_double(ae_res_t *e, aejson_object_t *self,
                                   ae_pool_t *pool, double *out,
                                   const char *fmt, ...)
#ifdef __GNUC__
          __attribute__((format (printf, 5, 6)))
#endif          
          ;


     /** 
      * Finds an integer array.  An error is returned if not found or
      * if any of the array members are not integers.  An array is
      * allocated of sufficient size from @p pool.  To cleanup the
      * memory free the pool.
      *
      * @param pool where to allocate from for both the query parsing
      * as well as the output array.
      * 
      * @param out_len the number of integers in the output array
      * 
      * @param out the output array
      * 
      * @param fmt printf style format which specifies the query path
      */
     bool aejson_object_find_array_int64(ae_res_t *e, aejson_object_t *self,
                                         ae_pool_t *pool,
                                         size_t *out_len,
                                         int64_t **out,
                                         const char *fmt, ...)
#ifdef __GNUC__
          __attribute__((format (printf, 6, 7)))
#endif          
          ;

     /** 
      * Finds a double array.  An error is returned if not found or if
      * any of the array members are not doubles.  An array is
      * allocated of sufficient size from @p pool.  To cleanup the
      * memory free the pool.
      *
      * @param pool where to allocate from for both the query parsing
      * as well as the output array.
      * 
      * @param out_len the number of integers in the output array
      * 
      * @param out the output array
      * 
      * @param fmt printf style format which specifies the query path
      */
     bool aejson_object_find_array_double(ae_res_t *e, aejson_object_t *self,
                                          ae_pool_t *pool,
                                          size_t *out_len,
                                          double **out,
                                          const char *fmt, ...)
#ifdef __GNUC__
          __attribute__((format (printf, 6, 7)))
#endif          
          ;


#ifdef __cplusplus
}
#endif


#endif
