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

#define MAX_THREADS 2
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
}



/**
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char **argv)
{
  /* ***** TP3 ***** */
  no_threaded_min_max();

  return EXIT_SUCCESS;
}