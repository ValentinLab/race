#include "shared.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

int main(int argc, char const *argv[]) {
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

  for (;;) {
    print_grid(SIZE, player_x, player_y);
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

    player_x += vx; // Nouvelle position
    player_y += vy; //

    printf("%i\n%i\n", player_x, player_y); // Envoyer les positions au serveur

    fgets(buf, BUFSIZE, stdin); // Récupérer la réponse du serveur

    if (strcmp(buf, "ERROR\n") == 0 || strcmp(buf, "FINISH") == 0) {
      break;
    }

    if (strcmp(buf, "CHECKPOINT\n") == 0) { // Récupérer le nouvel objectif
      get_obj_coords(&obj_x, &obj_y, &obj_w, &obj_h);
      continue;
    }
  }

  free(grid);
  return 0;
}
