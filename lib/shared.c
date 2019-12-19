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

int absol(int n) {
  if (n < 0) {
    return -n;
  }
  return n;
}

int sum_1_to_n(int n) {
  n = absol(n);
  return (n * (n + 1)) / 2;
}

int coord_if_we_break_now(int pos, int speed) {
  if (speed > 0) {
    return pos + sum_1_to_n(speed);
  }

  return pos - sum_1_to_n(speed);
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
  self->has_accelerated_x = 0;
  self->has_accelerated_y = 0;
}

void player_dump(const struct player *self) {
  assert(self != NULL);
  fprintf(stderr, "Joueur : pos(%i, %i) v = (%i, %i) \n", self->pos_x, self->pos_y, self->speed_x, self->speed_y);
}

void player_copy(struct player *self, struct player *copy) {
  copy->pos_x = self->pos_x;
  copy->pos_y = self->pos_y;
  copy->speed_x = self->speed_x;
  copy->speed_y = self->speed_y;
  copy->has_accelerated_x = self->has_accelerated_x;
  copy->has_accelerated_y = self->has_accelerated_y;
}

void player_update_pos(struct player *self) {
  self->pos_x += self->speed_x;
  self->pos_y += self->speed_y;
  self->has_accelerated_x = 0;
  self->has_accelerated_y = 0;
}

int player_dist(const struct player *self, const struct target *target, bool is_abscissa) {
  if (is_abscissa) {
    return target->x - self->pos_x;
  }
  return target->y - self->pos_y;
}

void player_reduce_speed_x(struct player *self) {
  if (self->has_accelerated_y >= 0 && self->speed_x != 0) {
    self->has_accelerated_x -= 1;
    // Si la vitesse est négative, on l'incrémente
    if (self->speed_x < 0) {
      self->speed_x += 1;
    } else {
      // Si la vitesse est positive, on la décrémente
      self->speed_x -= 1;
    }
  }
}

void player_reduce_speed_y(struct player *self) {
  if (self->has_accelerated_y >= 0 && self->speed_y != 0) {
    self->has_accelerated_y -= 1;
    // Si la vitesse est négative, on l'incrémente
    if (self->speed_y < 0) {
      self->speed_y += 1;
    } else {
      // Si la vitesse est positive, on la décrémente
      self->speed_y -= 1;
    }
  }
}

void player_reduce_speed(struct player *self) {
  player_reduce_speed_x(self);
  player_reduce_speed_y(self);
}

void player_increase_speed_x(struct player *self, struct target *target) {
  if (self->has_accelerated_x <= 0) {
    self->has_accelerated_x += 1;
    // Si l'objet est à gauche, on incrémente
    if (target->x < self->pos_x) {
      self->speed_x -= 1;
    } else {
      // Si l'objet est à droite on décrémente
      self->speed_x += 1;
    }
  }
}

void player_increase_speed_y(struct player *self, struct target *target) {
  if (self->has_accelerated_y <= 0) {
    self->has_accelerated_y += 1;
    // Si l'objet est à gauche, on incrémente
    if (target->y < self->pos_y) {
      self->speed_y -= 1;
    } else {
      // Si l'objet est à droite on décrémente
      self->speed_y += 1;
    }
  }
}

void player_increase_speed(struct player *self, struct target *target) {
  player_increase_speed_x(self, target);
  player_increase_speed_y(self, target);
}

bool player_is_on_target(const struct player *self, const struct target *target) {
  return (target->x <= self->pos_x && self->pos_x <= target->xright && target->y <= self->pos_y && self->pos_y <= target->ybottom);
}

bool is_on_target_X_if_brake_now(const struct player *self, const struct target *target) {
  int x_if_we_brake_now = coord_if_we_break_now(self->pos_x, self->speed_x);
  return (target->x <= x_if_we_brake_now && x_if_we_brake_now <= target->xright);
}

bool is_on_target_Y_if_brake_now(const struct player *self, const struct target *target) {
  int y_if_we_brake_now = coord_if_we_break_now(self->pos_y, self->speed_y);
  return (target->y <= y_if_we_brake_now && y_if_we_brake_now <= target->ybottom);
}

