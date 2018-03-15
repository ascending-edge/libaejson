/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/object.h>

#include <inttypes.h>

#include <alloca.h>
#include <aejson/query.h>

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
     memset(pad, '\t', depth);
     pad[depth] = '\0';
     
     fprintf(out, "%s{\n", pad);
     for(size_t i=0; i<self->members.len; ++i)
     {
          const aejson_pair_t *p = ae_ptrarray_at(&self->members, i);
          aejson_pair_dump(p, depth+1, out);
     }
     fprintf(out, "%s}\n", pad);
}


static bool aejson_object_find_internal(ae_res_t *e, aejson_object_t *self,
                                        aejson_node_t *query)
{
     for(size_t i=0; i<self->members.len; ++i)
     {
          aejson_pair_t *p = ae_ptrarray_at(&self->members, i);

          bool res = strcmp(query->name, p->name) == 0;
          /* no match, so try the next pair in the object */
          if(!res)
          {
               continue;
          }
          /* The name has been matched */
          if(query->index != -1)
          {
               if(p->value->type != AEJSON_VALUE_TYPE_ARRAY)
               {
                    ae_res_err(e, "expecting array");
                    return false;
               }
               /* can we find the index? */
               aejson_value_t *v = p->value;
               for(size_t j=0; j<query->index; ++i)
               {
                    if(!v->next)
                    {
                         ae_res_err(e, "index out of bounds");
                         return false;
                    }
                    v = v->next;
               }
          }

          /* We matched the name and the query has been exhausted.
           * This means that we found what we are looking for.  */
          if(!query->next)
          {
               AE_LD("found %s", query->name);
               return true;
          }


          /* We have a match but it's not an object and the query
           * isn't exhausted...so this is a failed search. */
          if(p->value->type != AEJSON_VALUE_TYPE_OBJECT)
          {
               ae_res_err(e, "search failed");
               return false;
          }
          /* Descend into the object, yay for recursion  */
          AE_TRY(aejson_object_find_internal(e, p->value->object, query->next));
     }
     return true;
}


bool aejson_object_find(ae_res_t *e, aejson_object_t *self,
                        ae_pool_t *pool, const char *path)
{
     aejson_query_t query;
     AE_MEM_CLEAR(&query);
     AE_TRY(aejson_query_init(e, &query));

     aejson_node_t *query_result = NULL;
     AE_TRY(aejson_query_parse(e, &query, pool, path, &query_result));

     printf("query parsed:\n");
     for(aejson_node_t *i = query_result; i != NULL; i=i->next)
     {
          printf("%s:%"PRId64"\n", i->name, i->index);
     }

     AE_TRY(aejson_object_find_internal(e, self, query_result));
     return true;
}
