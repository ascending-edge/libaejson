#ifndef _AEJSON_STRLIT_H
#define _AEJSON_STRLIT_H

#include <ae/ae.h>
#include <aejson/loc.h>

typedef struct aejson_strlit
{
     ae_pool_t *pool;
     size_t max;
     size_t len;
     char *str;
} aejson_strlit_t;

#ifdef __cplusplus
extern "C" {
#endif

     bool aejson_strlit_init(ae_res_t *e, aejson_strlit_t *self);
     bool aejson_strlit_cfg(ae_res_t *e, aejson_strlit_t *self,
                             ae_pool_t *pool);
     bool aejson_strlit_start(ae_res_t *e, aejson_strlit_t *self,
                              const aejson_loc_t *loc);
     bool aejson_strlit_add(ae_res_t *e, aejson_strlit_t *self,
                            const aejson_loc_t *loc, char c);
     bool aejson_strlit_end(ae_res_t *e, aejson_strlit_t *self,
                            const aejson_loc_t *loc,
                            char **out);

#ifdef __cplusplus
}
#endif


#endif
