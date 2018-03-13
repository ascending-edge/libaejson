#ifndef _PAIR_H
#define _PAIR_H

#include <stdio.h>

#include "value.h"

typedef struct pair
{
     const char *name;
     value_t *value;
} pair_t;

#ifdef __cplusplus
extern "C" {
#endif

     void pair_dump(const pair_t *self, int depth, FILE *out);

#ifdef __cplusplus
}
#endif


#endif
