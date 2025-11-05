/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<getopt.h>

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
void fork_yourself(){
    pid_t pid = fork(); // Create a new process, a child process
    
    if(pid<0){
      perror("Fork error");
      exit(EXIT_FAILURE);
    }

    if(pid==0){ // Child process
      pid_t my_pid = getpid();
      pid_t parent_pid = getppid();
      printf("[Child] PID = %d, PPID = %d\n", my_pid, parent_pid);
      int exit_code = my_pid % 10; // last digit of PID
      exit(exit_code);
    } 
    else { // Parent process
      printf("[Parent] PID child = %d\n", pid);
      int status; // Store child exit status
      waitpid(pid, &status, 0); // Wait for child to finish
      if (WIFEXITED(status)) {
        printf("[Parent] Return code child = %d\n", WEXITSTATUS(status));
      }
    }
}


/**
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
    /* ***** TP2 ***** */

    /* ********** FORK YOURSELF ********** */
    fork_yourself();

    return EXIT_SUCCESS;
}