#include <ae/ae.h>
#include <aejson/aejson.h>
#include <syslog.h>

static bool internal_main(ae_res_t *e)
{
     parser_t parser;
     AE_MEM_CLEAR(&parser);
     AE_TRY(parser_init(e, &parser));

     ae_pool_t pool;
     AE_MEM_CLEAR(&pool);
     AE_TRY(ae_pool_init(e, &pool, 1024*1024 * 5));

     aejson_object_t *result = NULL;
     bool res = parser_parse(e, &parser, &pool, &result);
     aejson_object_dump(result, 0, stdout);
     
     if(!ae_pool_uninit(e, &pool))
     {
          res = false;
     }
     return res;
}


int main(int argc, char *argv[])
{
     ae_res_t e;
     ae_res_init(&e);

     /* Open syslog logging to stderr to start.  If debugging is NOT
      * enabled this will be set back to defaults. */
     openlog(NULL, LOG_PERROR, LOG_USER);
     /* Default to a few levels, turn on all if debug is enabled. */
     g_ae_log.mask =
          AE_LOG_LEVEL_INFO
          | AE_LOG_LEVEL_WARN
          | AE_LOG_LEVEL_ERR;

     if(!ae_global_init(&e, NULL, NULL))
     {
          goto error_exit;
     }

	if(!internal_main(&e))
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

