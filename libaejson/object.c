/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/object.h>

#include <alloca.h>


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
     
     fprintf(out, "%sobject {\n", pad);
     for(size_t i=0; i<self->members.len; ++i)
     {
          const aejson_pair_t *p = ae_ptrarray_at(&self->members, i);
          aejson_pair_dump(p, depth+1, out);
     }
     fprintf(out, "%s}\n", pad);
}
