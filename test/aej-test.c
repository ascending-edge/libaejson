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
          aejson_value_t *val = NULL;
          //const char *query = "a[0][0][2][3].b";
          /* const char *query = "a.b.cd"; */

          //FAILURE
          /* const char *query = "   ."; */
          const char *query = ".";
          
          /* Location error! */
          /* const char *query = "[0]"; */

          /* const char *query = "a.a[1]"; */
          AE_LD("query=(%s)", query);
          if(!aejson_object_find(e, result, &pool, &val, query))
          {
               res = false;
          }
          else
          {
               printf("--found--\n");
               aejson_value_dump(val, 0, stdout);
               printf("\n---------\n");
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

