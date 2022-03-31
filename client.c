// C program to implement one side of FIFO
// This side writes first, then reads
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BLOCK_SIZE 4096
#define ERROR (-1)



int main(int argc, char *argv[]) {
  int fd, shmid;
  char *shm, *s, ok[200];

  char *myfifo = "/tmp/myfifo";

  mkfifo(myfifo, 0666);

  char arr1[64];
  fd = open(myfifo, O_WRONLY);
  write(fd, argv[1], strlen(argv[1]));
  close(fd);

  shmid = shmget(1001001, BLOCK_SIZE, IPC_CREAT | 0666);
  shm = shmat(shmid, NULL, 0);

  strncpy(shm,"this works", 11);

  shmctl(shmid, IPC_STAT, NULL);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}