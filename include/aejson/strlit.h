/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_STRLIT_H
#define _AEJSON_STRLIT_H

#include <ae/ae.h>
#include <aejson/loc.h>


/**
 * A class shared by the query scanner and json scanner for building
 * string literals.
 */
typedef struct aejson_strlit
{
     ae_pool_t *pool;           /**< where to allocate memory from */
     size_t max;                /**< current max size to hold */
     size_t len;                /**< current length of str */
     char *str;                 /**< the string */
} aejson_strlit_t;


#ifdef __cplusplus
extern "C" {
#endif

     /** 
      * Prepares an object for use.
      */
     bool aejson_strlit_init(ae_res_t *e, aejson_strlit_t *self);


     /** 
      * Configures the object by setting up an allocate.
      *
      * @param pool where to allocate from
      */
     bool aejson_strlit_cfg(ae_res_t *e, aejson_strlit_t *self,
                             ae_pool_t *pool);


     /** 
      * Call this when the beginning of a string literal is
      * discovered.
      *
      * @param loc where in the input the literal is located
      */
     bool aejson_strlit_start(ae_res_t *e, aejson_strlit_t *self,
                              const aejson_loc_t *loc);


     /** 
      * Adds a character to the string literal
      *
      * @param loc where in the input the character is located
      * 
      * @param c what to add
      */
     bool aejson_strlit_add(ae_res_t *e, aejson_strlit_t *self,
                            const aejson_loc_t *loc, char c);


     /** 
      * Called when the end of the literal is detected.  
      *
      * @param loc where the end of the literal is from the input
      * 
      * @param out the value of the string literal.  This is allocated
      * from the memory pool assigned to this.  Therefore callers can
      * use this as long as the memory pool is not destroyed (or make
      * a copy).
      */
     bool aejson_strlit_end(ae_res_t *e, aejson_strlit_t *self,
                            const aejson_loc_t *loc,
                            char **out);

#ifdef __cplusplus
}
#endif


#endif
