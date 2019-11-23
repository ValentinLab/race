#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 256
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

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

int axis_dist_2_obj(int player_position, int obj_position) {
  return obj_position - player_position;
}

int reduce_v(int v) {
  if (v < 0) {
    return ++v; // Si la vitesse est négative, on l'incrémente
  }
  return --v; // Si la vitesse est positive, on la décrémente
}

int increase_v(int v, int player_position, int obj_position) {
  if (obj_position < player_position) {
    return --v;
  }
  return ++v;
}

int sum_1_to_n(int n) {
  if (n < 0) {
    n = -n;
  }
  return (n * (n + 1)) / 2;
}