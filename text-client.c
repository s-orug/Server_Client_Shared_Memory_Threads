#include "text-memory.h"
pthread_mutex_t mutex;

void *thread(void *arg) {

  struct send *iptr = (struct send *)arg;

  pthread_mutex_lock(&mutex);
  if (word_checker(iptr->line, iptr->text)) {
    pthread_mutex_unlock(&mutex);
    return (void *)true;
  }
  pthread_mutex_unlock(&mutex);
  return (void *)false;
}

int main(int argc, char *argv[]) {

  char *myfifo = "/tmp/myfifo", filename[64];
  mkfifo(myfifo, 0666);

  strcpy(filename, argv[1]);
  int fd = open(myfifo, O_WRONLY);
  write(fd, filename, sizeof(filename));

  if (file_checker(argv[1]) == false) {
    fprintf(stderr, "INVALID FILE\n");
    exit(ERROR);
  }

  if (strlen(argv[2]) == 0) {
    fprintf(stderr, "INVALID TEXT\n");
    exit(ERROR);
  }

  char *memory = attach_segment(filename);
  memset(memory, '\0', BLOCK_SIZE);

  while (memory[sizeof(memory)] != EOT) {
    /* Waits */
  }

  close(fd);

  char *memory_array[ARR_SIZE] = {NULL};
  size_t n = 0;

  for (char *p = strtok(memory, "\n"); n < ARR_SIZE && p;
       p = strtok(NULL, "\n")) {
    memory_array[n++] = p;
  }

  pthread_t threads[THREADS];
  pthread_mutex_init(&mutex, NULL);
  void *ret_val;

  int iter = 1, l = 0;
  for (int i = 0; i < n; i++) {
    struct send vals;
    strcpy(vals.line, memory_array[i]);
    strcpy(vals.text, argv[2]);

    if (pthread_create(&threads[i % THREADS], NULL, thread, &vals) != 0) {
      perror("thread failed to create\n");
    }
    if (pthread_join(threads[i % THREADS], &ret_val) != 0) {
      perror("failed to join\n");
    }
    if ((bool)ret_val == true) {
      l += strlen(vals.line) + 1;
      fprintf(stdout, "%d\t%s\n", iter, vals.line);
      iter++;
    }
  }
  fprintf(stderr, "\nBYTES RECEIVED: %d \n", l);

  pthread_mutex_destroy(&mutex);
  destroy_segment(filename);

  return 0;
}