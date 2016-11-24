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
#include <sys/types.h>
#include <unistd.h>

#define JOB_SUB_THREAD_COUNT 4
#define CPU_THREAD_COUNT   8
#define IO_THREAD_COUNT    4
#define RUN_LENGTH          20
#define JOB_CREATE_ITERVAL   2
#define PHASE_TYPE_CPU      1
#define PHASE_TYPE_IO      2
#define TOTAL_PHASE_TYPES 2
#define TOTAL_JOBS         20
pthread_mutex_t padlock, peeklock, cpu_thread_lock, finished_lock;
pthread_mutex_t rnr_pop_lock, rnr_push_lock;
pthread_mutex_t fnsh_pop_lock, fnsh_push_lock;
pthread_mutex_t io_pop_lock, io_push_lock;
int total_jobs = 0;
int total_finished_jobs = 0;
Queue run_rdy_q = {0};
Queue finished_q ={0};
Queue io_q = {0};
/*
 * 
 */

typedef struct
{
  int thread_ID;
  Queue* finish_queue;
  Queue* rdy_to_run_q;
  Queue* io_q;
} Info_for_job;

  pthread_t job_sub_threads[JOB_SUB_THREAD_COUNT];
  pthread_t cpu_threads[CPU_THREAD_COUNT];
  pthread_t io_threads[IO_THREAD_COUNT];
Job* createJob (long thread_ID);
void* job_controller (void* inf);
void* cpu_thread (void* inf);
void* io_thread (void* inf);
void test_qs(Queue*, Queue*);


int
main (int argc, char** argv)
{
  //init of threads

  //init of locks
  pthread_mutex_init (&padlock, NULL);
  pthread_mutex_init (&peeklock, NULL);
  pthread_mutex_init (&cpu_thread_lock, NULL);
  pthread_mutex_init (&finished_lock, NULL);
pthread_mutex_init(&rnr_pop_lock, NULL);
pthread_mutex_init(&rnr_push_lock, NULL);
pthread_mutex_init(&fnsh_pop_lock, NULL);
pthread_mutex_init(&fnsh_push_lock, NULL);
pthread_mutex_init(&io_pop_lock, NULL);
pthread_mutex_init(&io_push_lock, NULL);

  init_locks();
  
srand(time(NULL));
  int i = 0;

  createQueue (&run_rdy_q);
   

  createQueue (&finished_q);
  

  createQueue (&io_q);
  long sub_ID[JOB_SUB_THREAD_COUNT] = {0};
 long cpu_ID[CPU_THREAD_COUNT] = {0};
  long io_ID[IO_THREAD_COUNT] = {0};
  

  for (i = 0; i < JOB_SUB_THREAD_COUNT; i++)
    {
      sub_ID[i] = i;
    }
  for (i = 0; i < JOB_SUB_THREAD_COUNT; i++)
    {

      pthread_create (&job_sub_threads[i], NULL, job_controller, (void*) &sub_ID[i]);
    }

    for (i = 0; i < CPU_THREAD_COUNT; i++)
    {
        cpu_ID[i] = i;
    }
     for (i = 0; i < IO_THREAD_COUNT; i++)
    {
         io_ID[i] = i;
    }
/*
    for (i = 0; i < IO_THREAD_COUNT; i++)
    {
      pthread_create (&io_threads[i], NULL, io_thread, (void*) &io_ID[i]);
    }
    for (i = 0; i < CPU_THREAD_COUNT; i++)
    {

      pthread_create (&cpu_threads[i], NULL, cpu_thread, (void*) &cpu_ID[i]);
    }  
*/
  

/*
  for (i = 0; i < CPU_THREAD_COUNT; i++)
    {

      pthread_join (cpu_threads[i], NULL);
    }
    for (i = 0; i < IO_THREAD_COUNT; i++)
    {

      pthread_join (io_threads[i], NULL);
    }
*/
    for (i = 0; i < JOB_SUB_THREAD_COUNT; i++)
    {

      pthread_join (job_sub_threads[i], NULL);
    }
  pthread_mutex_destroy (&padlock);
 pthread_mutex_destroy (&peeklock);
  pthread_mutex_destroy (&cpu_thread_lock);
pthread_mutex_destroy (&finished_lock);
pthread_mutex_destroy (&rnr_pop_lock);
pthread_mutex_destroy (&rnr_push_lock);
pthread_mutex_destroy (&fnsh_push_lock);
pthread_mutex_destroy (&fnsh_pop_lock);
pthread_mutex_destroy (&io_pop_lock);
pthread_mutex_destroy (&io_push_lock);
/*
    free(run_rdy_q);
    free(finished_q);
    free(io_q);
*/
            
  return (EXIT_SUCCESS);
}

//init jobs

