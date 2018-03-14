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

typedef struct aejson_object
{
     ae_ptrarray_t members;
} aejson_object_t;

#ifdef __cplusplus
extern "C" {
#endif

     bool aejson_object_init(ae_res_t *e, aejson_object_t *self,
                             ae_pool_t *pool);

     bool aejson_object_pair_add(ae_res_t *e, aejson_object_t *self,
                                 aejson_pair_t *p);

     void aejson_object_dump(const aejson_object_t *self,
                             int depth, FILE *out);

     bool aejson_object_find(ae_res_t *e, aejson_object_t *self,
                             const char *path);

#ifdef __cplusplus
}
#endif


#endif
