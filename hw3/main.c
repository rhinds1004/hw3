/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: hindsr
 *
 * Created on November 17, 2016, 8:56 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "q.h"
#include "pthread.h"
#include "time.h"

#define JOB_SUB_THREAD_COUNT 2
#define RUN_LENGTH          20
#define JOB_CREATE_ITERVAL   2
pthread_mutex_t padlock, peeklock;
int total_jobs = 0;
/*
 * 
 */

typedef struct {
  int   thread_ID;
  Queue* finish_queue;
}Info_for_job;

void* createJob(int* thread_ID);
void* job_controller(void* inf);



  
int main (int argc, char** argv)
{
  pthread_t job_sub_threads[JOB_SUB_THREAD_COUNT];
   pthread_mutex_init(&padlock, NULL);
    pthread_mutex_init(&peeklock, NULL);
    
  int i = 0;
  //  Queue* run_rdy_q = createQueue();
 // Queue* io_q = createQueue();
  Queue* finished_q = NULL;
  finished_q = (Queue*)malloc(sizeof(Queue)*1);
  createQueue(finished_q);
  Info_for_job info[JOB_SUB_THREAD_COUNT] = {0};
 // Job tempJob;
 Job* tempJob = (Job*)calloc(sizeof(Job),1);
    tempJob->job_id = 7;
  tempJob->thread_id = 0;
/*
  tempJob.job_id = 7;
  tempJob.thread_id = 3;
*/
  
  //crashed because of this
  //info[0].finish_queue->push(finished_q, tempJob);

finished_q->push(finished_q, tempJob);
Job* rejob = finished_q->peek(finished_q);
  //Info_for_job* info = (Info_for_job*)calloc(sizeof(Info_for_job), 1);
  for(i = 0; i < JOB_SUB_THREAD_COUNT; i++){
            
      info[i].finish_queue = finished_q;
      info[i].thread_ID = i;
      
    }
  for( i = 0; i < JOB_SUB_THREAD_COUNT; i++){

  pthread_create(&job_sub_threads[i], NULL, job_controller, (void*) &info[i]);
    }
 //job_controller(info);
  for( i = 0; i < JOB_SUB_THREAD_COUNT; i++){

  pthread_join(job_sub_threads[i],NULL);
    }
pthread_mutex_destroy(&padlock);
pthread_mutex_destroy(&peeklock);
  return (EXIT_SUCCESS);
}

//init jobs
void* createJob(int* thread_ID){
 // printf("thread %d: testing : job number: %d\n", *thread_ID, total_jobs);
        pthread_mutex_lock (&padlock);
      total_jobs++;
      printf("thread %d: testing : job number: %d\n", *thread_ID, total_jobs);
      pthread_mutex_unlock (&padlock);
 
}

//
void* job_controller(void* inf){
  Info_for_job* info = (Info_for_job*)inf;
  int my_thread_ID = info->thread_ID;
  time_t   init_time = time(NULL);
  time_t start_time = init_time;
  time_t prev_time = time(NULL);
  Job* finished_job;
  int check_ID = 0;
 // info->finish_queue->peek(info->finish_queue)->thread_id;
  //while(time(NULL) - init_time < RUN_LENGTH){
  while(total_jobs < 20){
      if(time(NULL)- prev_time > JOB_CREATE_ITERVAL){
      createJob(&my_thread_ID);
      prev_time = time(NULL);

        }
      else{
        //  pthread_mutex_lock (&peeklock);
          if(info->finish_queue->size != 0){
          
          
         // finished_job = info->finish_queue->peek(info->finish_queue)->thread_id;
     //     printf("id of job on queue: %d", finished_job);
            check_ID = info->finish_queue->peek(info->finish_queue)->thread_id;        
         if( check_ID == my_thread_ID){  //<-- peek crashes if queue empty?
              //engage lock

              finished_job = info->finish_queue->pop(info->finish_queue);
              printf("freeing job: %d freed by thread: %d\n", finished_job->job_id, my_thread_ID);
              free(finished_job);
            //  pthread_mutex_unlock (&peeklock);
              

              //free lock
           }
            }

          
        }
    }

  return NULL;
  //pthread_exit;
}