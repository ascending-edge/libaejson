#include <aejson/query.h>

#include "query-parse.h"

int qlex_init(void**);
int qlex_destroy(void*);


bool aejson_query_init(ae_res_t *e, aejson_query_t *self)
{
     return true;
}


/* void aejson_query_error_set(aejson_parser_t *self, */
/*                              const aejson_parser_loc_t *loc, */
/*                              const char *fmt, ...) */
/* { */
/*      char msg[2048]; */
/*      AE_STR_FROM_ARGS(msg, sizeof(msg), fmt); */
/*      ae_res_err(self->e, "%s", msg); */
/* } */


bool aejson_query_parse(ae_res_t *e, aejson_query_t *self,
                        ae_pool_t *pool)
{
     self->pool = pool;
     self->e = e;
     
     void* scanner;
     qlex_init(&scanner);

     qparse(scanner, self);
     qlex_destroy(scanner);     
     /* *out = self->result; */
     /* return self->result != NULL; */
     return true;
}


