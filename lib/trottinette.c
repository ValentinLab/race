#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define IS_BETWEEN(x, alpha, omega)  (((alpha) <= (x)) && ((x) <= (omega)))

static void accelerate_toward_target(struct player *self, struct target *target, bool *accelerated_x, bool *accelerated_y) {
  int brake_dist_x = sum_1_to_n(self->speed_x);
  int x_if_we_brake_now = self->pos_x + brake_dist_x;
  if (self->speed_x < 0) {
    x_if_we_brake_now = self->pos_x - brake_dist_x;
  }
  if (IS_BETWEEN(x_if_we_brake_now, target->x, target->xright)) {
    player_reduce_speed_x(self);
  } else {
    player_increase_speed_x(self, target);
    *accelerated_x = true;
  }
  
  int brake_dist_y = sum_1_to_n(self->speed_y);
  int y_if_we_brake_now = self->pos_y + brake_dist_y;
  if (self->speed_y < 0) {
    y_if_we_brake_now = self->pos_y - brake_dist_y;
  }
  if (IS_BETWEEN(y_if_we_brake_now, target->y, target->ybottom)) {
    player_reduce_speed_y(self);
  } else {
    player_increase_speed_y(self, target);
    *accelerated_y = true;
  }

  target_dump(target);
  fprintf(stderr, "Arrivée du joueur sur %i %i\n", x_if_we_brake_now, y_if_we_brake_now);

}

static void slow_down_to_avoid_borders(struct player *self, const int GRID_SIZE, bool accelerated_x, bool accelerated_y) {
  if ((self->speed_x < 0 && self->pos_x < 1 + sum_1_to_n(self->speed_x)) || (self->speed_x > 0 && GRID_SIZE - self->pos_x < 1 + sum_1_to_n(self->speed_x))) {
    if(accelerated_x) {
      player_reduce_speed_x(self);
    }
    player_reduce_speed_x(self);
  }

  if ((self->speed_y < 0 && self->pos_y < 1 + sum_1_to_n(self->speed_y)) || (self->speed_y > 0 && GRID_SIZE - self->pos_y < 1 + sum_1_to_n(self->speed_y))) {
    if(accelerated_y) {
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