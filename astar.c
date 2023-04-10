#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "astar.h"
#include "proceduralGeneration.h"

typedef struct NodeTemp NodeTemp;

struct NodeTemp {
    int g;
    int h;
    int f;

    int x;
    int y;

    int numero;

    int parent;
    NodeTemp* suivant;
};

typedef struct{
    NodeTemp *firstNode;
}NodeArray;

NodeArray *CreateNodeArray(){
    NodeArray *array = NULL;
    array = (NodeArray*)calloc(1, sizeof(NodeArray));
    array->firstNode = NULL;
    return array;
}

void DestroyNodeArray(NodeArray *array){
    NodeTemp *actual_node = array->firstNode;
    NodeTemp *precedent_node = NULL;
    while(actual_node != NULL){
        precedent_node = actual_node;
        actual_node = actual_node->suivant;
        free(precedent_node);
    }
    free(array);
}

NodeTemp* CreateNode(NodeArray *array){
    NodeTemp *node = NULL;
    if(array->firstNode == NULL){
        array->firstNode = (NodeTemp*)calloc(1, sizeof(NodeTemp));
        node = array->firstNode;
    } else {
        node = array->firstNode;
        while(node->suivant != NULL){
            node = node->suivant;
        }
        node->suivant = (NodeTemp*)calloc(1, sizeof(NodeTemp));
        node = node->suivant;
    }
    node->suivant = NULL;
    return node;
}

void DestroyNode(NodeArray *array, int place){
    NodeTemp *node = NULL;
    NodeTemp *precedent_node = NULL;
    int index = 0;
    node = array->firstNode;

    if(place == 0){
        array->firstNode = node->suivant;

        free(node);
    } else {
        while((node->suivant != NULL) && (index < place)){
            precedent_node = node;
            node = node->suivant;
            index++;
        }
        precedent_node->suivant = node->suivant;
        free(node);
    }
}

NodeTemp* NodeNumber(NodeArray *array, int number){
    NodeTemp *node = NULL;
    int index = 0;
    node = array->firstNode;
    while((index < number) && (node->suivant != NULL)){
        node = node->suivant;
        index++;
    }
    return node;
}

int ArraySize(NodeArray *array){
    NodeTemp *node = NULL;
    int index = 0;
    node = array->firstNode;
    while((node != NULL) && (index < 100)){
        node = node->suivant;
        index++;
    }
    return index;
}

NodeTemp* CopyNode(NodeTemp *destination, NodeTemp *source){
    NodeTemp *node;
    node = destination->suivant;
    memcpy(destination, source, sizeof(NodeTemp));
    destination->suivant = node;
    return destination;
}

void ReverseArray(int array[], int size){
    int *aux = NULL, i = 0, j = size - 1;
    aux = (int*)calloc(1, size * sizeof(int));
    while(i < size){
        aux[j] = array[i];
        j--;
        i++;
    }
    i = 0;
    j = 0;
    while(aux[j] == 0){
        j++;
    }
    while(j < size){
        array[i] = aux[j];
        i++;
        j++;
    }
}

//TODO pathfinding
void pathfinding_astar(Graph* graph, int path[20], int debut, int fin){
    NodeArray *listeOuverte = NULL;
    NodeArray *listeFermee = NULL;
    NodeArray *listeEnfants = NULL;
    NodeTemp *depart = (NodeTemp*)malloc(sizeof(NodeTemp));
    NodeTemp *arrivee = (NodeTemp*)malloc(sizeof(NodeTemp));
    NodeTemp *current_node = NULL;
    NodeTemp *temporary_node = NULL;
    NodeTemp *children = NULL;

    int index = 0, current_index = 0, children_index = 0;
    int enfant[4], parent;

    depart->x = debut / MAPSIZE;
    depart->y = debut % MAPSIZE ;
    depart->g = 0;
    depart->h = 0;
    depart->f = 0;
    depart->numero = debut;
    depart->suivant = NULL;
    depart->parent = 0;

    arrivee->x = fin / MAPSIZE;
    arrivee->y = fin % MAPSIZE;
    arrivee->g = 0;
    arrivee->h = 0;
    arrivee->f = 0;
    arrivee->numero = fin;


    memset(path, 0, sizeof(int) * 50);

    listeOuverte = CreateNodeArray();
    listeFermee = CreateNodeArray();
    listeEnfants = CreateNodeArray();
    CopyNode(CreateNode(listeOuverte), depart);

    while(ArraySize(listeOuverte) > 0){
        //printf("\n%d ", ArraySize(listeOuverte));
        current_node = listeOuverte->firstNode;
        current_index = 0;
        index = 0;

        //recupere l'element le plus petit
        while(index < ArraySize(listeOuverte)){
            temporary_node = NodeNumber(listeOuverte, index);
            if(current_node->f > temporary_node->f){
                current_node = temporary_node;
                current_index = index;
            }
            index++;
        }
        // printf("node:%d, x:%d, y:%d, f:%d, g:%d, h:%d, parent:%d", current_node->numero, current_node->x, current_node->y, current_node->f, current_node->g, current_node->h, current_node->parent);

        current_node = CopyNode(CreateNode(listeFermee), current_node);
        DestroyNode(listeOuverte, current_index);

        DestroyNodeArray(listeEnfants);
        listeEnfants = CreateNodeArray();

        //ecrire resultat
        if(current_node->numero == fin){
                printf("\n");
                current_index = 1;
                path[0] = fin;
                while(current_node->numero != debut){
                    path[current_index] = current_node->parent;
                    parent = current_node->parent;
                   // printf("%d ", current_node->parent);
                    index = 0;
                    while((current_node->numero != parent) && (index < 100)){
                        current_node = NodeNumber(listeFermee, index);
                        index++;
                    }
                    current_index++;
                }
                while(current_index < 50){
                    path[current_index] = 0;
                    //printf("%d ", 0);
                    current_index++;
                }
                printf("\n");
                ReverseArray(path, 20);
                return;
        }
    }
}