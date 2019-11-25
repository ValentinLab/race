#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ----------------------------------------
 * Général
 * ----------------------------------------
 */

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

int sum_1_to_n(int n) {
  if (n < 0) { // valeur absolue
    n = -n;
  }
  return (n * (n + 1)) / 2;
}

/*
 * ----------------------------------------
 * Struct player
 * ----------------------------------------
 */

void player_init(struct player *self, char *buf) {
  assert(self != NULL);

  // Position du joueur
  fgets(buf, BUFSIZE, stdin);
  self->pos_x = atoi(buf);
  fgets(buf, BUFSIZE, stdin);
  self->pos_y = atoi(buf);

  // Vitesse du joueur
  self->speed_x = 0;
  self->speed_y = 0;
}

void player_update_pos(struct player *self) {
  self->pos_x += self->speed_x;
  self->pos_y += self->speed_y;
} 

int player_dist(struct player *self, struct target *other, bool is_abscissa) {
  if(is_abscissa) {
    return other->x - self->pos_x;
  }
  return other->y - self->pos_y;
}

void player_reduce_speed_x(struct player *self) {
  // Si la vitesse est négative, on l'incrémente
  if (self->speed_x < 0) {
    self->speed_x += 1;
    return;
  }
  // Si la vitesse est positive, on la décrémente
  self->speed_x -= 1;
}

void player_reduce_speed_y(struct player *self) {
  // Si la vitesse est négative, on l'incrémente
  if (self->speed_y < 0) {
    self->speed_y += 1;
    return;
  }
  // Si la vitesse est positive, on la décrémente
  self->speed_y -= 1;
}

void player_increase_speed_x(struct player *self, struct target *other) {
  // Si l'objet est à gauche, on incrémente
  if (other->x < self->pos_x) {
    self->speed_x -= 1;
    return;
  }
  // Si l'objet est à droite on décrémente
  self->speed_x += 1;
}

void player_increase_speed_y(struct player *self, struct target *other) {
  // Si l'objet est à gauche, on incrémente
  if (other->y < self->pos_y) {
    self->speed_y -= 1;
    return;
  }
  // Si l'objet est à droite on décrémente
  self->speed_y += 1;
}

/*
 * ----------------------------------------
 * Struct target
 * ----------------------------------------
 */

void target_init(struct target *self, char *buf) {
  assert(self != NULL);

  // Abscisse de l'objectif
  fgets(buf, BUFSIZE, stdin);
  self->x = atoi(buf);
  // Ordonnée de l'objectif
  fgets(buf, BUFSIZE, stdin);
  self->y = atoi(buf);
  // Largeur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  self->w = atoi(buf);
  // Hauteur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  self->h = atoi(buf);
}

/*
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
*/