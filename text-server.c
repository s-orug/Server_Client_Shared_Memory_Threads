#include "text-memory.h"

int main() {

  fprintf(stdout, "SERVER STARTED");

  while (1) {
    char filename[64];
    char *myfifo = "/tmp/myfifo"; // named pipe path

    mkfifo(myfifo, 0666);            // creating named pipe with 0666 permission
    int fd = open(myfifo, O_RDONLY); // opens the pipe read only

    memset(filename, '\0', sizeof(filename));

    if (read(fd, filename, sizeof(filename))) {
      fprintf(stderr, "CLIENT REQUEST RECEIVED");

      if (file_checker(filename) == false) {
        fprintf(stderr, "\tINVALID FILE\n");
        continue;
      }

      char *memory = attach_segment(filename);
      fprintf(stderr, "\tMEMORY OPEN");
      memset(memory, '\0', BLOCK_SIZE);

      FILE *fp;
      fp = fopen(filename, "r");
      fprintf(stderr, "\tOPENING: %s", filename);

      if (fp != NULL) {
        size_t length = fread(memory, sizeof(char), BLOCK_SIZE, fp);
        if (ferror(fp) != 0) {
          perror("Error in reading");
        } else {
          memory[length++] = '\0';
        }
      }
      memory[sizeof(memory)] = EOT;
      fclose(fp);

      fprintf(stderr, "\tFILE CLOSED");
      detach_segment(filename);
      fprintf(stderr, "\tMEMORY CLOSED\n");
    }
    close(fd);
  }

  return 0;
}