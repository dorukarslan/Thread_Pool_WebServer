#ifndef QUEUE_H_
#define QUEUE_H_

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int *array;
};

struct Queue *createQueue(unsigned capacity);
int isFull(struct Queue *queue);
int isEmpty(struct Queue *queue);
void enqueue(struct Queue *queue, int item);
int dequeue(struct Queue *queue);
#endif