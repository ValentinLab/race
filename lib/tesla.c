#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/*
 * Met à jour la vitesse du joueur
 * S'il est trop proche de l'objectif par rapport à sa vitesse 
 * (delta < v + v-1 + ... + 1 <=> distance_vers_objectif <= distance_de_freinage), 
 * alors il ralentit.
 * S'il est assez loin de l'objectif, il accélère
 * Sinon, il garde la même vitesse.
 */
static void update_speed(struct player *self, struct target *target) {
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

/**
 * Modifie *self avec la valeur la plus faible disponible sur une ligne de la grille
 */
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

  self->xright = self->x; // Taille 1x1 pour la cible
  self->ybottom = self->y;
}

/**
 * Modifie *self avec la meilleure case de la ligne du haut de la cible
 */
static void target_optimise_top(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x;
  size_t iter_x_max = initial_target->xright;
  size_t iter_y_min = initial_target->y;
  size_t iter_y_max = initial_target->y;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

/**
 * Modifie *self avec la meilleure case de la ligne du bas de la cible
 */
static void target_optimise_bottom(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x;
  size_t iter_x_max = initial_target->xright;
  size_t iter_y_min = initial_target->ybottom;
  size_t iter_y_max = initial_target->ybottom;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

/**
 * Modifie *self avec la meilleure case de la ligne de gauche de la cible
 */
static void target_optimise_left(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->x;
  size_t iter_x_max = initial_target->x;
  size_t iter_y_min = initial_target->y;
  size_t iter_y_max = initial_target->ybottom;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

/**
 * Modifie *self avec la meilleure case de la ligne de droite de la cible
 */
static void target_optimise_right(struct target *self, const struct target *initial_target, const int *ground, const size_t SIZE, const struct player *player) {
  size_t iter_x_min = initial_target->xright;
  size_t iter_x_max = initial_target->xright;
  size_t iter_y_min = initial_target->y;
  size_t iter_y_max = initial_target->ybottom;
  target_optimise_line(self, ground, SIZE, iter_x_min, iter_x_max, iter_y_min, iter_y_max);
}

/**
 * Modifie la case cible vers la case à la valeur la plus petite dans la cible, parmi les cases qui sont sur les bords 
 * Retourne 1 si on utilise target temp
 * Voir la description de l'algorithme dans le README.
 */
static int target_optimise(struct target *self, struct target *temp, const int *ground, const size_t SIZE, const struct player *player) {
  self->value = 0x7FFFFFFF;
  struct target initial_target;
  target_copy(self, &initial_target);

  if (self->x <= player->pos_x && player->pos_x <= self->xright && player->pos_y < self->y) { // Partie supérieure
    target_optimise_top(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  if (self->x <= player->pos_x && player->pos_x <= self->xright && self->ybottom < player->pos_y) { // Partie inférieure
    target_optimise_bottom(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  if (self->y <= player->pos_y && player->pos_y <= self->ybottom && player->pos_x < self->x) { // Partie gauche
    target_optimise_left(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  if (self->y <= player->pos_y && player->pos_y <= self->ybottom && self->xright < player->pos_x) { // Partie droite
    target_optimise_right(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  // Cas diagonales
  if (player->pos_y < self->y) { // Diagonale supérieure
    target_optimise_top(self, &initial_target, ground, SIZE, player);

    if (player->pos_x < self->x) { // Supérieure gauche
      target_optimise_left(self, &initial_target, ground, SIZE, player);
      target_copy(self, temp);
      --temp->x;
      --temp->y;
      temp->xright = temp->x;
      temp->ybottom = temp->y;
      return 1;
    }

    // Supérieure droite
    target_optimise_right(self, &initial_target, ground, SIZE, player);
    target_copy(self, temp);
    ++temp->x;
    --temp->y;
    temp->xright = temp->x;
    temp->ybottom = temp->y;
    return 1;
  }

  // Diagonale inférieure
  target_optimise_bottom(self, &initial_target, ground, SIZE, player);
  if (player->pos_x < self->x) { // Inférieure gauche
    target_optimise_left(self, &initial_target, ground, SIZE, player);
    target_copy(self, temp);
    --temp->x;
    ++temp->y;
    temp->xright = temp->x;
    temp->ybottom = temp->y;
    return 1;
  }

  // Inférieure droite
  target_optimise_right(self, &initial_target, ground, SIZE, player);
  target_copy(self, temp);
  ++temp->x;
  ++temp->y;
  temp->xright = temp->x;
  temp->ybottom = temp->y;
  return 1;
}

static void get_and_optimize_obj(struct target *real_target, struct target *target, struct player *player, char *buf, const int *grid, const int SIZE) {

  struct target initial_target;
  target_init(&initial_target, buf);
  target_copy(&initial_target, real_target);

  int status = target_optimise(real_target, target, grid, SIZE, player);
  if (status == 0) {
    // Pas besoin de cible temporaire
    target_copy(real_target, target);
    return;
  }

  // Là, on regarde dans quelle configuration on se trouve. Soit (voir photo pour explication)
  // On stocke toutes les cases par lesquelles on va passer si on ne va pas faire de détour
  struct player pl_copy;

  player_copy(player, &pl_copy);
  while (!player_is_on_target(&pl_copy, &initial_target)) {
    update_speed(&pl_copy, real_target);
    player_update_pos(&pl_copy);
  }

  if (player_is_on_target(&pl_copy, real_target)) { // Si on arrive sur la vraie cible : on a pas besoin de cible temporaire
    target_copy(real_target, target);
  }
  // Sinon, on est du mauvais côté -> on garde la cible temporaire
}

int main() {
  // Structures
  struct player tesla;
  struct target real_target;
  struct target curr_target;

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
  player_init(&tesla, buf);

  // Récupérer les informations sur le premier objectif
  get_and_optimize_obj(&real_target, &curr_target, &tesla, buf, grid, SIZE);

  for (size_t round = 1;; ++round) {
    fprintf(stderr, "---[ Round #%zu\n", round);

    fprintf(stderr, "Cible actuelle : ");
    target_dump(&curr_target);
    fprintf(stderr, "\n");

    update_speed(&tesla, &curr_target);
    player_slow_down_to_avoid_borders(&tesla, SIZE);
    player_update_pos(&tesla);
    if (player_is_on_target(&tesla, &curr_target)) {
      // Si on est arrivé sur la cible temporaire, alors on vise maintenant la vraie cible
      target_copy(&real_target, &curr_target);
    }

    // Envoyer les positions au serveur
    printf("%i\n%i\n", tesla.pos_x, tesla.pos_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      break;
    }

    if (strcmp(buf, "CHECKPOINT\n") == 0) {
      get_and_optimize_obj(&real_target, &curr_target, &tesla, buf, grid, SIZE);
      continue;
    }
  }

  free(grid);
  return 0;
}
