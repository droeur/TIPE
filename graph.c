#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "graph.h"

/****
 * Structure de graphes:
 * liste chainée de noeds avec haque noeud est une liste chainée des voisins pondérés
 * 
 * [graph]
 *    |
 * [noeud] -> [arete] -> [arete]
 *    |
 * [noeud] -> [arete]
 *    |
 * [noeud] -> [arete] -> [arete] -> [arete]
*/

struct graph_struct {
    Node* firstNode;
};

struct node_struct {
    int index;
    Edge* edge;
    Node* nextNode;
};

struct edge_struct {
    Node* dest;
    Edge* nextEdge;
};

Graph* GraphCreate() {
    Graph* g;
    g = (Graph*)malloc(sizeof(Graph));
    g->firstNode = NULL;
    assert(g);
    return g;
}

void GraphFill(Graph *g){
    Node *node, *newNode, *destNode;
    Edge *edge, *nextEdge;
    int i = 0, j = 0;

    while(i < NUMBER_OF_NODES){
        //Création du noeds suivant
        newNode = (Node*)calloc(1, sizeof(Node));
        assert(newNode);
        printf("node %d : %d \n", i, node);
        newNode->index = i;

        if(!g->firstNode){
            g->firstNode = newNode;
            node = g->firstNode;
        } else {
            node->nextNode = newNode;
            node = newNode;
        }

        i++;
    }
    node = g->firstNode;
    while(node){
        destNode = g->firstNode;
        node->edge = (Edge*)calloc(1, sizeof(Edge));
        edge = node->edge;
        while(destNode) {
            if(node != destNode){
                nextEdge = (Edge*)calloc(1, sizeof(Edge));
                edge->nextEdge = nextEdge;
                edge->dest = destNode;
                
                edge = nextEdge;
            }
            destNode = destNode->nextNode;
        }
        node = node->nextNode;
    }
}

void GraphPrint(Graph *g) {
    Node *node = g->firstNode;
    Edge *edge;
    int index, dest;

    printf(" - first node : %d - \n", node);

    while(node){
        printf("(%d @ %d", node->index, node);
        edge = node->edge;
        while(edge && edge->dest){
            printf(" -> %d ", edge->dest->index);
            edge = edge->nextEdge;
        }
        printf(")\n");
        node = node->nextNode;
    }
}

void GraphFree(Graph* g){
    Node *node = g->firstNode;
    Node *nextNode;
    Edge *edge, *nextEdge;
    while(node){
        nextNode = node->nextNode;
        edge = node->edge;
        while(edge){
            nextEdge = edge->nextEdge;
            free(edge);
            edge = nextEdge;
        }
        free(node);
        node = nextNode;
    }
    free(g);
}