#include "shared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

/*
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
  fgets(buf, BUFSIZE, stdin);
  int player_x = atoi(buf);
  fgets(buf, BUFSIZE, stdin);
  int player_y = atoi(buf);

  // Récupérer les informations sur l'objectif
  int obj_x = 0; // abscisse
  int obj_y = 0; // ordonnée
  int obj_w = 0; // largeur
  int obj_h = 0; // hauteur
  get_obj_coords(&obj_x, &obj_y, &obj_w, &obj_h);

  // Vecteur vitesse du joueur
  int vx = 0;
  int vy = 0;

  for (size_t i = 0; 1; ++i) {
    vx = update_speed(player_x, vx, obj_x);
    vy = update_speed(player_y, vy, obj_y);

    // Mise à jour de la position du joueur
    player_x += vx;
    player_y += vy;

    // Envoyer les positions au serveur
    printf("%i\n%i\n", player_x, player_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) {
      fprintf(stderr, "ERREUR : Disqualifié\n");
      return 1;
    }
    get_obj_coords(&obj_x, &obj_y, &obj_w, &obj_h);
  }

  free(grid);
  return 0;
}
