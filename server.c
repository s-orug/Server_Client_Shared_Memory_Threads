#include <text-memory.h>

#define BLOCK_SIZE 4096

int main() {
  int fd;
  char file_name[64];
  char *myfifo = "/tmp/myfifo"; // named pipe path

  mkfifo(myfifo, 0666); // creating named pipe with 0666 permission

  while (1) {
    fd = open(myfifo, O_RDONLY); // opens the pipe read only

    memset(file_name, '\0', sizeof(file_name));
    // clears the buffer every time.

    if (read(fd, file_name, sizeof(file_name)) > 0) {
      int shmid;
      char *shm, *s;

      close(fd);
      printf("FILE: %s\n", file_name);

      shmid = shmget(1001001, BLOCK_SIZE, 0666);
      shm = shmat(shmid, NULL, 0);
      printf("string is: %s", shm);
      shmctl(shmid, IPC_STAT, NULL);
    }
  }
  return 0;
}