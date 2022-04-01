#include "text-memory.h"

void *thread(void *arg) {

  struct send *iptr = (struct send *)arg;
  int i = 0;
  if (word_checker(iptr->line, iptr->text)) {
    fprintf(stdout, "%d\t%s", iptr->iter, iptr->line);
    return (void *)true;
  }
  return (void *)false;
}

int main(int argc, char *argv[]) {

  char *myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);

  sem_t *server = sem_open(S_SEM, 0);
  sem_t *client = sem_open(C_SEM, 1);

  int fd = open(myfifo, O_WRONLY);
  write(fd, argv[1], strlen(argv[1]));
  char *k = attach_segment(argv[1]);

  int i = 0, iter = 1;
  pthread_t threads[4];
  while (true) {
    for (int j = 0; j < 4; j++) {
      sem_wait(server);

      void *ret_val;
      if (strlen(k) > 0) {
        if (k[0] == EOT) {
          fprintf(stdout, "\nBYTES RECEVIED: %d\n", i);
          break;
        }
        send vals;
        strncpy(vals.line, k, sizeof(vals.line));
        strncpy(vals.text, argv[2], sizeof(vals.text));
        vals.iter = iter;
        vals.i = i;
        if (pthread_create(&threads[i], NULL, thread, &vals) != 0) {
          perror("thread failed to create\n");
        }
        if (pthread_join(threads[i], &ret_val) != 0) {
          perror("failed to join\n");
        }
        if ((bool)ret_val == true) {
          iter++;
          i += strlen(k);
        }
      }

      sem_post(client);
    }
    if (k[0] == EOT) {
      break;
    }
  }
  detach_segment(argv[1]);

  close(fd);

  sem_unlink(S_SEM);
  sem_unlink(C_SEM);

  sem_close(server);
  sem_close(client);

  return 0;
}
