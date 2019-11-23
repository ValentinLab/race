#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 256

void print_grid(int size, int player_x, int player_y) {
  for (size_t i = 0; i < size; ++i) {
    fprintf(stderr, "|");
    for (size_t j = 0; j < size; ++j) {
      if (i == player_x && j == player_y) {
        fprintf(stderr, "x");
      } else {
        fprintf(stderr, " ");
      }
      fprintf(stderr, "|");
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "\n");
}

void get_obj_coords(int *x, int *y, int *w, int *h) {
  assert(x != NULL);
  assert(y != NULL);
  assert(w != NULL);
  assert(h != NULL);

  char buf[BUFSIZE];
  fgets(buf, BUFSIZE, stdin); // Abscisse de l'objectif
  (*x) = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Ordonnée de l'objectif
  (*y) = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Largeur de l'objectif
  (*w) = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Hauteur de l'objectif
  (*h) = atoi(buf);
}