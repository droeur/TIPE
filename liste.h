#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED

typedef struct _GenericList GenericList;
typedef struct _GenericNode GenericNode;

GenericList* GenericListCreate();
void GenericListFree(GenericList* liste);
void GenericNodeAppend(GenericList* liste, void* contenu);
void *GenericNodeGet(GenericList *list, int index);
void GenericNodeAdd(GenericList* liste, void* contenu, int index);
void GenericNodeFree(GenericList *list, int index);

#endif