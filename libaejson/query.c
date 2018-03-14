/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/query.h>

#include "query-parse.h"

/* I hate doing this but I haven't come up with a more elegant way.  I
 * ought to look at how flex is implemented and make something less
 * complicated. */
int qlex_init(void**);
int qlex_destroy(void*);
struct yy_buffer_state;
struct yy_buffer_state* q_scan_string(const char *, void *);
void q_delete_buffer(struct yy_buffer_state*, void*);


bool aejson_query_init(ae_res_t *e, aejson_query_t *self)
{
     AE_TRY(aejson_strlit_init(e, &self->strlit));
     return true;
}


void aejson_query_error_set(aejson_query_t *self,
                            const aejson_loc_t *loc,
                            const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     ae_res_err(self->e, "%s", msg);
}


bool aejson_query_parse(ae_res_t *e, aejson_query_t *self,
                        ae_pool_t *pool, const char *in)
{
     self->pool = pool;
     self->e = e;

     AE_TRY(aejson_strlit_cfg(e, &self->strlit, pool));
     
     void* scanner;
     qlex_init(&scanner);

     struct yy_buffer_state *bs = q_scan_string(in, scanner);
     qparse(scanner, self);
     q_delete_buffer(bs, scanner);
     qlex_destroy(scanner);     
     /* *out = self->result; */
     /* return self->result != NULL; */
     return true;
}


bool aejson_query_strlit_start(aejson_query_t *self,
                               const aejson_loc_t *loc)
{
     AE_TRY(aejson_strlit_start(self->e, &self->strlit, loc));
     return true;
}


bool aejson_query_strlit_add(aejson_query_t *self,
                             const aejson_loc_t *loc,
                             char c)
{
     AE_TRY(aejson_strlit_add(self->e, &self->strlit, loc, c));
     return true;
}


bool aejson_query_strlit_end(aejson_query_t *self,
                             const aejson_loc_t *loc,
                             char **out)
{
     AE_TRY(aejson_strlit_end(self->e, &self->strlit, loc, out));
     return true;
}
