#include "text-memory.h"

int main() {

  int k = 0;
  while (1) {
    if (k == 0)
      fprintf(stdout, "SERVER STARTED\n");
    k = 1;

// *************** NAMED PIPE W/READ_ONLY ****************** //

    char filename[64];
    char *myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);
    int fd = open(myfifo, O_RDONLY);

// *** CLEARING THE BUFFER BEFORE READING THE FILE NAME *** //

    memset(filename, '\0', sizeof(filename));

// **** READS THE FILENAME AND LOGS THE CLIENT REQUEST **** //

    if (read(fd, filename, sizeof(filename))) {
      fprintf(stderr, "CLIENT REQUEST RECEIVED");
      
// ************ CHECKING IF THE FILE IS VALID ************** //

      if (file_checker(filename) == false) {
        fprintf(stderr, "\tINVALID FILE\n");
        continue;
      }

// * IF THE FILE IS INVALID CONTINUES TO THE NEXT ITERATION * //

// *** ATTACHES TO THE SHARED MEMORY AND LOGS MEMORY OPEN *** //

      char *memory = attach_segment(filename);
      fprintf(stderr, "\tMEMORY OPEN");
      memset(memory, '\0', BLOCK_SIZE);

// ** OPENS THE FILE, LOGS THE STEP, AND WRITES THE CONTENT ** //
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

// ***** AFTER WRITING TO SHARED MEMORY EOT CHAR IS ADDED ***** //
      memory[sizeof(memory)] = EOT;
      fclose(fp);

// ******* HE FILE IS CLOSED AND THE ACTION IS LOGGED ********* //

      fprintf(stderr, "\tFILE CLOSED");

// **************** MEMORY SEGMENT IS DETACHED **************** //
      detach_segment(filename);
      fprintf(stderr, "\tMEMORY CLOSED\n");
    }
    close(fd);
  }

  return 0;
}