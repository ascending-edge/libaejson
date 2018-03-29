/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_NODE_H
#define _AEJSON_NODE_H

typedef enum aejson_node_type
{
     AEJSON_NODE_TYPE_ID=0,     
     AEJSON_NODE_TYPE_INDEX,

     AEJSON_NODE_TYPE_ENDMARKER,
} aejson_node_type_t;

/**
 * This class is used when parsing the query strings.
 */
typedef struct aejson_node
{
     struct aejson_node *next;  /**< linked list of nodes */
     aejson_node_type_t type;
     union {
          uint64_t index;
          char *id;
     };
} aejson_node_t;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif


#endif
