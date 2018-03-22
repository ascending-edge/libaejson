/**
 * @author Greg Rowe <greg.rowe@ascending-edge.com>
 * 
 * Copyright (C) 2018 Ascending Edge, LLC - All Rights Reserved
 */
#ifndef _AEJSON_NODE_H
#define _AEJSON_NODE_H

/**
 * This class is used when parsing the query strings.
 */
typedef struct aejson_node
{
     struct aejson_node *next;  /**< linked list of nodes */
     char *name;                /**< the name of the node */
     int64_t index;             /**< array index, -1 if not
                                 * specified */
} aejson_node_t;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif


#endif
