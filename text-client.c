#include "text-memory.h"
pthread_mutex_t mutex;

int loop = 1;
char word[] = "the journey";

// ************** THREAD FUNCTION ************** //
void *thread(void *arg) {                        //
  pthread_mutex_lock(&mutex);                    //
                                                 //
  char *iptr = arg;                              //
  if (strstr(iptr, word)) {                      //
    fprintf(stdout, "%d\t%s\n", loop, iptr);     //
    loop++;                                      //
  }                                              //
  pthread_mutex_unlock(&mutex);                  //
  pthread_exit(NULL);                            //
}                                                //
// ************** END OF FUNCTION************** //

int main(int argc, char *argv[]) {

// **************** NAMED PIPE ***************** //
                                                 //
  char *myfifo = "/tmp/myfifo", filename[64];    //
  mkfifo(myfifo, 0666);                          //
                                                 //
  strcpy(filename, argv[1]);                     //
  int fd = open(myfifo, O_WRONLY);               //
  write(fd, filename, sizeof(filename));         //
  close(fd);                                     //
                                                 //
// *************** *END OF PIPE **************** //

// ******** CHECKS IF THE FILE IS VALID ******** //

  if (file_checker(argv[1]) == false) {          
    fprintf(stderr, "INVALID FILE\n");            
    exit(ERROR);                                 
  }                                              

// ******* CHECKS IF THE STRING IS VALID ******* //

  if (strlen(argv[2]) == 0) { 
    fprintf(stderr, "INVALID TEXT\n");
    exit(ERROR);
  }

// ******** CREATES MEMORY AND ATTACHES ******** //
  char *memory = attach_segment(filename);

// ** WAITING FOR THE SERVER TO FINISH WRITING * //
  while (memory[sizeof(memory)] != EOT) {
    /* Waits */
  }

// ***** CONVERTING 1D *CHAR to 2D **CHAR ****** //

  size_t n = 0;
  char *memory_array[ARR_SIZE] = {NULL};

  for (char *p = strtok(memory, "\n"); n < ARR_SIZE && p;
       p = strtok(NULL, "\n")) {
    memory_array[n++] = p;
  }

// ******* INITIALIZING THREADS AND MUTEX ******* //

  pthread_t threads[THREADS];
  pthread_mutex_init(&mutex, NULL);

// ** PARSING THROUGHT EACH LINE USING STRINGS ** //

  int iter = 0, use = 0;
  for (int j = 0; j < ceil(n / THREADS) + 1; j++) {
    if (floor(n / THREADS) > iter) {
      use = THREADS;
    } else {
      use = n % THREADS;
    }

// ************ SPINNING THREADS **************** //

    for (int i = 0; i < use; i++) {
      int index = i + j * THREADS;
      strcpy(word, argv[2]);
      if (pthread_create(&threads[i % THREADS], NULL, thread,
                         memory_array[index]) != 0) {
        perror("thread failed to create\n");
      }
    }

// ************** JOINING THREADS **************** //

    for (int i = 0; i < use; i++) {
      if (pthread_join(threads[i % THREADS], NULL) != 0) {
        perror("failed to join\n");
      }
    }
    iter++;
  }

// ***** DESTROYING MUTEX AND MEMORY SEGMENT ****** //
  pthread_mutex_destroy(&mutex);
  destroy_segment(filename);

  return 0;
}