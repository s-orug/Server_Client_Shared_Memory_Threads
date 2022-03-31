#include "text-memory.h"

int main() {
  // char *k = attach_segment(create_segment(create_key("dat/dante.txt")));
  // printf("%s", k);
  // char EOT=0x04;
  // printf("%c\n", EOT);
  char word[] = "the journey";
  char filename[] = "dat/dante.txt";
  // bool c = word_checker(line, word);
  // printf("%s\n", c ? "true" : "false");

  FILE *fp;
  char command[2048], line[2048], *store = "";
  sprintf(command, "cat  %s ", filename);
  if (file_checker(filename) != 0) {
    fp = popen(command, "r");

    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
      if (word_checker(line, word))
        printf("%s", line);
    }
    pclose(fp);
    return 0;
  }
}