#include <aejson/pair.h>

#include <alloca.h>

void pair_dump(const pair_t *self, int depth, FILE *out)
{
     char *pad = alloca(depth+1);
     memset(pad, '\t', depth);
     pad[depth] = '\0';
     
     fprintf(out, "%spair { %s:\n", pad, self->name);
     value_dump(self->value, depth +1, out);
     fprintf(out, "\n\%s}\n", pad);
}
