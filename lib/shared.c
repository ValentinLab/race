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

void print_grid(const int *grid, const int SIZE) {
  for (size_t y = 0; y < SIZE; ++y) {
    fprintf(stderr, "|");
    for (size_t x = 0; x < SIZE; ++x) {
      fprintf(stderr, "%i", grid[y * SIZE + x]);
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

  self->speed_x = 0;
  self->speed_y = 0;
}

void player_copy(struct player *player, struct player *new) {
  new->pos_x = player->pos_x;
  new->pos_x = player->pos_y;
  new->speed_x = player->speed_x;
  new->speed_y = player->speed_y;
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
  if (self->speed_x != 0) {
    // Si la vitesse est négative, on l'incrémente
    if (self->speed_x < 0) {
      self->speed_x += 1;
      return;
    }
    // Si la vitesse est positive, on la décrémente
    self->speed_x -= 1;
  }
}

void player_reduce_speed_y(struct player *self) {
  if (self->speed_y != 0) {
    // Si la vitesse est négative, on l'incrémente
    if (self->speed_y < 0) {
      self->speed_y += 1;
      return;
    }
    // Si la vitesse est positive, on la décrémente
    self->speed_y -= 1;
  }
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
    player_reduce_speed_x(self);
  } else if ((delta > 0 && delta < sum_1_to_n(self->speed_x)) || (delta < 0 && -sum_1_to_n(self->speed_x) < delta)) {
    player_reduce_speed_x(self);
  } else if ((delta > 0 && sum_1_to_n(self->speed_x + 1) <= delta) || (delta < 0 && delta <= -sum_1_to_n(self->speed_x - 1))) {
    player_increase_speed_x(self, target);
  }

  delta = player_dist(self, target, false); // deltaY
  fprintf(stderr, "DeltaY : %i\n", delta);
  if (delta == 0) {
    player_reduce_speed_y(self);
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

  fprintf(stderr, "Buffer 0 : %s\n", buf);
  // Abscisse de l'objectif
  fgets(buf, BUFSIZE, stdin);
  fprintf(stderr, "Buffer 1 : %s\n", buf);
  self->x = atoi(buf);
  // Ordonnée de l'objectif
  fgets(buf, BUFSIZE, stdin);
  fprintf(stderr, "Buffer 2 : %s\n", buf);
  self->y = atoi(buf);
  // Largeur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  fprintf(stderr, "Buffer 3 : %s\n", buf);
  self->w = atoi(buf);
  // Hauteur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  fprintf(stderr, "Buffer 4 : %s\n", buf);

  self->h = atoi(buf);
}

void target_dump(const struct target *self) {
  fprintf(stderr, "Cible : %i %i (%ix%i)\n", self->x, self->y, self->w, self->h);
}

void target_copy(const struct target *self, struct target *copy) {
  assert(self != NULL);
  assert(copy != NULL);

  copy->x = self->x;
  copy->y = self->y;
  copy->w = self->w;
  copy->h = self->h;
  copy->value = self->value;
}

bool target_is_player_on(const struct target *self, const struct player *player) {
  return (self->x <= player->pos_x && player->pos_x <= self->x + self->w && self->y <= player->pos_y && player->pos_y <= self->y + self->h);
}