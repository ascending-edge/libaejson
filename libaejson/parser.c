/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/parser.h>

#include "json-parse.h"

/* I hate doing this but I haven't come up with a more elegant way.  I
 * ought to look at how flex is implemented and make something less
 * complicated. */
int jlex_init(void**);
int jlex_destroy(void*);
struct yy_buffer_state;
struct yy_buffer_state* j_scan_string(const char *, void *);
void j_delete_buffer(struct yy_buffer_state*, void*);
void jset_in(FILE * , void* );



bool aejson_parser_init(ae_res_t *e, aejson_parser_t *self)
{
     AE_TRY(aejson_strlit_init(e, &self->strlit));
     return true;
}


void aejson_parser_error_set(aejson_parser_t *self,
                             const aejson_loc_t *loc,
                             const char *fmt, ...)
{
     char msg[2048];
     AE_STR_FROM_ARGS(msg, sizeof(msg), fmt);
     ae_res_err(self->e, "%s", msg);
}


/** 
 * Prepare the scanner for use independent of input type (string,
 * file).  The sets self->pool, and self->e.
 *
 * @param pool memory pool to allocate from for scanning and parsing
 * 
 * @param scanner output - initialized scanner that must be destroyed
 */
static bool aejson_scan_init(ae_res_t *e, aejson_parser_t *self,
                             ae_pool_t *pool, void *scanner)
{
     self->pool = pool;
     self->e = e;
     self->result = NULL;
     AE_TRY(aejson_strlit_cfg(e, &self->strlit, pool));
     jlex_init(scanner);
     return true;
}


bool aejson_parser_parse(ae_res_t *e, aejson_parser_t *self,
                         ae_pool_t *pool,
                         const char *json,
                         aejson_object_t **out)
{
     void *scanner = NULL;
     AE_TRY(aejson_scan_init(e, self, pool, &scanner));
     struct yy_buffer_state *bs = j_scan_string(json, scanner);
     jparse(scanner, self);
     j_delete_buffer(bs, scanner);
     jlex_destroy(scanner);     
     *out = self->result;
     return self->result != NULL;
}


bool aejson_parser_parse_file(ae_res_t *e, aejson_parser_t *self,
                              ae_pool_t *pool,
                              FILE *in,
                              aejson_object_t **out)
{
     void *scanner = NULL;
     AE_TRY(aejson_scan_init(e, self, pool, &scanner));
     jset_in(in, scanner);
     jparse(scanner, self);
     jlex_destroy(scanner);     
     *out = self->result;
     return self->result != NULL;
}


bool aejson_parser_comment_add(aejson_parser_t *self,
                               const aejson_loc_t  *loc,
                               const char *comment)
{
     /* Maybe at some point I'll decorate the parse tree with some
      * comments. */
     /* printf("ignoring comment: %s\n", comment); */
     return true;
}


bool aejson_parser_strlit_start(aejson_parser_t *self,
                                const aejson_loc_t *loc)
{
     AE_TRY(aejson_strlit_start(self->e, &self->strlit, loc));
     return true;
}


bool aejson_parser_strlit_add(aejson_parser_t *self,
                              const aejson_loc_t *loc,
                              char c)
{
     AE_TRY(aejson_strlit_add(self->e, &self->strlit, loc, c));
     return true;
}


bool aejson_parser_strlit_end(aejson_parser_t *self,
                               const aejson_loc_t *loc,
                               char **out)
{
     AE_TRY(aejson_strlit_end(self->e, &self->strlit, loc, out));
     return true;
}


bool aejson_parser_value_reset(aejson_parser_t *self)
{
     self->value_stack = NULL;
     return true;
}


bool aejson_parser_value_append(aejson_parser_t *self,
                                aejson_value_t *value)
{
     if(!self->value_stack)
     {
          ae_res_err(self->e, "empty stack");
          return false;
     }
     if(!self->value_stack->value)
     {
          self->value_stack->value = value;
     }
     else
     {
          self->value_stack->value->next = value;
     }
     ++self->value_stack->dimension;
     return true;
}


bool aejson_parser_value_push(aejson_parser_t *self,
                              aejson_value_t *value)
{
     stack_node_t *new_node;
          
     AE_TRY(ae_pool_calloc(self->e, self->pool, &new_node, sizeof(*new_node)));
     new_node->value = value;
     new_node->next = self->value_stack;
     self->value_stack = new_node;
     ++self->value_stack->dimension;
     return true;
}


bool aejson_parser_value_pop(aejson_parser_t *self)
{
     if(!self->value_stack)
     {
          ae_res_err(self->e, "empty stack");
          return false;
     }
     self->value_stack = self->value_stack->next;
     return true;
}
