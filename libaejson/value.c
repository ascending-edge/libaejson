#include <aejson/value.h>

#include <inttypes.h>
#include <alloca.h>

#include <aejson/object.h>

bool aejson_value_init(ae_res_t *e, aejson_value_t *self,
                       aejson_value_type_t t)
{
     self->type = t;
     return true;
}


void aejson_value_dump(const aejson_value_t *self,
                       int depth, FILE *out)
{
     char *pad = alloca(depth+1);
     memset(pad, '\t', depth);
     pad[depth] = '\0';
     
     fprintf(out, "%svalue = ", pad);
     switch(self->type)
     {
     case AEJSON_VALUE_TYPE_STRING:
          fprintf(out, "%s", self->str);
          break;
     case AEJSON_VALUE_TYPE_DOUBLE:
          fprintf(out, "%g", self->dbl);
          break;
     case AEJSON_VALUE_TYPE_INTEGER:
          fprintf(out, "%"PRId64, self->integer);
          break;
     case AEJSON_VALUE_TYPE_OBJECT:
          aejson_object_dump((aejson_object_t*)self->object, depth+1, out);
          break;
     case AEJSON_VALUE_TYPE_ARRAY:
          break;
     case AEJSON_VALUE_TYPE_BOOLEAN:
          fprintf(out, "%s", self->boolean ? "true" : "false");
          break;
     case AEJSON_VALUE_TYPE_NULL:
          fprintf(out, "null");
          break;
     default:
          break;
     }
     
}

