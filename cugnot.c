#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/**
 * Met à jour la vitesse du joueur
 * S'il est trop proche de l'objectif par rapport à sa vitesse (delta < v + v-1 + ... + 1), il ralentit
 * S'il est assez loin de l'objectif, il accélère
 * Sinon, il garde la même vitesse.
 */
static int update_speed(int player_axis_p, int v, int obj_axis_p) {
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
}

int main() {
  setbuf(stdout, NULL);
  char buf[BUFSIZE];

  fgets(buf, BUFSIZE, stdin); // Récupérer la taille
  const int SIZE = atoi(buf);

  int *grid = calloc(SIZE * SIZE, sizeof(int));

  for (int i = 0; i < SIZE * SIZE; ++i) {
    fgets(buf, BUFSIZE, stdin); // Récupérer les valeurs sur la grille
    grid[i] = atoi(buf);
  }

  fgets(buf, BUFSIZE, stdin); // Récupérer l'abscisse initiale du joueur
  int player_x = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Récupérer l'ordonnée initiale du joueur
  int player_y = atoi(buf);

  int obj_x = 0; // Informations sur l'objectif
  int obj_y = 0; //
  int obj_w = 0; //
  int obj_h = 0; //
  get_obj_coords(&obj_x, &obj_y, &obj_w, &obj_h);

  int vx = 0;
  int vy = 0;

  for (size_t i = 0; 1; ++i) {
    vx = update_speed(player_x, vx, obj_x);
    vy = update_speed(player_y, vy, obj_y);

    player_x += vx; // Nouvelle position
    player_y += vy; //

    printf("%i\n%i\n", player_x, player_y); // Envoyer les positions au serveur

    fgets(buf, BUFSIZE, stdin); // Récupérer la réponse du serveur

    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH") == 0) {
      fprintf(stderr, "ERREUR : Disqualifié\n");
      return 1;
    }

    if (strcmp(buf, "CHECKPOINT\n") == 0) { // Récupérer le nouvel objectif
      get_obj_coords(&obj_x, &obj_y, &obj_w, &obj_h);
      continue;
    }
  }

  free(grid);
  return 0;
}
