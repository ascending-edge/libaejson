/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 *
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved 
 */
#ifndef _AEJSON_VALUE_H
#define _AEJSON_VALUE_H

#include <stdio.h>

#include <ae/ae.h>


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


typedef struct aejson_value
{
     struct aejson_value *next;
     aejson_value_type_t type;
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
     
     bool aejson_value_init(ae_res_t *e, aejson_value_t *self,
                            aejson_value_type_t t);

     void aejson_value_dump(const aejson_value_t *self,
                            int depth, FILE *out);
#ifdef __cplusplus
}
#endif


#endif
