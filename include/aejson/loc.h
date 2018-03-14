#ifndef _AEJSON_LOC_H
#define _AEJSON_LOC_H

#include <ae/ae.h>

typedef struct aejson_loc
{
     size_t first_line;
     size_t first_column;
     size_t last_line;
     size_t last_column;
} aejson_loc_t;

#define LOC_USER_ACTION \
     loc->first_line = loc->last_line = yylineno; \
     loc->first_column = yycolumn;                \
     loc->last_column = yycolumn + yyleng - 1;    \
     yycolumn += yyleng;


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif


#endif
