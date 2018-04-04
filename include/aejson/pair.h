/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_PAIR_H
#define _AEJSON_PAIR_H

#include <stdio.h>

#include <aejson/value.h>


/**
 * This class represents a pair contained in a json object.  Objects
 * are made up of zero or more pairs.
 */
typedef struct aejson_pair
{
     const char *name;          /**< field name */
     aejson_value_t *value;     /**< contents of the field */
} aejson_pair_t;

#ifdef __cplusplus
extern "C" {
#endif

     /** 
      * A debugging tool that provides a textual representation of the
      * value.
      *
      * @param depth used for padding; increases with recursion
      * 
      * @param out where to write to
      */
     void aejson_pair_dump(const aejson_pair_t *self,
                           int depth, FILE *out);

#ifdef __cplusplus
}
#endif


#endif
