#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

static void accelerate_toward_target(struct player *self, struct target *target) {
  if (0 <= will_player_touch_target_with_current_speed(self, target)) {
    player_increase_speed(self, target);
    if (-1 == will_player_touch_target_with_current_speed(self, target)) { // 'with increased speed' en fait
      player_reduce_speed(self);
    }
  } else {

    if (is_overshooting_target_X_if_brake_now(self, target) || is_on_target_X_if_brake_now(self, target)) {
      player_reduce_speed_x(self);
    } else {
      player_increase_speed_x(self, target);
      if (is_overshooting_target_X_if_brake_now(self, target)) {
        player_reduce_speed_x(self);
      }
    }

    if (is_overshooting_target_Y_if_brake_now(self, target) || is_on_target_Y_if_brake_now(self, target)) {
      player_reduce_speed_y(self);
    } else {
      player_increase_speed_y(self, target);
      if (is_overshooting_target_Y_if_brake_now(self, target)) {
        player_reduce_speed_y(self);
      }
    }
  }
}

static void slow_down_to_avoid_borders(struct player *self, const int GRID_SIZE) {
  if ((self->speed_x < 0 && self->pos_x < 1 + sum_1_to_n(self->speed_x)) || (self->speed_x > 0 && GRID_SIZE - self->pos_x < 1 + sum_1_to_n(self->speed_x))) {
    player_reduce_speed_x(self); // la sous-fonction vérifie que l'on ne ralentit pas 2 fois
    player_reduce_speed_x(self);
  }

  if ((self->speed_y < 0 && self->pos_y < 1 + sum_1_to_n(self->speed_y)) || (self->speed_y > 0 && GRID_SIZE - self->pos_y < 1 + sum_1_to_n(self->speed_y))) {
    player_reduce_speed_y(self); // la sous-fonction vérifie que l'on ne ralentit pas 2 fois
    player_reduce_speed_y(self);
  }
}

int main() {
  // Structures
  struct player trottinette;
  struct target target;

  // Création du buffer
  setbuf(stdout, NULL);
  char buf[BUFSIZE];

  // Récupérer la taille de la grille
  fgets(buf, BUFSIZE, stdin);
  const int SIZE = atoi(buf);
  // Grille en row major
  int *grid = calloc(SIZE * SIZE, sizeof(int));
  // Récupérer les valeurs sur la grille
  for (int i = 0; i < SIZE * SIZE; ++i) {
    fgets(buf, BUFSIZE, stdin);
    grid[i] = atoi(buf);
  }

  // Récupérer la position initiale du joueur
  player_init(&trottinette, buf);

  // Récupérer les informations sur l'objectif
  target_init(&target, buf);
  target_dump(&target);

  for (size_t round = 1;; ++round) {
    fprintf(stderr, "---[ Round #%zu\n", round);

    accelerate_toward_target(&trottinette, &target);
    slow_down_to_avoid_borders(&trottinette, SIZE);
    player_update_pos(&trottinette);

    // Envoyer les positions au serveur
    printf("%i\n%i\n", trottinette.pos_x, trottinette.pos_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      return 0;
    }
    if (strcmp(buf, "CHECKPOINT\n") == 0) {
      target_init(&target, buf);
      target_dump(&target);
      continue;
    }
  }

  free(grid);
  return 0;
}