bool is_overshooting_target_X_if_brake_now(const struct player *self, const struct target *target) {
  int x_if_we_brake_now = coord_if_we_break_now(self->pos_x, self->speed_x);
  return ((self->pos_x < target->x && target->xright < x_if_we_brake_now) || (target->xright < self->pos_x && x_if_we_brake_now < target->x));
}

bool is_overshooting_target_Y_if_brake_now(const struct player *self, const struct target *target) {
  int y_if_we_brake_now = coord_if_we_break_now(self->pos_y, self->speed_y);
  return ((self->pos_y < target->y && target->ybottom < y_if_we_brake_now) || (target->ybottom < self->pos_y && y_if_we_brake_now < target->y));
}

bool is_overshooting_target_if_brake_now(const struct player *self, const struct target *target) {
  return is_overshooting_target_X_if_brake_now(self, target) || is_overshooting_target_Y_if_brake_now(self, target);
}

int will_player_touch_target_X_with_current_speed(const struct player *self, const struct target *target) {
  if (player_is_on_target(self, target)) {
    return 0;
  }

  if (self->speed_x == 0) {
    return -1;
  } // Vitesse nulle et on est pas sur la cible

  if ((self->speed_x < 0 && self->pos_x < target->x) || (self->speed_x > 0 && target->xright < self->pos_x)) {
    return -1; // On va pas dans la bonne direction
  }

  int delta = player_dist(self, target, true); // deltaX
  if (delta % self->speed_x == 0) {
    return delta / absol(self->speed_x);
  }

  return -1;
}

int will_player_touch_target_Y_with_current_speed(const struct player *self, const struct target *target) {
  if (player_is_on_target(self, target)) {
    return 0;
  }

  if (self->speed_y == 0) {
    return -1;
  } // Vitesse nulle et on est pas sur la cible

  if ((self->speed_y < 0 && self->pos_y < target->y) || (self->speed_y > 0 && target->ybottom < self->pos_y)) {
    return -1; // On va pas dans la bonne direction
  }

  int delta = player_dist(self, target, true); // deltaY
  if (delta % self->speed_y == 0) {
    return delta / absol(self->speed_y);
  }

  return -1;
}

int will_player_touch_target_with_current_speed(const struct player *self, const struct target *target) {
  int iteration_needed_X = will_player_touch_target_X_with_current_speed(self, target);
  int iteration_needed_Y = will_player_touch_target_Y_with_current_speed(self, target);
  if (iteration_needed_X == iteration_needed_Y) {
    return iteration_needed_X;
  }

  return -1;
}

void player_slow_down_to_avoid_borders(struct player *self, const int GRID_SIZE) {
  if ((self->speed_x < 0 && self->pos_x < sum_1_to_n(self->speed_x)) || (self->speed_x > 0 && GRID_SIZE - self->pos_x < 1 + sum_1_to_n(self->speed_x))) {
    player_reduce_speed_x(self); // la sous-fonction vérifie que l'on ne ralentit pas 2 fois
    player_reduce_speed_x(self);
  }

  if ((self->speed_y < 0 && self->pos_y < sum_1_to_n(self->speed_y)) || (self->speed_y > 0 && GRID_SIZE - self->pos_y < 1 + sum_1_to_n(self->speed_y))) {
    player_reduce_speed_y(self); // la sous-fonction vérifie que l'on ne ralentit pas 2 fois
    player_reduce_speed_y(self);
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
  int w = atoi(buf);
  // Hauteur de l'objectif
  fgets(buf, BUFSIZE, stdin);
  int h = atoi(buf);

  self->xright = self->x + w - 1;
  self->ybottom = self->y + h - 1;
  self->value = 0x7FFFFFFF; // On initialise au maximum des entier pour la comparaison plus tard
}

void target_dump(const struct target *self) {
  fprintf(stderr, "Cible : %i %i to %i %i - value %i\n", self->x, self->y, self->xright, self->ybottom, self->value);
}

void target_copy(const struct target *self, struct target *copy) {
  assert(self != NULL);
  assert(copy != NULL);

  copy->x = self->x;
  copy->y = self->y;
  copy->xright = self->xright;
  copy->ybottom = self->ybottom;
  copy->value = 0x7FFFFFFF;
}