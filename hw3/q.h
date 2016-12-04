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
#include "pthread.h"
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
#define NR_PHASES           3
#define NR_PHASE_TYPE       1
#define TRUE                1
#define FALSE               0
    pthread_mutex_t push_lock, peek_lock, pop_lock;     


typedef struct  {
    int thread_id;
int job_id;
int nr_phases;  
int current_phase;
// Phase types: 1 = CPU phase; 2 = IO phase
int phasetype_and_duration[NR_PHASES][NR_PHASE_TYPE];
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
 * Create and initiate a Queue
 */
int createQueue (Queue*);
void init_locks();

/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void init_locks(){
        pthread_mutex_init (&push_lock, NULL);
  pthread_mutex_init (&peek_lock, NULL);
  pthread_mutex_init (&pop_lock, NULL);
    
}


void push (Queue* queue, Job* item) {
// Create a new node
   // pthread_mutex_lock(&push_lock);
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
   // pthread_mutex_unlock(&push_lock);
}
/**
 * Return and remove the first item. returns a Job struct pointer
 */
Job* pop (Queue* queue) {
  //  pthread_mutex_lock(&pop_lock);
    Node* head;
    // get the first item
    if(queue->size != 0){  head = queue->head; }
    if( head != NULL) { Job* item = head->item; 
    // move head pointer to next node, decrease size
    queue->head = head->next;
    queue->size--;
    // free the memory of original head
  //  free(head);
    return item;
    }
    return NULL;
 //   pthread_mutex_unlock(&pop_lock);
}
/**
 * Return but not remove the first item. // need to make it so if list is empty dont crash
 */
Job* peek (Queue* queue) {
   // pthread_mutex_lock(&peek_lock);
    Node* head ; 
    if(queue->head != NULL){
     head = queue->head;
    if( head->item != NULL) return head->item;
    }
    else{
        return -2;
    }
   // pthread_mutex_unlock(&peek_lock);
}

/**
 * Create and initiate a Queue
 */
int createQueue (Queue* queue) {
    
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return 0;
}

