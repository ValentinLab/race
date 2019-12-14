#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define IS_BETWEEN(x, alpha, omega) (((alpha) <= (x)) && ((x) <= (omega)))

static void accelerate_toward_target(struct player *self, struct target *target, bool *accelerated_x, bool *accelerated_y) {
  if (!will_player_touch_target_X_with_current_speed(self, target)) {
    if (is_on_target_X_if_brake_now(self, target)) {
      player_reduce_speed_x(self);
      *accelerated_x = false;
    } else {
      player_increase_speed_x(self, target);
      if (if_overshooting_target_X_if_brake_now(self, target)) {
        player_reduce_speed_x(self);
        *accelerated_x = false;
      } else {
        *accelerated_x = true;
      }
    }
  }

  if (!will_player_touch_target_Y_with_current_speed(self, target)) {
    if (is_on_target_Y_if_brake_now(self, target)) {
      player_reduce_speed_y(self);
      *accelerated_y = false;
    } else {
      player_increase_speed_y(self, target);
      if (if_overshooting_target_Y_if_brake_now(self, target)) {
        player_reduce_speed_y(self);
        *accelerated_y = false;
      } else {
        *accelerated_y = true;
      }
    }
  }
}

static void slow_down_to_avoid_borders(struct player *self, const int GRID_SIZE, bool accelerated_x, bool accelerated_y) {
  if ((self->speed_x < 0 && self->pos_x < 1 + sum_1_to_n(self->speed_x)) || (self->speed_x > 0 && GRID_SIZE - self->pos_x < 1 + sum_1_to_n(self->speed_x))) {
    if (accelerated_x) {
      player_reduce_speed_x(self);
    }
    player_reduce_speed_x(self);
  }

  if ((self->speed_y < 0 && self->pos_y < 1 + sum_1_to_n(self->speed_y)) || (self->speed_y > 0 && GRID_SIZE - self->pos_y < 1 + sum_1_to_n(self->speed_y))) {
    if (accelerated_y) {
      player_reduce_speed_y(self);
    }
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
  bool accelerated_x = false;
  bool accelerated_y = false;

  // Récupérer les informations sur l'objectif
  target_init(&target, buf);
  target_dump(&target);

  for (size_t round = 1;; ++round) {
    fprintf(stderr, "---[ Round #%zu\n", round);

    accelerate_toward_target(&trottinette, &target, &accelerated_x, &accelerated_y);
    slow_down_to_avoid_borders(&trottinette, SIZE, accelerated_x, accelerated_y);
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