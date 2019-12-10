#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

static void target_optimise_line(struct target *self, const int *ground, const size_t SIZE, const size_t iter_x_min, const size_t iter_x_max, const size_t iter_y_min, const size_t iter_y_max) {
  for (size_t y = iter_y_min; y <= iter_y_max; ++y) {
    for (size_t x = iter_x_min; x <= iter_x_max; ++x) {
      if (ground[y * SIZE + x] < self->value) {
        self->x = x;
        self->y = y;
        self->value = ground[y * SIZE + x];
      }
    }
  }

  self->w = 1;
  self->h = 1;
}

static void target_top(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x;
  size_t iter_x_max = initial_target->x + initial_target->w - 1;
  size_t iter_y_min = initial_target->y;
  size_t iter_y_max = initial_target->y;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

static void target_bottom(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x;
  size_t iter_x_max = initial_target->x + initial_target->w - 1;
  size_t iter_y_min = initial_target->y + initial_target->h - 1;
  size_t iter_y_max = initial_target->y + initial_target->h - 1;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

static void target_left(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x;
  size_t iter_x_max = initial_target->x;
  size_t iter_y_min = initial_target->y;
  size_t iter_y_max = initial_target->y + initial_target->h - 1;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

static void target_right(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x + initial_target->w - 1;
  size_t iter_x_max = initial_target->x + initial_target->w - 1;
  size_t iter_y_min = initial_target->y;
  size_t iter_y_max = initial_target->y + initial_target->h - 1;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

/**
 * Modifie la case cible vers la case à la valeur la plus petite dans la cible 
 */
static void target_optimise(struct target *self, const int *ground, const size_t SIZE, const struct player *player) {
  self->value = 0x7FFFFFFF;
  struct target initial_target;
  target_copy(self, &initial_target);

  size_t max_obj_x = self->x + self->w - 1;
  size_t max_obj_y = self->y + self->h - 1;

  if (self->x <= player->pos_x && player->pos_x <= max_obj_x && player->pos_y < self->y) { // Partie supérieure
    target_top(self, &initial_target, ground, SIZE, player);
    return;
  }

  if (self->x <= player->pos_x && player->pos_x <= max_obj_x && max_obj_y < player->pos_y) { // Partie inférieure
    target_bottom(self, &initial_target, ground, SIZE, player);
    return;
  }

  if (self->y <= player->pos_y && player->pos_y <= max_obj_y && player->pos_x < self->x) { // Partie gauche
    target_left(self, &initial_target, ground, SIZE, player);
    return;
  }

  if (self->y <= player->pos_y && player->pos_y <= max_obj_y && max_obj_x < player->pos_x) { // Partie droite
    target_right(self, &initial_target, ground, SIZE, player);
    return;
  }

  // Cas diagonales
  if (player->pos_y < self->y) { // Supérieure
    target_top(self, &initial_target, ground, SIZE, player);

    if (player->pos_x < self->x) { // Supérieure gauche
      target_left(self, &initial_target, ground, SIZE, player);
      return;
    }

    // Supérieure droite
    target_right(self, &initial_target, ground, SIZE, player);
    return;
  }

  // Inférieure
  target_bottom(self, &initial_target, ground, SIZE, player);
  if (player->pos_x < self->x) { // Inférieure gauche
    target_left(self, &initial_target, ground, SIZE, player);
    return;
  }

  // Inférieure droite
  target_right(self, &initial_target, ground, SIZE, player);
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
  fprintf(stderr, "Objectif non optimisé : %i %i \n", target.x, target.y);
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
      fprintf(stderr, "Objectif non optimisé : %i %i (%ix%i)\n", target.x, target.y, target.w, target.h);
      target_optimise(&target, grid, SIZE, &multipla);
      fprintf(stderr, "Nouvel objectif optimisé : %i %i (%ix%i)\n", target.x, target.y, target.w, target.h);
      continue;
    }
  }

  free(grid);
  return 0;
}
