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

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);
    
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "input",   required_argument, 0, 'i' },
  { "output",  required_argument, 0, 'o' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hvi:o:";

void print_reverse(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) { perror("Erreur ouverture fichier"); exit(EXIT_FAILURE); }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) { perror("Erreur lseek SEEK_END"); close(fd); exit(EXIT_FAILURE); }

    char c;
    for (off_t i = 1; i <= file_size; i++) { // Loop through the file in reverse
        if (lseek(fd, -i, SEEK_END) == -1) { perror("Erreur lseek boucle"); close(fd); exit(EXIT_FAILURE); } // Move to the i-th byte from the end
        if (read(fd, &c, 1) != 1) { perror("Erreur lecture"); close(fd); exit(EXIT_FAILURE); } // Read one byte
        write(STDOUT, &c, 1);
    }
    close(fd);
}

void mode_to_perms(mode_t mode, char* str) {
    str[0] = (S_ISDIR(mode)) ? 'd' : '-';
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';
}

void ls_like(const char* path) {
    DIR* dir = opendir(path);
    if (!dir) {
        perror("Erreur ouverture répertoire");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    struct stat st;
    char fullpath[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL) { // Read each directory entry
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // Skip . and ..
          continue;

      snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name); // Construct full path

      if (stat(fullpath, &st) == -1) { // Get file stats
          perror("Erreur stat");
          continue;
      }

      char perms[11];
      mode_to_perms(st.st_mode, perms);

      struct passwd* pw = getpwuid(st.st_uid);
      struct group* gr = getgrgid(st.st_gid);

      char date[64];
      struct tm* tm_info = localtime(&st.st_mtime);
      strftime(date, sizeof(date), "%d%m%y @ %Hh%M", tm_info);

      dprintf(STDOUT, "%-20s %s %s:%s %8lld %s\n",
              entry->d_name,
              perms,
              pw ? pw->pw_name : "unknown",
              gr ? gr->gr_name : "unknown",
              st.st_size,
              date);
    }

    closedir(dir);
}

void affiche_message(const char *message, char *prog_a_exec) {
  /* PT1 */
  // if(message == NULL) {
  //     fprintf(stderr, "Message NULL reçu\n");
  //     exit(EXIT_FAILURE);
  // }
  // printf("Mot : %s\n", message);

  /* PT2 */
  pid_t pid = fork();
  if(pid < 0) {
      perror("Error creating child process (fork)");
      exit(EXIT_FAILURE);
  }

  if(pid == 0){ // Child process
    fprintf(stdout, "Child [PID %d] ", getpid());

    close(1); // Close stdout

    char template[] = "/tmp/proc-exerciseXXXXXX"; 
    // using mkstemp for unique temp file
    // XXXXXX will be replaced by mkstemp to create a unique filename
    int fd = mkstemp(template);
    if(fd < 0) {
        perror("Error creating temp file");
        exit(EXIT_FAILURE);
    }

    printf("fd open in child: %d\n", fd);

    // Redirect stdout to the temp file
    if (dup2(fd, STDOUT_FILENO) == -1) {
          perror("dup2");
          exit(EXIT_FAILURE);
      }

      execlp(prog_a_exec, prog_a_exec, NULL); // Execute the program
      perror("execlp failed");
      exit(EXIT_FAILURE);
  } else { // Parent process
    fprintf(stdout, "Parent [PID %d] ", getpid());

    int status;
    waitpid(pid, &status, 0); // Wait for child to finish

    if (WIFEXITED(status)) {
        printf("That's all folks, exit status: %d\n", WEXITSTATUS(status));
    }
  }
}

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;
  char* bin_input_param = NULL;
  char* bin_output_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'i':
        //input param
        if (optarg)
        {
          bin_input_param = dup_optarg_str();         
        }
        break;
      case 'o':
        //output param
        if (optarg)
        {
          bin_output_param = dup_optarg_str();
        }
        break;
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  if (bin_input_param == NULL || bin_output_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);
    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
  }


  // Printing params
  dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n", 
          "input",   bin_input_param, 
          "output",  bin_output_param, 
          "verbose", is_verbose_mode);

  // Business logic must be implemented at this point

  /* ***** TP1 ***** */

  /* ********** PRINT IN REVERSE ********** */
  // print_reverse(bin_input_param);
  // printf("\n\n");

  // ********** LS COMMAND ********** //
  // ls_like(bin_input_param);
  // printf("\n\n");


  /* ********** AFFICHE MESSAGE ********** */
  affiche_message(bin_input_param, bin_output_param);

  // Freeing allocated data
  free_if_needed(bin_input_param);
  free_if_needed(bin_output_param);

  return EXIT_SUCCESS;
}

