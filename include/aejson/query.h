/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 *
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 * 
 * a.b.c.d[x]
 */
#ifndef _AEJSON_QUERY_H
#define _AEJSON_QUERY_H

#include <ae/ae.h>

typedef struct aejson_query
{
     ae_res_t *e;
     ae_pool_t *pool;
} aejson_query_t;

#ifdef __cplusplus
extern "C" {
#endif


     bool aejson_query_init(ae_res_t *e, aejson_query_t *self);
     bool aejson_query_parse(ae_res_t *e, aejson_query_t *self,
                             ae_pool_t *pool);


#ifdef __cplusplus
}
#endif


#endif
