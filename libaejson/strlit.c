/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/strlit.h>


bool aejson_strlit_init(ae_res_t *e, aejson_strlit_t *self)
{
     return true;
}


bool aejson_strlit_cfg(ae_res_t *e, aejson_strlit_t *self,
                       ae_pool_t *pool)
{
     self->pool = pool;
     return true;
}


bool aejson_strlit_start(ae_res_t *e, aejson_strlit_t *self,
                         const aejson_loc_t *loc)
{
     if(self->str)
     {
          ae_res_err(e, "must not start a literal while processing one!");
          return false;
     }
     self->max = 512;
     AE_TRY(ae_pool_calloc(e, self->pool, &self->str, self->max));
     return true;
}


bool aejson_strlit_add(ae_res_t *e, aejson_strlit_t *self,
                       const aejson_loc_t *loc, char c)
{
     /* Do we need to resize? */
     if(self->len == self->max - 1)
     {
          size_t new_max = self->max + 512;
          char *new_str = NULL;
          AE_TRY(ae_pool_realloc(e, self->pool,
                                 self->str, self->max,
                                 &new_str, new_max));
          self->str = new_str;
          self->max = new_max;
          self->str[self->len -1] = '\0';
     }
     self->str[self->len++] = c;
     return true;
}


bool aejson_strlit_end(ae_res_t *e, aejson_strlit_t *self,
                       const aejson_loc_t *loc,
                       char **out)
{
     if(!self->str)
     {
          ae_res_err(e, "not currently processing a string literal");
          return false;
     }
     *out = self->str;
     self->str = NULL;
     self->len = 0;
     return true;
}
