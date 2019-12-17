#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/*
 * Met à jour la vitesse du joueur
 * S'il est trop proche de l'obj
void update_speed(struct player *self, struct target *target);
ectif par rapport à sa vitesse (delta < v + v-1 + ... + 1), il ralentit
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
 * Retourne 1 si on utilise target temp
 */
static int target_optimise(struct target *self, struct target *temp, const int *ground, const size_t SIZE, const struct player *player) {
  self->value = 0x7FFFFFFF;
  struct target initial_target;
  target_copy(self, &initial_target);

  size_t max_obj_x = self->x + self->w - 1;
  size_t max_obj_y = self->y + self->h - 1;

  if (self->x <= player->pos_x && player->pos_x <= max_obj_x && player->pos_y < self->y) { // Partie supérieure
    target_top(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  if (self->x <= player->pos_x && player->pos_x <= max_obj_x && max_obj_y < player->pos_y) { // Partie inférieure
    target_bottom(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  if (self->y <= player->pos_y && player->pos_y <= max_obj_y && player->pos_x < self->x) { // Partie gauche
    target_left(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  if (self->y <= player->pos_y && player->pos_y <= max_obj_y && max_obj_x < player->pos_x) { // Partie droite
    target_right(self, &initial_target, ground, SIZE, player);
    return 0;
  }

  // Cas diagonales
  if (player->pos_y < self->y) { // Supérieure
    target_top(self, &initial_target, ground, SIZE, player);

    if (player->pos_x < self->x) { // Supérieure gauche
      target_left(self, &initial_target, ground, SIZE, player);
      target_copy(self, temp);
      --temp->x;
      --temp->y;
      return 1;
    }

    // Supérieure droite
    target_right(self, &initial_target, ground, SIZE, player);
    target_copy(self, temp);
    ++temp->x;
    --temp->y;
    return 1;
  }

  // Inférieure
  target_bottom(self, &initial_target, ground, SIZE, player);
  if (player->pos_x < self->x) { // Inférieure gauche
    target_left(self, &initial_target, ground, SIZE, player);
    target_copy(self, temp);
    --temp->x;
    ++temp->y;
    return 1;
  }

  // Inférieure droite
  target_right(self, &initial_target, ground, SIZE, player);
  target_copy(self, temp);
  ++temp->x;
  ++temp->y;
  return 1;
}

static void get_and_optimize_obj(struct target *real_target, struct target *target, struct player *player, char *buf, const int *grid, const int SIZE) {

  struct target initial_target;
  initial_target.x = 0;
  initial_target.y = 0;
  initial_target.w = 0;
  initial_target.h = 0;
  initial_target.value = 0;
  target_init(&initial_target, buf);
  fprintf(stderr, "Objectif non optimisé : ");
  target_dump(&initial_target);

  target_copy(&initial_target, real_target);
  int status = target_optimise(real_target, target, grid, SIZE, player);
  if (status == 0) {
    // Pas besoin de cible temporaire
    target_copy(real_target, target);
    fprintf(stderr, "Nouvel objectif optimisé : ");
    target_dump(real_target);
    return;
  }

  // Là, on regarde dans quelle configuration on se trouve. Soit (voir photo pour explication)
  // On stocke toutes les cases par lesquelles on va passer si on ne va pas faire de détour
  struct player pl_copy;

  player_copy(player, &pl_copy);
  while (!target_is_player_on(&initial_target, &pl_copy)) {
    update_speed(&pl_copy, real_target);
    player_update_pos(&pl_copy);
  }

  if (target_is_player_on(real_target, &pl_copy)) { // Si on arrive sur la vraie cible : on a pas besoin de cible temporaire
    target_copy(real_target, target);
    fprintf(stderr, "Nouvel objectif optimisé (pas besoin de temp) : ");
    target_dump(target);
  } else { // On est du mauvais côté, on garde la cible temporaire
    fprintf(stderr, "Nouvel objectif (temporaire) optimisé : ");
    target_dump(target);
    fprintf(stderr, "Nouvel objectif optimisé : ");
    target_dump(real_target);
  }
}

int main() {
  // Structures
  struct player multipla;
  struct target real_target;
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

  print_grid(grid, SIZE);
  fprintf(stderr, "\n");

  // Récupérer la position initiale du joueur
  player_init(&multipla, buf);

  // Récupérer les informations sur l'objectif
  get_and_optimize_obj(&real_target, &target, &multipla, buf, grid, SIZE);

  for (size_t round = 1;; ++round) {
    fprintf(stderr, "---[ Round #%zu\n", round);

    update_speed(&multipla, &target);
    player_update_pos(&multipla);
    if (target_is_player_on(&target, &multipla)) {
      target_copy(&real_target, &target);
    }

    // Envoyer les positions au serveur
    printf("%i\n%i\n", multipla.pos_x, multipla.pos_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      break;
    }

    if (strcmp(buf, "CHECKPOINT\n") == 0) {
      get_and_optimize_obj(&real_target, &target, &multipla, buf, grid, SIZE);
      continue;
    }
  }

  free(grid);
  return 0;
}
