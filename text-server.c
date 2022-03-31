#include "text-memory.h"

int main() {
  char filename[64];
  char *myfifo = "/tmp/myfifo"; // named pipe path

  mkfifo(myfifo, 0666); // creating named pipe with 0666 permission
  printf("oooofofofof");
  while (1) {

    sem_t *server = sem_open(S_SEM, O_CREAT, 0660, 0);
    sem_t *client = sem_open(C_SEM, O_CREAT, 0660, 1);

    int fd = open(myfifo, O_RDONLY); // opens the pipe read only

    memset(filename, '\0', sizeof(filename));
    // clears the buffer every time.
    int i = 0, iter = 1;

    if (read(fd, filename, sizeof(filename)) > 0 &
        (file_checker(filename) != 0)) {
      // file name
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
      sem_wait(client);

      memset(k, '\0', strlen(k) + 1);
      k[0] = EOT;

      sem_post(server);
      detach_segment(filename);
      printf("\nBYTES SENT: %d\n", i);
      pclose(fp);
    } /*else if (file_checker(filename) == 0) {
       printf("INVALID FILE\n");
    }*/

    close(fd);
  }
  return 0;
}

// anna_karenina.txt 1985015
// lorem_ipsum.txt 578
// dante.txt 222472