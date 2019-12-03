#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/**
 * Modifie la case cible vers la case à la valeur la plus petite dans la cible 
 */
static void target_optimise(struct target *self, const int *ground, const size_t SIZE, const struct player *player) {
  int actual_target_value = 0x7FFFFFFF;
  size_t max_obj_x = self->x + self->w - 1;
  size_t max_obj_y = self->y + self->h - 1;

  size_t iter_x_min = 0;
  size_t iter_x_max = SIZE;
  size_t iter_y_min = 0;
  size_t iter_y_max = SIZE;

  if (self->x <= player->pos_x && player->pos_x <= max_obj_x && player->pos_y < self->y) { // Partie supérieure
    iter_x_min = self->x;
    iter_x_max = max_obj_x;
    iter_y_min = self->y;
    iter_y_max = self->y;

  } else if (self->x <= player->pos_x && player->pos_x <= max_obj_x && max_obj_y < player->pos_y) { // Partie inférieure
    iter_x_min = self->x;
    iter_x_max = max_obj_x;
    iter_y_min = max_obj_y;
    iter_y_max = max_obj_y;

  } else if (self->y <= player->pos_y && player->pos_y <= max_obj_y && player->pos_x < self->x) { // Partie gauche
    iter_x_min = self->x;
    iter_x_max = self->x;
    iter_y_min = self->y;
    iter_y_max = max_obj_y;

  } else if (self->y <= player->pos_y && player->pos_y <= max_obj_y && max_obj_x < player->pos_x) { // Partie droite
    iter_x_min = max_obj_x;
    iter_x_max = max_obj_x;
    iter_y_min = max_obj_y;
    iter_y_max = max_obj_y;

  } else {
    iter_x_min = self->x;
    iter_x_max = max_obj_x;
    iter_y_min = self->y;
    iter_y_max = max_obj_y;
  }

  for (size_t y = iter_y_min; y <= iter_y_max; ++y) {
    for (size_t x = iter_x_min; x <= iter_x_max; ++x) {
      if (ground[y * SIZE + x] < actual_target_value) {
        self->x = x;
        self->y = y;
        actual_target_value = ground[y * SIZE + x];
      }
    }
  }
}

int main() {
  // Structures
  struct player multipla;
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
  player_init(&multipla, buf);

  // Récupérer les informations sur l'objectif
  target_init(&target, buf);
  target_optimise(&target, grid, SIZE, &multipla);
  fprintf(stderr, "Objectif optimisé : %i %i \n", target.x, target.y);

  for (size_t round = 1;; ++round) {
    fprintf(stderr, "---[ Round #%zu\n", round);

    update_speed(&multipla, &target);
    player_update_pos(&multipla);

    // Envoyer les positions au serveur
    printf("%i\n%i\n", multipla.pos_x, multipla.pos_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      return 0;
    }
    if (strcmp(buf, "CHECKPOINT\n") == 0) {
      target_init(&target, buf);
      target_optimise(&target, grid, SIZE, &multipla);
      fprintf(stderr, "Nouvel objectif optimisé : %i %i \n", target.x, target.y);
      continue;
    }
  }

  free(grid);
  return 0;
}
