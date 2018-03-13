#ifndef _VALUE_H
#define _VALUE_H

#include <stdio.h>

#include <ae/ae.h>


typedef enum value_type
{
     VALUE_TYPE_STRING = 0,
     VALUE_TYPE_DOUBLE,
     VALUE_TYPE_INTEGER,
     VALUE_TYPE_OBJECT,
     VALUE_TYPE_ARRAY,
     VALUE_TYPE_BOOLEAN,
     VALUE_TYPE_NULL,
     VALUE_TYPE_ENDMARKER,
} value_type_t;

typedef struct value
{
     value_type_t type;
     union {
          char *str;
          double dbl;
          int64_t integer;
          void *object;
          ae_ptrarray_t *array;
          bool boolean;
     };
} value_t;

#ifdef __cplusplus
extern "C" {
#endif
     
     bool value_init(ae_res_t *e, value_t *self, value_type_t t);

     void value_dump(const value_t *self, int depth, FILE *out);
#ifdef __cplusplus
}
#endif


#endif
