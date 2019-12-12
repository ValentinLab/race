#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define PIPE_WRITE 0
#define PIPE_READ 1
#define BUFSIZE 30

struct player_result {
  char *name;
  size_t number_of_victory;
  size_t total_moves;
  size_t total_score;
};

static void print_help() {
  printf("stats-for-race\n");
  printf("\n  Usage : \n");
  printf("    stats-for-race [(-i|--iteration) <number>] <race-server> <players>\n");
  printf("    stats-for-race -h\n");

  printf("\n  Options : \n");
}

int main(int argc, char **argv) {
  int iteration_number = 20; // by default

  extern char *optarg;
  int c;
  while ((c = getopt(argc, argv, "hi:")) != -1) {
    switch (c) {
    case 'h':
      print_help();
      return 0;

    case 'i':
      iteration_number = atoi(optarg);
      continue;

    default:
      return 1;
    }
  }

  printf("Statistiques pour Race \n");
  printf("Nombre de tests : %i \n", iteration_number);

  return 0;
}
