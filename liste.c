#include <stdlib.h>
#include <assert.h>
#include "liste.h"

struct GenericList_struct {
    GenericNode* firstNode;
};
struct GenericNode_strcut {
    void* contenu;
    GenericNode* nextNode;
};

GenericList* GenericListCreate(){
    GenericList* liste = (GenericList*)calloc(1, sizeof(GenericList));
    assert(liste);
    return liste;
}
void GenericListFree(GenericList* liste){
    GenericNode* node = liste->firstNode, *nextNode;

    if(!liste)
        return;

    while(node->nextNode) {
        nextNode = node->nextNode;
        free(node);
        node = nextNode;
    }
    free(liste);
    return;
}

void GenericNodeAppend(GenericList* liste, void* contenu){
    GenericNode* node;

    if(!liste)
        return;

    node = (GenericNode*)calloc(1, sizeof(GenericNode));
    assert(node);
    
    node->contenu = contenu;
    node->nextNode = NULL;

    if(liste->firstNode == NULL) {
        liste->firstNode = node;
    }
    else {
        GenericNode *tempNode = liste->firstNode;
        while(tempNode->nextNode != NULL){
            tempNode = tempNode->nextNode;
        }
        tempNode->nextNode = node;
    }

}

void *GenericNodeGet(GenericList *list, int index) {
    GenericNode *node = list->firstNode;
    int  actualNode = 0;
    while(node != NULL){
        if(actualNode == index){
            return node->contenu;
        }
        
        node = node->nextNode;
        actualNode++;
    }
    return NULL;
}


void GenericNodeFree(GenericList *list, int index) {
    GenericNode *temporaryNode;

    if(!list)
        return;
    
    temporaryNode = list->firstNode;
    if(index == 0) {
        GenericNode *temporaryNode = list->firstNode;
        list->firstNode = temporaryNode->nextNode;
        free(temporaryNode);
        return;
    } else {
        int actualNodeIndex = 1;
        while(temporaryNode->nextNode) {
            if(actualNodeIndex == index) {
                GenericNode *node = temporaryNode->nextNode;
                temporaryNode->nextNode = temporaryNode->nextNode->nextNode;
                free(node);
                return;
            }
            temporaryNode = temporaryNode->nextNode;
            actualNodeIndex++;
        }
    }
}
