/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_PAIR_H
#define _AEJSON_PAIR_H

#include <stdio.h>

#include <aejson/value.h>

typedef struct aejson_pair
{
     const char *name;
     aejson_value_t *value;
} aejson_pair_t;

#ifdef __cplusplus
extern "C" {
#endif

     void aejson_pair_dump(const aejson_pair_t *self,
                           int depth, FILE *out);

#ifdef __cplusplus
}
#endif


#endif
