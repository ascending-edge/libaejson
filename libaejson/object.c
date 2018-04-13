/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/object.h>

#include <inttypes.h>
#include <stdarg.h>
#include <alloca.h>
#include <aejson/query.h>

#define MAKE_PATH                               \
     char path[8192];                           \
     va_list args;                              \
     va_start(args, fmt);                       \
     AE_STR_PRINTV(path, fmt, args);            \
     va_end(args)


bool aejson_object_init(ae_res_t *e, aejson_object_t *self,
                        ae_pool_t *pool)
{
     AE_TRY(ae_ptrarray_init(e, &self->members, pool, 16));
     return true;
}


bool aejson_object_pair_add(ae_res_t *e, aejson_object_t *self,
                            aejson_pair_t *p)
{
     AE_TRY(ae_ptrarray_append(e, &self->members, p));
     return true;
}


void aejson_object_dump(const aejson_object_t *self,
                        int depth, FILE *out)
{
     char *pad = alloca(depth+1);
     memset(pad, ' ', depth);
     pad[depth] = '\0';
     
     fprintf(out, "%s{\n", pad);
     for(size_t i=0; i<self->members.len; ++i)
     {
          const aejson_pair_t *p = ae_ptrarray_at(&self->members, i);
          aejson_pair_dump(p, depth+1, out);
     }
     fprintf(out, "%s}\n", pad);
}


static bool aejson_object_value_from_array(ae_res_t *e,
                                           aejson_value_t *array,
                                           size_t ix,
                                           aejson_value_t **out)
{
     if(array->type != AEJSON_VALUE_TYPE_ARRAY)
     {
          ae_res_err(e, "error: expecting array but saw %s",
                     aejson_value_type_to_string(array->type));
          return false;
     }
     /* can we find the index? */
     aejson_value_t *v = array->array;
     for(size_t i=0; i<ix; ++i)
     {
          if(!v->next)
          {
               ae_res_err(e, "index out of bounds %zu, max %zu", ix, i);
               return false;
          }
          v = v->next;
     }
     *out = v;
     return true;
}



static bool aejson_object_find_internal(ae_res_t *e, aejson_object_t *self,
                                        aejson_node_t *query,
                                        aejson_value_t **out)
{
     for(size_t i=0; i<self->members.len; ++i)
     {
          aejson_pair_t *p = ae_ptrarray_at(&self->members, i);

          /* AE_LD("comparing %s to %s", query->id, p->name); */
          bool res = strcmp(query->id, p->name) == 0;
          /* no match, so try the next pair in the object */
          if(!res)
          {
               continue;
          }
          aejson_value_t *value = p->value;
          if(!query->next)
          {
               *out = value;
               return true;
          }

          while(query->next
                && (query->next->type == AEJSON_NODE_TYPE_INDEX))
          {
               AE_TRY(aejson_object_value_from_array(
                           e, value, query->next->index, &value));
               query = query->next;
          }
          
          /* Nothing left in the query */
          if(!query->next)
          {
               *out = value;
               return true;
          }

          
          if(value->type != AEJSON_VALUE_TYPE_OBJECT)
          {
               ae_res_err(e, "expecting object at %s but saw %s",
                          query->id,
                          aejson_value_type_to_string(value->type));
               return false;
          }
          return aejson_object_find_internal(e, value->object, query->next, out);
     }
     
     ae_res_err(e, "no matches found");
     return false;
}


static bool aejson_object_finds(ae_res_t *e, aejson_object_t *self,
                                ae_pool_t *pool, aejson_value_t **out,
                                const char *path)
{
     aejson_query_t query_parser;
     AE_MEM_CLEAR(&query_parser);
     AE_TRY(aejson_query_init(e, &query_parser));

     aejson_node_t *query = NULL;
     AE_TRY(aejson_query_parse(e, &query_parser, pool, path, &query));

     /* printf("query parsed:\n"); */
     /* for(aejson_node_t *i = query; i != NULL; i=i->next) */
     /* { */
     /*      switch(i->type) */
     /*      { */
     /*      case AEJSON_NODE_TYPE_INDEX: */
     /*           printf("[%"PRId64"]\n", i->index); */
     /*           break; */
     /*      case AEJSON_NODE_TYPE_ID: */
     /*           printf("%s:\n", i->id); */
     /*           break; */
     /*      default: */
     /*           break; */
     /*      } */
     /* } */
     AE_TRY(aejson_object_find_internal(e, self, query, out));
     return true;
}


bool aejson_object_find(ae_res_t *e, aejson_object_t *self,
                        ae_pool_t *pool, aejson_value_t **out,
                        const char *fmt, ...)
{
     MAKE_PATH;
     /* AE_LD("query: (%s)", path); */
     AE_TRY(aejson_object_finds(e, self, pool, out, path));
     return true;
}


static bool aejson_object_find_type(ae_res_t *e, aejson_object_t *self,
                                    ae_pool_t *pool, aejson_value_t **val,
                                    aejson_value_type_t expected_type,
                                    const char *path)
{
     AE_TRY(aejson_object_finds(e, self, pool, val, path));
     if((*val)->type != expected_type)
     {
          ae_res_err(e, "%s: type mismatch, expecting %s but found %s",
                     path,
                     aejson_value_type_to_string(expected_type),
                     aejson_value_type_to_string((*val)->type));
          return false;
     }
     return true;
}


