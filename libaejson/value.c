/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
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


/** 
 * Iterates over a list of value objects and dumps them.
 *
 * @param depth used for output padding
 * 
 * @param out where to write to
 */
static void aejson_value_array_dump(aejson_value_t *array,
                                    int depth, FILE *out)
{
     /* fprintf(out, "%zu-[", array->dimension); */
     fprintf(out, "[");
     for(aejson_value_t *i=array; i; i=i->next)
     {
          aejson_value_dump(i, depth, out);
          if(i->next)
          {
               fprintf(out, ",");
          }
     }
     fprintf(out, "]");     
}


void aejson_value_dump(const aejson_value_t *self,
                       int depth, FILE *out)
{
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
          aejson_value_array_dump(self->array, depth, out);
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


const char* aejson_value_type_to_string(aejson_value_type_t t)
{
     const char *MAP[] = {
          "string",
          "double",
          "integer",
          "object",
          "array",
          "boolean",
          "null",
     };

     return ae_misc_enum_to_string(AEJSON_VALUE_TYPE_ENDMARKER,
                                   t,
                                   MAP,
                                   AE_ARRAY_LEN(MAP),
                                   __FILE__,
                                   __LINE__);
}
