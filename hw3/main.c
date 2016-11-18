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
  int i = 0;
    Queue run_rdy_q = createQueue();
  Queue io_q = createQueue();
  Queue finished_q = createQueue();
  for( i = 0; i < JOB_SUB_THREAD_COUNT; i++){
      Info_for_job* info = (Info_for_job*)calloc(sizeof(Info_for_job), 1);
      info->finish_queue = &finished_q;
      info->thread_ID = i;
      job_controller(info);
  pthread_create(&job_sub_threads[i], NULL, job_controller, info);
    }
 //job_controller(info);
  //pthread_join(job_sub_threads[0],NULL);
  return (EXIT_SUCCESS);
}

//init jobs
void* createJob(int* thread_ID){
  printf("testing");
}

//
void* job_controller(void* inf){
  Info_for_job* info = (Info_for_job*)inf;
  int my_thread_ID = info->thread_ID;
  time_t   init_time = time(NULL);
  time_t start_time = init_time;
  time_t prev_time = time(NULL);
  Job* finished_job;

  //while(time(NULL) - init_time < RUN_LENGTH){
  while(total_jobs < 20){
      if(time(NULL)- prev_time > JOB_CREATE_ITERVAL){
      createJob(&my_thread_ID);
      prev_time = time(NULL);
      total_jobs++;
        }
      else{
        /*  if(info->finish_queue->peek(info->finish_queue)->thread_id == my_thread_ID){
              //engage lock

              finished_job = info->finish_queue->pop(info->finish_queue);
              free(finished_job);

              //free lock
              
            }
      */  }
    }

  return NULL;
  //pthread_exit;
}