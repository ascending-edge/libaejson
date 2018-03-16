/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 *
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved 
 */
#ifndef _AEJSON_VALUE_H
#define _AEJSON_VALUE_H

#include <stdio.h>

#include <ae/ae.h>

/**
 * The possible types stored in a value object.
 */
typedef enum aejson_value_type
{
     AEJSON_VALUE_TYPE_STRING = 0,
     AEJSON_VALUE_TYPE_DOUBLE,
     AEJSON_VALUE_TYPE_INTEGER,
     AEJSON_VALUE_TYPE_OBJECT,
     AEJSON_VALUE_TYPE_ARRAY,
     AEJSON_VALUE_TYPE_BOOLEAN,
     AEJSON_VALUE_TYPE_NULL,
     AEJSON_VALUE_TYPE_ENDMARKER,
} aejson_value_type_t;


/**
 * A class that holds the value of a json 'node'.
 */
typedef struct aejson_value
{
     struct aejson_value *next; /**< linked list... */
     aejson_value_type_t type;  /**< What is stored here */
     union {
          bool boolean;          
          char *str;
          double dbl;
          int64_t integer;
          void *object;
          struct aejson_value *array;
     };
} aejson_value_t;

#ifdef __cplusplus
extern "C" {
#endif

     /** 
      * Prepares a value object
      *
      * @param t the type of this value
      */
     bool aejson_value_init(ae_res_t *e, aejson_value_t *self,
                            aejson_value_type_t t);

     /** 
      * Provides a textual dump to the file descriptor of your choice.
      *
      * @param depth provides padding in the output and increases when
      * recursion happens
      *
      * @param out where to write to
      */
     void aejson_value_dump(const aejson_value_t *self,
                            int depth, FILE *out);
#ifdef __cplusplus
}
#endif


#endif
