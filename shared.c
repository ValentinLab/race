#include "shared.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

int player_dist(struct player *self, struct target *target, bool is_abscissa) {
  if (is_abscissa) {
    return target->x - self->pos_x;
  }
  return target->y - self->pos_y;
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

void player_increase_speed_x(struct player *self, struct target *target) {
  // Si l'objet est à gauche, on incrémente
  if (target->x < self->pos_x) {
    self->speed_x -= 1;
    return;
  }
  // Si l'objet est à droite on décrémente
  self->speed_x += 1;
}

void player_increase_speed_y(struct player *self, struct target *target) {
  // Si l'objet est à gauche, on incrémente
  if (target->y < self->pos_y) {
    self->speed_y -= 1;
    return;
  }
  // Si l'objet est à droite on décrémente
  self->speed_y += 1;
}

void update_speed(struct player *self, struct target *target) {
  int delta = player_dist(self, target, true); // deltaX
  fprintf(stderr, "DeltaX : %i\n", delta);
  if (delta == 0) {
    self->speed_x = 0;
  } else if ((delta > 0 && delta < sum_1_to_n(self->speed_x)) || (delta < 0 && -sum_1_to_n(self->speed_x) < delta)) {
    player_reduce_speed_x(self);
  } else if ((delta > 0 && sum_1_to_n(self->speed_x + 1) <= delta) || (delta < 0 && delta <= -sum_1_to_n(self->speed_x - 1))) {
    player_increase_speed_x(self, target);
  }

  delta = player_dist(self, target, false); // deltaY
  fprintf(stderr, "DeltaY : %i\n", delta);
  if (delta == 0) {
    self->speed_y = 0;
  } else if ((delta > 0 && delta < sum_1_to_n(self->speed_y)) || (delta < 0 && -sum_1_to_n(self->speed_y) < delta)) {
    player_reduce_speed_y(self);
  } else if ((delta > 0 && sum_1_to_n(self->speed_y + 1) <= delta) || (delta < 0 && delta <= -sum_1_to_n(self->speed_y - 1))) {
    player_increase_speed_y(self, target);
  }
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