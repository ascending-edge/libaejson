# Overview

I created this library partly out of boredom but also because I wanted
a few features that didn't exist from existing JSON libraries for C.
More than anything else I wanted c-style comments.  I also wanted basic
arithmetic and heredoc syntax for strings.

The result is a small and simple library for parsing JSON and
extracting values.  It should work pretty well in applications that
wish to use JSON for configuration files.

# Features

- small footprint
- minimal dependencies - Depends on libae
- differentiates between floating point and integer values
- c-style comments are allowed in the JSON
- here-doc syntax is available for strings


# Deviations 

- Single and multiple line c-style comments
- heredoc syntax for strings
- Basic numeric expressions are allowed

## Examples

```javascript
{
    /* multi line comments, 
       oh my!
    */
    // single line too!  Yay!
    "a":[0,1,2,3,4,5],
    // this is parsed as a single integer: 2048
    "b":1024 * 2,
    "heredoc":<<<
a
b
c
>>>
}
```


# Use

The typical usage is to parse JSON text from a string or a file into
an aejson object and then extract values using a single query string.

The library utilizes memory pools.  Internally the parser will
allocate memory as needed from the pool that is provided.  All of the
allocated memory is freed when the pool is freed (in a single
operation).


## Example Parsing

```c
/* prepare a minimum 5MB memory pool */
ae_pool_t pool;

/* this is required because libae requires that all objects be set to
   zero prior to initializing */
AE_MEM_CLEAR(&pool);
AE_TRY(ae_pool_init(e, &pool, 1024*1024 * 5));

/* prepare the parser */
aejson_parser_t parser;
AE_MEM_CLEAR(&parser);
AE_TRY(aejson_parser_init(e, &parser));

/* parse stdin */
aejson_object_t *result = NULL;
bool res = aejson_parser_parse_file(e, &parser, &pool, stdin, &result);
fclose(stdin);

/* the only cleanup required is to free the memory pool*/
AE_TRY(ae_pool_uninit(e, &pool));
```

## Querying

Finding data is achieved by querying using the same sort of syntax
that you would (I hope) expect.  For example "a.b.c" would return the
value 64 from the following json {"a":{"b":"c":64}}

A number of functions are available for locating values.  They include:

- `aejson_object_find` - Finds any value, output is aejson_value_t *
- `aejson_object_find_string` - Finds a string
- `aejson_object_find_int64` - Finds an integer
- `aejson_object_find_double` - Finds a floating point value
- `aejson_object_find_array_int64` - Finds and duplicates an array of
  integers
- `aejson_object_find_array_string` - Finds an array of strings
  without duplication
- `aejson_object_find_array_double` - Finds and duplicates an array of
  floating point values.

### Example - Querying

In the example below the output will be:

```sh
--found--
sup: one
sup: two
sup: three
---------
```

```javascript
{
    "s":["one","two","three"],
}
```

```c
size_t vals_len = 0;
const char *query = "s";
if(!aejson_object_find_array_string(e, result, &pool,
                                    &vals_len, &vals, query))
{
    /* the world has ended */
    abort();
}

printf("--found string array--\n");
for(size_t i=0; i<vals_len; ++i)
{
    printf("sup: %s\n", vals[i]);
}
printf("---------\n");

/* cleanup by calling ae_pool_uninit(e, &pool) */
```