Job* createJob (long thread_ID)
{
  int i = 0, j = 0, phase_sel = 0;;
  Job* new_job = (Job*) calloc (sizeof (Job), 1);
  pthread_mutex_lock (&padlock);
  total_jobs++;
  new_job->job_id = total_jobs;
  new_job->thread_id = thread_ID;
  new_job->nr_phases = NR_PHASES;

  pthread_mutex_unlock (&padlock);
  for(i = 0; i < NR_PHASES; i++){
      for (j = 0; j < NR_PHASE_TYPE; j++){
          if(j == 0 && i == 0){
              new_job->phasetype_and_duration[i][j] = PHASE_TYPE_CPU;
            }
          else{
              phase_sel = (rand()%TOTAL_PHASE_TYPES)+1;
              new_job->phasetype_and_duration[i][j] = phase_sel;
            }
        }
    }
  return new_job;
}
void* cpu_thread (void* inf)
{
/*
    Info_for_job* info = (Info_for_job*) inf;
  int my_thread_ID = info->thread_ID; 
   int check_ID = 0;
   time_t prev_time = time (NULL);

     Job* job_to_run = (Job*)calloc(sizeof(Job), 1);


  if (run_rdy_q.size != 0 || run_rdy_q.head != NULL) 

            {
                pthread_mutex_lock (&rnr_pop_lock);
                job_to_run = pop (&run_rdy_q);
                  pthread_mutex_unlock (&rnr_pop_lock);  
                if(job_to_run != NULL){


                  if(job_to_run->phasetype_and_duration[job_to_run->current_phase][NR_PHASE_TYPE] == PHASE_TYPE_CPU){
                      printf("executing Job: %d On cpu_thread: %d\n", job_to_run->job_id, job_to_run->thread_id); // crash here
                      prev_time = time (NULL);
                     while( time (NULL) - prev_time < PHASE_TYPE_CPU){
                         prev_time = time(NULL);
                       }

                  (job_to_run->current_phase)++;
                  if(job_to_run->current_phase == job_to_run->nr_phases) job_to_run->is_completed = TRUE;
                    }
                  if(job_to_run->is_completed == TRUE){
                      pthread_mutex_lock (&fnsh_push_lock);
                      info->finish_queue->push(info->finish_queue, job_to_run );
                       pthread_mutex_unlock (&fnsh_push_lock);
                      printf("transffering Job: %d On cpu_thread: %d to finished Queue\n", job_to_run->job_id, job_to_run->thread_id);
                      pthread_exit((void*)job_to_run->is_completed);
                    }
                  if(job_to_run->phasetype_and_duration[job_to_run->current_phase][NR_PHASE_TYPE] == PHASE_TYPE_CPU){
                   //   printf("transffering Job: %d On cpu_thread: %d to Ready to Run Queue\n", job_to_run->job_id, job_to_run->thread_id);
                       pthread_mutex_lock (&rnr_push_lock);
                      info->rdy_to_run_q->push(info->rdy_to_run_q, job_to_run);
                       pthread_mutex_unlock (&rnr_push_lock);
                      printf("transffering Job: %d On cpu_thread: %d to Ready to Run Queue\n", job_to_run->job_id, job_to_run->thread_id);
                      printf("phase %d Number of Phases left: %d\n", job_to_run->current_phase,(job_to_run->nr_phases-job_to_run->current_phase));
                    }
                  else if(job_to_run->phasetype_and_duration[job_to_run->current_phase][NR_PHASE_TYPE] == PHASE_TYPE_IO){
                    //  ;
                       pthread_mutex_lock (&io_push_lock);
                      info->io_q->push(info->io_q, job_to_run);
                       pthread_mutex_unlock (&io_push_lock);
                      printf("transffering Job: %d On cpu_thread: %d to IO Queue\n", job_to_run->job_id, job_to_run->thread_id);
                      printf("phase %d Number of Phases left: %d\n", job_to_run->current_phase,(job_to_run->nr_phases-job_to_run->current_phase));
                    }
                  }
            } 
       
     free(job_to_run);
*/
     pthread_exit(NULL);
}

