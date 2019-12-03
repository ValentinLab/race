#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

int main(int argc, char const *argv[]) {
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

  for (;;) {
    // print_grid(SIZE, player_x, player_y);
    if (obj_x < player_x) { // Nouvelle vitesse en X
      if (vx > 0) {
        vx = 0;
      } else {
        vx = -1;
      }
    } else if (obj_x > player_x) {
      if (vx < 0) {
        vx = 0;
      } else {
        vx = 1;
      }
    } else {
      vx = 0;
    }

    if (obj_y < player_y) { // Nouvelle vitesse en Y
      if (vy > 0) {
        vy = 0;
      } else {
        vy = -1;
      }
    } else if (obj_y > player_y) {
      if (vy < 0) {
        vy = 0;
      } else {
        vy = 1;
      }
    } else {
      vy = 0;
    }

    // Mise à jour de la position du joueur
    player_x += vx;
    player_y += vy;

    // Envoyer les positions au serveur
    printf("%i\n%i\n", player_x, player_y);

    // Récupérer la réponse du serveur
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH\n") == 0) { // fin du jeu ou erreur
      break;
    }
    get_obj_coords(&obj_x, &obj_y, &obj_w, &obj_h); // nouvel objectif
  }

  free(grid);
  return 0;
}
