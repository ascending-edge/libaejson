/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <ae/ae.h>
#include <aejson/aejson.h>
#include <syslog.h>

#include <inttypes.h>

/** 
 * This is the real main().
 */
static bool internal_main(ae_res_t *e, int argc, char **argv)
{
     ae_pool_t pool;
     AE_MEM_CLEAR(&pool);
     AE_TRY(ae_pool_init(e, &pool, 1024*1024 * 5));

     aejson_parser_t parser;
     AE_MEM_CLEAR(&parser);
     AE_TRY(aejson_parser_init(e, &parser));
     
     aejson_object_t *result = NULL;
     bool res = aejson_parser_parse_file(e, &parser, &pool, stdin, &result);
     fclose(stdin);

     if(res)
     {
          aejson_object_dump(result, 0, stdout);
          abort();
          /* aejson_value_t *val = NULL; */
          double *myint = NULL;
          size_t myint_len = 0;
          if(!aejson_object_find_array_double(e, result, &pool,
                                             &myint_len, &myint,
                                             "a.b.c.d"))
          {
               res = false;
          }
          else
          {
               AE_LD("found array: %zu", myint_len);
               for(size_t i=0; i<myint_len; ++i)
               {
                    /* AE_LD("fuck[%zu]=%"PRId64, i, myint[i]); */
                    AE_LD("fuck[%zu]=%g", i, myint[i]);
               }
               /* aejson_value_dump(val, 0, stdout); */
          }
     }
     
     
     if(!ae_pool_uninit(e, &pool))
     {
          res = false;
     }
     return res;
}


/**
 * Entry point...
 *
 * This sets up logging and reports errors.
 */
int main(int argc, char *argv[])
{
     ae_res_t e;
     ae_res_init(&e);

     /* Open syslog logging to stderr to start.  If debugging is NOT
      * enabled this will be set back to defaults. */
     openlog(NULL, LOG_PERROR, LOG_USER);
     /* Default to a few levels, turn on all if debug is enabled. */
     g_ae_log.mask = 0xff;
          /* AE_LOG_LEVEL_INFO */
          /* | AE_LOG_LEVEL_WARN */
          /* | AE_LOG_LEVEL_ERR; */

     if(!ae_global_init(&e, NULL, NULL))
     {
          goto error_exit;
     }

	if(!internal_main(&e, argc, argv))
     {
          goto error_exit;
     }

     /* Cleanup. */
     ae_global_uninit(&e);
     AE_LOG_RES(&e);

     return 0;

error_exit:
     AE_LOG_RES(&e);
     /* Make sure error messages make it to stderr */
     if(e.msg[AE_RES_N_ERR].count)
     {
          fprintf(stderr, "%s\n", e.msg[AE_RES_N_ERR].msg);
     }
     return 1;
}

