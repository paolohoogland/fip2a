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
void fork_yourself()
{
  pid_t pid = fork(); // Create a new process, a child process

  if (pid < 0)
  {
    perror("Fork error");
    exit(EXIT_FAILURE);
  }

  if (pid == 0)
  { // Child process
    pid_t my_pid = getpid();
    pid_t parent_pid = getppid();
    printf("[Child] PID = %d, PPID = %d\n", my_pid, parent_pid);
    int exit_code = my_pid % 10; // last digit of PID
    exit(exit_code);
  }
  else
  { // Parent process
    printf("[Parent] PID child = %d\n", pid);
    int status;               // Store child exit status
    waitpid(pid, &status, 0); // Wait for child to finish
    if (WIFEXITED(status))
    {
      printf("[Parent] Return code child = %d\n", WEXITSTATUS(status));
    }
  }
}

void affiche_message(const char *message, char *prog_a_exec)
{
  /* PT1 */
  // if(message == NULL) {
  //     fprintf(stderr, "Message NULL reçu\n");
  //     exit(EXIT_FAILURE);
  // }
  // printf("Mot : %s\n", message);

  /* PT2 */
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("Error creating child process (fork)");
    exit(EXIT_FAILURE);
  }

  if (pid == 0)
  { // Child process
    fprintf(stdout, "Child [PID %d] ", getpid());

    close(STDOUT); // Close stdout

    char template[] = "/tmp/proc-exerciseXXXXXX";
    // using mkstemp for unique temp file
    // XXXXXX will be replaced by mkstemp to create a unique filename
    int fd = mkstemp(template);
    if (fd < 0)
    {
      perror("Error creating temp file");
      exit(EXIT_FAILURE);
    }

    // Reopen stdout to terminal before printing
    int stdout_fd = open("/dev/tty", O_WRONLY);
    if (stdout_fd != -1)
    {
      dup2(stdout_fd, STDOUT_FILENO);
      close(stdout_fd);
    }
    printf("fd open in child: %d\n", fd);

    // Redirect stdout to the temp file
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
      perror("dup2");
      exit(EXIT_FAILURE);
    }

    execlp(prog_a_exec, prog_a_exec, NULL); // Execute the program
    perror("execlp failed");
    exit(EXIT_FAILURE);
  }
  else
  { // Parent process
    fprintf(stdout, "Parent [PID %d] ", getpid());

    int status;
    waitpid(pid, &status, 0); // Wait for child to finish

    if (WIFEXITED(status))
    {
      printf("That's all folks, exit status: %d\n", WEXITSTATUS(status));
    }
  }
}


void pipe_exercise(){
  int pipefd[2];
  pid_t pid1, pid2;
  int status;
  
  // Create pipe
  if (pipe(pipefd) == -1) {
    perror("pipe");
    return;
  }
  
  // First fork for ps command
  pid1 = fork();
  if (pid1 == -1) {
    perror("fork");
    return;
  }
  
  if (pid1 == 0) {
    // Child 1: ps eaux
    close(pipefd[0]); // Close read end
    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
    close(pipefd[1]);
    execlp("ps", "ps", "eaux", NULL);
    perror("execlp ps");
    exit(1);
  }
  
  // Second fork for grep command
  pid2 = fork();
  if (pid2 == -1) {
    perror("fork");
    return;
  }
  
  if (pid2 == 0) {
    // Child 2: grep "^root "
    close(pipefd[1]); // Close write end
    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
    close(pipefd[0]);
    
    // Redirect stdout to /dev/null
    int devnull = open("/dev/null", O_WRONLY);
    dup2(devnull, STDOUT_FILENO);
    close(devnull);
    
    execlp("grep", "grep", "^root ", NULL);
    perror("execlp grep");
    exit(1);
  }
  
  // Parent: close pipe ends and wait for children
  close(pipefd[0]);
  close(pipefd[1]);
  
  // Wait for ps to finish
  waitpid(pid1, &status, 0);
  
  // Wait for grep to finish and check its exit status
  waitpid(pid2, &status, 0);
  
  if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
    // grep found "root", write message
    write(STDOUT_FILENO, "root est connecté\n", 18);
  }
}

/**
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char **argv)
{
  /* ***** TP2 ***** */

  /* ********** FORK YOURSELF ********** */
  //fork_yourself();

  /* ********** AFFICHE MESSAGE ********** */
  //affiche_message("Hello World!", "/bin/ls");

  /* ********** PIPE EXERCISE ********** */
  pipe_exercise();

  return EXIT_SUCCESS;
}