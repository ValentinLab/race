#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/**
 * Modifie la case cible vers la case à la valeur la plus petite dans la cible 
 */
static void target_optimise(struct target *self, const int *ground, const size_t SIZE) {
  int actual_target_value = ground[self->y * SIZE + self->x];
  size_t max_obj_x = self->x + self->w;
  size_t max_obj_y = self->y + self->h;  

  for (size_t y = self->y; y < max_obj_y; ++y) {

    for (size_t x = self->x; x < max_obj_x; ++x) {

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
  target_optimise(&target, grid, SIZE);
  fprintf(stderr, "Objectif optimisé : %i %i \n", target.x, target.y);

  for (size_t round = 1;; ++round) {
    fprintf(stderr, "---[ Round #%zu\n", round);

    update_speed(&cugnot, &target);
    player_update_pos(&cugnot);

    // Envoyer les positions au serveur
    printf("%i\n%i\n", cugnot.pos_x, cugnot.pos_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      free(grid);
      return 0;
    }
    if (strcmp(buf, "CHECKPOINT\n") == 0) {
      target_init(&target, buf);
      target_optimise(&target, grid, SIZE);
      fprintf(stderr, "Nouvel objectif optimisé : %i %i \n", target.x, target.y);
      continue;
    }
  }

  free(grid);
  return 0;
}
