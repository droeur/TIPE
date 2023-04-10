#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

typedef struct _element element;
typedef struct _priority_queue priority_queue;

priority_queue *PriorityQueueCreate();
void PriorityQueueFree(priority_queue* queue);

#endif