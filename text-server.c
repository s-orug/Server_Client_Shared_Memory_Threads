#include "text-memory.h"

volatile sig_atomic_t stop;

int main() {

  sem_unlink(S_SEM);
  sem_unlink(C_SEM);
  fprintf(stdout, "SERVER STARTED");
  while (1) {
    char filename[64];
    char *myfifo = "/tmp/myfifo"; // named pipe path

    mkfifo(myfifo, 0666); // creating named pipe with 0666 permission

    sem_t *server = sem_open(S_SEM, O_CREAT, 0666, 0);
    sem_t *client = sem_open(C_SEM, O_CREAT, 0666, 1);

  /*  void sig_handler(int signum){
      sem_unlink(S_SEM);
      sem_unlink(C_SEM);

      sem_close(server);
      sem_close(client);
      exit(0);
    }
    signal(SIGINT, sig_handler);*/
    //if(stop == 1) continue;
    int fd = open(myfifo, O_RDONLY); // opens the pipe read only

    memset(filename, '\0', sizeof(filename));

    int i = 0, iter = 1;
    if (read(fd, filename, sizeof(filename)) > 0 &
        (file_checker(filename) != 0)) {
      fprintf(stderr, "CLIENT REQUEST RECEIVED");
      char *k = attach_segment(filename), line[2048];
      fprintf(stderr, "\tMEMORY OPEN");

      FILE *fp;

      fp = fopen(filename, "r");
      fprintf(stderr, "\tOPENING: %s", filename);

      while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        sem_wait(client);
        strncpy(k, line, BLOCK_SIZE);
        i += strlen(k);
        printf("%d\t%s", iter, k);
        iter++;

        sem_post(server);
      }
      pclose(fp);
      fprintf(stderr, "\tFILE CLOSED");

      sem_wait(client);
      memset(k, EOT, BLOCK_SIZE);
      sem_post(server);

      detach_segment(filename);
      destroy_segment(filename);
      fprintf(stderr, "\tMEMORY CLOSED");
    }
    // MEMORY CLOSING PROBLEM
    close(fd);

    sem_unlink(S_SEM);
    sem_unlink(C_SEM);

    sem_close(server);
    sem_close(client);
  }
  return 0;
}
