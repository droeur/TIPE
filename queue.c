#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "liste.h"
/***
 * Tas binaire
 * 
 */


struct _element {
    int priority;
    void *data;
};
struct _priority_queue {
    GenericList *array;
    int cur_size;
};

priority_queue *PriorityQueueCreate(){
    priority_queue* queue = (priority_queue*)(calloc(1, sizeof(priority_queue)));
    assert(queue);
    queue->cur_size = 0;
    queue->array = GenericListCreate();
    return queue;
}

void PriorityQueueFree(priority_queue* queue){
    assert(queue);
    GenericListFree(queue->array);
    free(queue);
}

static void Element_Swap(priority_queue* queue, int i, int j){
    assert(queue && i >= 0 && i <= queue->cur_size && j >= 0 && j <= queue ->cur_size);
    element *firstElement = (element*)GenericNodeGet(queue->array, i),
            *secondElement = (element*)GenericNodeGet(queue->array, j);
    GenericNodeFree(queue->array, i);
    GenericNodeAdd(queue->array, secondElement, i);
    GenericNodeFree(queue->array, j);
    GenericNodeAdd(queue->array, firstElement, j);
}