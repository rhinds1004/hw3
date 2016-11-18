/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   q.h
 * Author: hinds
 *
 * Created on November 15, 2016, 3:24 PM
 */

#ifndef Q_H
#define Q_H
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* Q_H */


/**
  * This sample is about how to implement a queue in c
  *
  * Type of item is int
  * Add item to tail
  * Get item from head
  * Can get the size
  * Can display all content
  */
#define NR_PHASES           5
#define TRUE                0
#define FALSE               1
         


typedef struct  {
    int thread_id;
int job_id;
int nr_phases;  
int current_phase;
// Phase types: 1 = CPU phase; 2 = IO phase
int phasetype_and_duration[NR_PHASES][NR_PHASES];
int is_completed;
}Job;

/**
 * The Node struct,
 * contains item and the pointer that point to next node.
 */
typedef struct Node {
    Job* item;
    struct Node* next;
} Node;

/**
 * The Queue struct, contains the pointers that
 * point to first node and last node, the size of the Queue,
 * and the function pointers.
 */
typedef struct Queue {
    Node* head;
    Node* tail;

    void (*push) (struct Queue*, Job*); // add item to tail
    // get item from head and remove it from queue
    Job* (*pop) (struct Queue*);
    // get item from head but keep it in queue
    Job* (*peek) (struct Queue*);
    // display all element in queue
    void (*display) (struct Queue*);
    // size of this queue
    int size;
} Queue;
/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void push (Queue* queue, Job* item);
/**
 * Return and remove the first item.
 */
Job* pop (Queue* queue);
/**
 * Return but not remove the first item.
 */
Job* peek (Queue* queue);
/**
 * Show all items in queue.
 */
void display (Queue* queue);
/**
 * Create and initiate a Queue
 */
int createQueue (Queue*);


/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void push (Queue* queue, Job* item) {
    // Create a new node
    Node* n = (Node*) malloc (sizeof(Node));
    n->item = item;
    n->next = NULL;

    if (queue->head == NULL) { // no head
        queue->head = n;
    } else{
        queue->tail->next = n;
    }
    queue->tail = n;
    queue->size++;
}
/**
 * Return and remove the first item. returns a Job struct pointer
 */
Job* pop (Queue* queue) {
    // get the first item
    Node* head = queue->head;
    Job* item = head->item;
    // move head pointer to next node, decrease size
    queue->head = head->next;
    queue->size--;
    // free the memory of original head
    free(head);
    return item;
}
/**
 * Return but not remove the first item. // need to make it so if list is empty dont crash
 */
Job* peek (Queue* queue) {
    Node* head = queue->head;
    
    return head->item;
}
/**
 * Show all items in queue.
 */
void display (Queue* queue) {
    printf("\nDisplay: ");
    // no item
    if (queue->size == 0)
        printf("No item in queue.\n");
    else { // has item(s)
        Node* head = queue->head;
        int i, size = queue->size;
        printf("%d item(s):\n", queue->size);
        for (i = 0; i < size; i++) {
            if (i > 0)
                printf(", ");
            printf("%d", head->item->job_id);
            head = head->next;
        }
    }
    printf("\n\n");
}
/**
 * Create and initiate a Queue
 */
int createQueue (Queue* queue) {
    
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    queue->push = &push;
    queue->pop = &pop;
    queue->peek = &peek;
    //queue.display = &display;
    return 0;
}

