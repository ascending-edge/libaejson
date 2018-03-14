/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#include <aejson/pair.h>
#include <alloca.h>

void aejson_pair_dump(const aejson_pair_t *self,
                      int depth, FILE *out)
{
     char *pad = alloca(depth+1);
     memset(pad, '\t', depth);
     pad[depth] = '\0';
     
     fprintf(out, "%spair { %s:\n", pad, self->name);
     aejson_value_dump(self->value, depth +1, out);
     fprintf(out, "\n\%s}\n", pad);
}
