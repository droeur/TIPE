#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#define NUMBER_OF_NODES 10

typedef struct graph_struct Graph;
typedef struct node_struct Node;
typedef struct edge_struct Edge;

Graph* GraphCreate();
void GraphFill(Graph *g);
void GraphFree(Graph* g);
void GraphPrint(Graph *g);
void* NodeDataGet(Graph* g, int index);

#endif