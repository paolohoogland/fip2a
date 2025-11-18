/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define STDOUT 1
#define STDERR 2

#define SIZE (int)1e8
int tab[SIZE];

#define MAX_THREADS 128
pid_t tids[MAX_THREADS];
int mins[MAX_THREADS];
int maxs[MAX_THREADS];

void no_threaded_min_max(){
  //fill tab with random values
  for(int i=0; i<SIZE; i++){
    tab[i] = rand() % SIZE;
  }

  int min;
  int max;
  struct timeval tv;
  struct timeval tv2;
  gettimeofday(&tv, NULL);

  for(int i=0; i<SIZE; i++){
    if(i==0){
      min = tab[i];
      max = tab[i];
    }
    else{
      if(tab[i] < min){
        min = tab[i];
      }
      if(tab[i] > max){
        max = tab[i];
      }
    }
  }

  gettimeofday(&tv2, NULL);
  int chrono = (tv2.tv_sec - tv.tv_sec) * 1000000 + (tv2.tv_usec - tv.tv_usec);

  printf("No threaded min: %d\n", min);
  printf("No threaded max: %d\n", max);
  printf("No threaded time: %d\n", chrono);
}


void threaded_min_max(){
  //fill tab with random values
  for(int i=0; i<SIZE; i++){
    tab[i] = rand() % SIZE;
  }

  int pipes[MAX_THREADS][2]; // One pipe per child

  struct timeval tv;
  struct timeval tv2;
  gettimeofday(&tv, NULL);
  

  for(int i = 0; i<MAX_THREADS; i++){
    // Create pipe before fork
    if(pipe(pipes[i]) == -1){
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid < 0){
      perror("Fork error");
      exit(EXIT_FAILURE);
    }
    if(pid == 0){
      // Child process - close read end
      close(pipes[i][0]);

      //child process
      int start = i * (SIZE / MAX_THREADS);
      int end = (i + 1) * (SIZE / MAX_THREADS);
      int min;
      int max;
      for(int j=start; j<end; j++){
        if(j==start){
          min = tab[j];
          max = tab[j];
        }
        else{
          if(tab[j] < min){
            min = tab[j];
          }
          if(tab[j] > max){
            max = tab[j];
          }
        }
      }
      // Send results through pipe
      write(pipes[i][1], &min, sizeof(int));
      write(pipes[i][1], &max, sizeof(int));
      close(pipes[i][1]);
      
      exit(0);
    }
  }


  // Read results from pipes
  for(int i = 0; i<MAX_THREADS; i++){
    read(pipes[i][0], &mins[i], sizeof(int));
    read(pipes[i][0], &maxs[i], sizeof(int));
    close(pipes[i][0]);
    
    int status;
    waitpid(tids[i], &status, 0);
  }



  // Rest of your code remains the same...
  int final_min, final_max;
  for(int i=0; i<MAX_THREADS; i++){
    if(i==0){
      final_min = mins[i];
      final_max = maxs[i];
    }
    else{
      if(mins[i] < final_min){
        final_min = mins[i];
      }
      if(maxs[i] > final_max){
        final_max = maxs[i];
      }
    }
  }

  gettimeofday(&tv2, NULL);
  int chrono = (tv2.tv_sec - tv.tv_sec) * 1000000 + (tv2.tv_usec - tv.tv_usec);

  printf("Threaded min: %d\n", final_min);
  printf("Threaded max: %d\n", final_max);
  printf("Threaded time: %d\n", chrono);
}



/**
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char **argv)
{
  /* ***** TP3 ***** */
  no_threaded_min_max();

  threaded_min_max();

  return EXIT_SUCCESS;
}