bool aejson_object_find_int64(ae_res_t *e, aejson_object_t *self,
                              ae_pool_t *pool, int64_t *out,
                              const char *fmt, ...)
{
     MAKE_PATH;
     /* AE_LD("query: (%s)", path); */
     aejson_value_t *val = NULL;
     AE_TRY(aejson_object_find_type(e, self, pool, &val,
                                    AEJSON_VALUE_TYPE_INTEGER, path));
     *out = val->integer;
     return true;
}


bool aejson_object_find_string(ae_res_t *e, aejson_object_t *self,
                               ae_pool_t *pool, char **out,
                               const char *fmt, ...)
{
     MAKE_PATH;
     aejson_value_t *val = NULL;
     AE_TRY(aejson_object_find_type(e, self, pool, &val,
                                    AEJSON_VALUE_TYPE_STRING, path));
     *out = val->str;
     return true;
}


bool aejson_object_find_double(ae_res_t *e, aejson_object_t *self,
                              ae_pool_t *pool, double *out,
                              const char *fmt, ...)
{
     MAKE_PATH;
     /* AE_LD("query: (%s)", path); */
     aejson_value_t *val = NULL;
     AE_TRY(aejson_object_find_type(e, self, pool, &val,
                                    AEJSON_VALUE_TYPE_DOUBLE, path));
     *out = val->dbl;
     return true;
}



bool aejson_object_find_array_int64(ae_res_t *e, aejson_object_t *self,
                                    ae_pool_t *pool,
                                    size_t *out_len,
                                    int64_t **out,
                                    const char *fmt, ...)
{
     MAKE_PATH;
     /* AE_LD("query: (%s)", path); */
     aejson_value_t *val = NULL;
     AE_TRY(aejson_object_find_type(e, self, pool, &val,
                                    AEJSON_VALUE_TYPE_ARRAY, path));
     /* AE_LD("%zu", val->array->dimension); */
     size_t len_bytes = val->array->dimension * sizeof(**out);
     *out_len = val->array->dimension;
     AE_TRY(ae_pool_alloc(e, pool, out, len_bytes));

     aejson_value_t *iter = val->array;
     int64_t *out_iter = *out;
     for(size_t i=0; i<val->array->dimension; ++i)
     {
          if(iter->type != AEJSON_VALUE_TYPE_INTEGER)
          {
               ae_res_err(e, "type mismatch, "
                          "expecting integer but saw %s at index %zu",
                          aejson_value_type_to_string(iter->type), i);
               return false;
          }
          *out_iter = iter->integer;
          ++out_iter;
          iter = iter->next;
     }

     return true;
}


bool aejson_object_find_array_string(ae_res_t *e, aejson_object_t *self,
                                    ae_pool_t *pool,
                                    size_t *out_len,
                                    char ***out,
                                    const char *fmt, ...)
{
     MAKE_PATH;
     /* AE_LD("query: (%s)", path); */
     aejson_value_t *val = NULL;
     AE_TRY(aejson_object_find_type(e, self, pool, &val,
                                    AEJSON_VALUE_TYPE_ARRAY, path));
     /* AE_LD("%zu", val->array->dimension); */
     size_t len_bytes = val->array->dimension * sizeof(**out);
     *out_len = val->array->dimension;
     AE_TRY(ae_pool_alloc(e, pool, out, len_bytes));

     aejson_value_t *iter = val->array;
     char **out_iter = *out;
     for(size_t i=0; i<val->array->dimension; ++i)
     {
          if(iter->type != AEJSON_VALUE_TYPE_STRING)
          {
               ae_res_err(e, "type mismatch, "
                          "expecting string but saw %s at index %zu",
                          aejson_value_type_to_string(iter->type), i);
               return false;
          }
          *out_iter = iter->str;
          ++out_iter;
          iter = iter->next;
     }

     return true;
}


bool aejson_object_find_array_double(ae_res_t *e, aejson_object_t *self,
                                     ae_pool_t *pool,
                                     size_t *out_len,
                                     double **out,
                                     const char *fmt, ...)
{
     MAKE_PATH;
     /* AE_LD("query: (%s)", path); */
     aejson_value_t *val = NULL;
     AE_TRY(aejson_object_find_type(e, self, pool, &val,
                                    AEJSON_VALUE_TYPE_ARRAY, path));
     /* AE_LD("%zu", val->array->dimension); */
     size_t len_bytes = val->array->dimension * sizeof(**out);
     *out_len = val->array->dimension;
     AE_TRY(ae_pool_alloc(e, pool, out, len_bytes));

     aejson_value_t *iter = val->array;
     double *out_iter = *out;
     for(size_t i=0; i<val->array->dimension; ++i)
     {
          if(iter->type != AEJSON_VALUE_TYPE_DOUBLE)
          {
               ae_res_err(e, "type mismatch, "
                          "expecting double but saw %s at index %zu",
                          aejson_value_type_to_string(iter->type), i);
               return false;
          }
          *out_iter = iter->dbl;
          ++out_iter;
          iter = iter->next;
     }

     return true;
}
