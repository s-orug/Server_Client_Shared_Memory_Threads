#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define BLOCK_SIZE 2000000
#define ARR_SIZE 1000000

#define ERROR (-1)
#define EOT (0x04)
#define ACK (0x06)

#define C_SEM "/client"
#define S_SEM "/server"

#define THREADS (4)

// 1024 2048 3072 4096 5120 6144 7168 8192
//                 ^                    ^
//-----------------|--------------------|
// file->key (ftok) create_key
// key->block (shmget) create_segment
// block->ptr (shmat) attach_segment

typedef struct send {
  char line[2048];
  char text[64];
} send;

bool file_checker(char filename[]) { // used to check if a file can be opened
  FILE *f;
  if (f = fopen(filename, "r")) {
    fclose(f);
    return true; // returns true if it can open
  }
  return false; // returns false if it can't open
}

key_t create_key(char filename[]) {
  key_t tmp = ftok(filename, 0);
  if (tmp < 0) {
    exit(ERROR);
  }
  return tmp;
}

int create_segment(char filename[]) {
  key_t key = create_key(filename);
  int shmid = shmget(key, BLOCK_SIZE, IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("create_segment");
    exit(ERROR);
  }
  return shmid;
}

char *attach_segment(char filename[]) {
  int shmid = create_segment(filename);
  if (shmat(shmid, NULL, 0) < 0) {
    perror("attach_segment\n");
    exit(ERROR);
  }
  return shmat(shmid, NULL, 0);
}

bool detach_segment(char filename[]) {
  char *shm = attach_segment(filename);
  return shmdt(shm) != ERROR;
}

bool destroy_segment(char filename[]) {
  int shmid = create_segment(filename);
  return (shmctl(shmid, IPC_RMID, NULL) != ERROR);
}

bool word_checker(char line[], char word[]) { return strstr(line, word); }

void logg(char text[]) { fprintf(stderr, "%s", text); }

void out(char text[]) { fprintf(stdout, "%s", text); }
