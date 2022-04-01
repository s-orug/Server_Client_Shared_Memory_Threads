#include "text-memory.h"

int main() {

  sem_unlink(S_SEM);
  sem_unlink(C_SEM);
  while (1) {
    char filename[64];
    char *myfifo = "/tmp/myfifo"; // named pipe path

    mkfifo(myfifo, 0666); // creating named pipe with 0666 permission

    sem_t *server = sem_open(S_SEM, O_CREAT, 0666, 0);
    sem_t *client = sem_open(C_SEM, O_CREAT, 0666, 1);

    int fd = open(myfifo, O_RDONLY); // opens the pipe read only

    memset(filename, '\0', sizeof(filename));

    int i = 0, iter = 1;
    if (read(fd, filename, sizeof(filename)) > 0 &
        (file_checker(filename) != 0)) {

      char *k = attach_segment(filename), line[2048];

      FILE *fp;

      fp = fopen(filename, "r");
      while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        sem_wait(client);
        strncpy(k, line, BLOCK_SIZE);
        i += strlen(k);
        printf("%d\t%s", iter, k);
        iter++;

        sem_post(server);
      }
      pclose(fp);

      sem_wait(client);
      memset(k, EOT, BLOCK_SIZE);
      sem_post(server);
      printf("\nBYTES SENT: %d\n", i);
    }
    // detach_segment(filename);
    close(fd);
    sem_unlink(S_SEM);
    sem_unlink(C_SEM);
  }
  return 0;
}
