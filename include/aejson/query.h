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

typedef struct aejson_query
{
     ae_res_t *e;
     ae_pool_t *pool;
     aejson_strlit_t strlit;

     aejson_node_t *result;
} aejson_query_t;

#ifdef __cplusplus
extern "C" {
#endif

     bool aejson_query_init(ae_res_t *e, aejson_query_t *self);
     bool aejson_query_parse(ae_res_t *e, aejson_query_t *self,
                             ae_pool_t *pool, const char *in,
                             aejson_node_t **out);
     
     /* bool aejson_query_parse(ae_res_t *e, aejson_query_t *self, */
     /*                         ae_pool_t *pool, */
     /*                         const char *in_file); */

     void aejson_query_error_set(aejson_query_t *self,
                                 const aejson_loc_t *loc,
                                 const char *fmt, ...);

     bool aejson_query_strlit_start(aejson_query_t *self,
                                    const aejson_loc_t *loc);
     bool aejson_query_strlit_add(aejson_query_t *self,
                                  const aejson_loc_t *loc,
                                  char c);
     bool aejson_query_strlit_end(aejson_query_t *self,
                                  const aejson_loc_t *loc,
                                  char **out);


#ifdef __cplusplus
}
#endif


#endif
