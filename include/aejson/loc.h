/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_LOC_H
#define _AEJSON_LOC_H

#include <ae/ae.h>

/**
 * This is used for keeping track of locations in the scanners and
 * parsers.
 */
typedef struct aejson_loc
{
     int first_line;
     int first_column;
     int last_line;
     int last_column;
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
