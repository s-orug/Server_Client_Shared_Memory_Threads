#include "text-memory.h"

int main(int argc, char *argv[]) {

  pthread_t threads[THREADS];

  char *myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);

  sem_t *server = sem_open(S_SEM, 0);
  sem_t *client = sem_open(C_SEM, 1);

  int fd = open(myfifo, O_WRONLY);
  write(fd, argv[1], strlen(argv[1]));
  char *k = attach_segment(argv[1]);

  int i = 0, iter = 1;
  while (true) {
    sem_wait(server);

    if (strlen(k) > 0) {
      if (k[0] == EOT)
        break;
      i += strlen(k);
      printf("%d\t%s", iter, k);
      iter++;
    }

    sem_post(client);
  }
  fprintf(stdout, "\nBYTES RECEVIED: %d\n", i);

  close(fd);

  sem_close(server);
  sem_close(client);

  destroy_segment(argv[1]);
  return 0;
}

// anna_karenina.txt 1985015
// lorem_ipsum.txt 578
// dante.txt 222472