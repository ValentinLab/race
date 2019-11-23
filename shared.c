#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
  // Abscisse de l'objectif
  fgets(buf, BUFSIZE, stdin);
  *x = atoi(buf);
  // Ordonnée de l'objectif
  fgets(buf, BUFSIZE, stdin);
  *y = atoi(buf);
  // Largeur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  *w = atoi(buf);
  // Hauteur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  *h = atoi(buf);
}

int axis_dist_2_obj(int player_position, int obj_position) {
  return obj_position - player_position;
}

int reduce_v(int v) {
  // Si la vitesse est négative, on l'incrémente
  if (v < 0) {
    return ++v;
  }
  // Si la vitesse est positive, on la décrémente
  return --v;
}

int increase_v(int v, int player_position, int obj_position) {
  // Si l'objet est à gauche, on incrémente
  if (obj_position < player_position) {
    return --v;
  }
  // Si l'objet est à droite on décrémente
  return ++v;
}

int sum_1_to_n(int n) {
  if (n < 0) { // valeur absolue
    n = -n;
  }
  return (n * (n + 1)) / 2;
}