void* io_thread (void* thread_ID_Array)
{
 // long my_thread_ID = *(long *)thread_ID_Array; 
/*
   int check_ID = 0;
   time_t prev_time = time (NULL);
Job* job_to_run;
    // Job* job_to_run = (Job*)calloc(sizeof(Job), 1);

     while (total_finished_jobs < 20){
if((time(NULL) - prev_time) >  1){
           sleep(2);
           prev_time = time(NULL);
         }
  if ((info->io_q->size != 0 || info->io_q->head != NULL))
            {
                        pthread_mutex_lock (&io_pop_lock);
                job_to_run = pop(&io_q);
                  pthread_mutex_unlock (&io_pop_lock);
                

                  
   
                  if(job_to_run->phasetype_and_duration[job_to_run.current_phase][NR_PHASE_TYPE] == PHASE_TYPE_IO){
                      printf("executing Job: %d On io_thread: %d\n", job_to_run.job_id, job_to_run.thread_id); // crash here
                      prev_time = time (NULL);
                     while( time (NULL) - prev_time < PHASE_TYPE_IO){
                         prev_time = time(NULL);
                       }
                 // sleep((job_to_run->phasetype_and_duration[job_to_run->current_phase][NR_PHASE_TYPE]));
                  (job_to_run->current_phase)++;
                  if(job_to_run->current_phase == job_to_run->nr_phases) job_to_run->is_completed = TRUE;
                    }
                  if(job_to_run->is_completed == TRUE){
                       pthread_mutex_lock (&fnsh_push_lock);
                      push(&finished_q, job_to_run );
                       pthread_mutex_unlock (&fnsh_push_lock);
                      printf("transffering Job: %d On io_thread: %d to finished Queue\n ", job_to_run->job_id, job_to_run->thread_id);
                      pthread_exit((void*)&job_to_run.is_completed);
                    }
                  else if(job_to_run.phasetype_and_duration[job_to_run.current_phase][NR_PHASE_TYPE] == PHASE_TYPE_CPU){
                       pthread_mutex_lock (&rnr_push_lock);
                      info->rdy_to_run_q->push(info->rdy_to_run_q, &job_to_run);
                       pthread_mutex_unlock (&rnr_push_lock);
                      printf("transffering Job: %d On io_thread: %d to Ready to Run Queue\n", job_to_run.job_id, job_to_run.thread_id);
                      printf("phase %d Number of Phases left: %d\n", job_to_run.current_phase,(job_to_run.nr_phases-job_to_run.current_phase));
                    }
                  else if(job_to_run.phasetype_and_duration[job_to_run.current_phase][NR_PHASE_TYPE] == PHASE_TYPE_IO){
                       pthread_mutex_lock (&io_push_lock);
                      info->io_q->push(info->io_q, &job_to_run);
                       pthread_mutex_unlock (&io_push_lock);
                      printf("transffering Job: %d On io_thread: %d to IO Queue\n", job_to_run.job_id, job_to_run.thread_id);
                      printf("phase %d Number of Phases left: %d\n", job_to_run.current_phase,(job_to_run.nr_phases-job_to_run.current_phase));
                    }
            } 
       }
    // free(job_to_run);
*/
     pthread_exit(NULL);
}
//

void* job_controller (void* thread_ID_Array)
{
  long my_thread_ID = *(long *)thread_ID_Array;
  time_t init_time = time (NULL);
  time_t end_time = init_time;
  time_t prev_time = time (NULL);
  Job* finished_job;
  Job* new_job;
  int check_ID = -2;
  
  //while(time(NULL) - init_time < RUN_LENGTH){

  while (total_finished_jobs < 20)   //needs to change since it will stop trying to empty the finish queue once there are 20 jobs created.
    {

      if (time (NULL) - prev_time > JOB_CREATE_ITERVAL && total_jobs != TOTAL_JOBS)
        {
            new_job = createJob (my_thread_ID);
        //    pthread_mutex_lock (&rnr_push_lock);
        //  push (&run_rdy_q, new_job);
          push (&finished_q, new_job);
          printf("pushed Job : %d onto Ready to Run queue\n", new_job->job_id);
          prev_time = time (NULL);
        }
 //     else
 //       {
            if (time (NULL) - end_time > 1){
        //    pthread_mutex_lock (&peeklock);
          if (finished_q.size != 0)
          { 
              check_ID = peek(&finished_q)->thread_id;//segment signal here
         //   pthread_mutex_unlock (&peeklock);
              
              if (check_ID != -2 && check_ID == my_thread_ID)
                { //<-- peek crashes if queue empty?
          
                  pthread_mutex_lock (&fnsh_pop_lock);//
                  finished_job = pop(&finished_q);
                 pthread_mutex_unlock (&fnsh_pop_lock);
                  if(finished_job != NULL){
                  printf ("freeing job: %d freed by thread: %ld\n", finished_job->job_id, my_thread_ID);
                  pthread_mutex_lock (&peeklock);
                  free (finished_job);
                   pthread_mutex_unlock (&peeklock);
                   
                  pthread_mutex_lock(&finished_lock);
                  total_finished_jobs++;
                   pthread_mutex_unlock(&finished_lock);
                    }
                  //  pthread_mutex_unlock (&peeklock);
                }//if 
            }
          end_time = time(NULL);
              }//   }//else
  
    }//while loop


pthread_exit(NULL);
}

void test_qs(Queue* finished_q, Queue* rdy_to_run){
   Job* tempJob = (Job*) calloc (sizeof (Job), 1);
  tempJob->job_id = 7;
  tempJob->thread_id = 0;
  push (finished_q, tempJob);
   Job* tempJob2 = (Job*) calloc (sizeof (Job), 1);
  tempJob2->job_id = 3;
  tempJob2->thread_id = 1;
   // push (&finished_q, tempJob2); 
   
}