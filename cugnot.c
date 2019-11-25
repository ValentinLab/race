#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/*static int update_speed(int player_axis_p, int v, int obj_axis_p) {
  int delta = axis_dist_2_obj(player_axis_p, obj_axis_p);

  if (delta == 0) {
    return 0;
  }
  if ((delta > 0 && delta < sum_1_to_n(v)) || (delta < 0 && -sum_1_to_n(v) < delta)) {
    return reduce_v(v);
  }
  if ((delta > 0 && sum_1_to_n(v + 1) <= delta) || (delta < 0 && delta <= -sum_1_to_n(v - 1))) {
    return increase_v(v, player_axis_p, obj_axis_p);
  }
  return v;
}*/

/*
 * Met à jour la vitesse du joueur
 * S'il est trop proche de l'objectif par rapport à sa vitesse (delta < v + v-1 + ... + 1), il ralentit
 * S'il est assez loin de l'objectif, il accélère
 * Sinon, il garde la même vitesse.
 */
static void update_speed(struct player *self, struct target *other) {
  int delta = player_dist(self, other, true);

  if (delta == 0) {
    self->speed_x = 0;
  } else if ((delta > 0 && delta < sum_1_to_n(self->speed_x)) || (delta < 0 && -sum_1_to_n(self->speed_x) < delta)) {
    player_reduce_speed_x(self);
  } else if ((delta > 0 && sum_1_to_n(self->speed_x + 1) <= delta) || (delta < 0 && delta <= -sum_1_to_n(self->speed_x - 1))) {
    player_increase_speed_x(self, other);
  }

  delta = player_dist(self, other, false);
  if (delta == 0) {
    self->speed_y = 0;
  } else if ((delta > 0 && delta < sum_1_to_n(self->speed_y)) || (delta < 0 && -sum_1_to_n(self->speed_y) < delta)) {
    player_reduce_speed_y(self);
  } else if ((delta > 0 && sum_1_to_n(self->speed_y + 1) <= delta) || (delta < 0 && delta <= -sum_1_to_n(self->speed_y - 1))) {
    player_increase_speed_y(self, other);
  }
}

int main() {
  // Structures
  struct player cugnot;
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
  player_init(&cugnot, buf);

  // Récupérer les informations sur l'objectif
  target_init(&target, buf);

  for (;;) {
    update_speed(&cugnot, &target);
    player_update_pos(&cugnot);

    // Envoyer les positions au serveur
    printf("%i\n%i\n", cugnot.pos_x, cugnot.pos_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      return 0;
    }
    if(strcmp(buf, "CHECKPOINT\n") == 0) {
      target_init(&target, buf);
      continue;
    }
  }

  free(grid);
  return 0;
}
