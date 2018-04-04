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
 *
 * This is the same as the default location tracker
 */
typedef struct aejson_loc
{
     int first_line;
     int first_column;
     int last_line;
     int last_column;
} aejson_loc_t;

/**
 * This macro is used for tracking locations in the query scanner and
 * the json scanner.
 */
#define LOC_USER_ACTION                         \
     loc->first_line = loc->last_line;          \
     loc->first_column = loc->last_column;      \
     for(int i = 0; yytext[i] != '\0'; i++)     \
     {                                          \
          if(yytext[i] == '\n')                 \
          {                                     \
               loc->last_line++;                \
               loc->last_column = 0;            \
          }                                     \
          else                                  \
          {                                     \
               loc->last_column++;              \
          }                                     \
     }



#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif


#endif
