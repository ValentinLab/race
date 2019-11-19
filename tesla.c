#include "tesla.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

void print_grid(int size, int px, int py) {
  for(size_t i = 0; i < size; ++i) {
    fprintf(stderr, "|");
    for(size_t j = 0; j < size; ++j) {
      if(i == px && j == py) {
        fprintf(stderr, "x");
      } else {
        fprintf(stderr, " ");
      }
      fprintf(stderr, "|");
    }
    fprintf(stderr, "\n");
  }
}

int main(int argc, char const *argv[]) {
  setbuf(stdout, NULL);
  char buf[BUFSIZE];
  
  fgets(buf, BUFSIZE, stdin); // Récupérer la taille
  int size = atoi(buf);

  for (int i = 0; i < size * size; ++i) {
    fgets(buf, BUFSIZE, stdin); // Récupérer les valeurs sur la grille
    int value = atoi(buf);
    // TODO: do something with value
  }

  fgets(buf, BUFSIZE, stdin); // Récupérer l'abscisse initiale
  int px = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Récupérer l'ordonnée initiale
  int py = atoi(buf);

  fgets(buf, BUFSIZE, stdin); // Récupérer l'abscisse de l'objectif
  int x = atoi(buf); 
  fgets(buf, BUFSIZE, stdin); // Récupérer l'ordonnée de l'objectif
  int y = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Récupérer la largeur de l'objectif
  int w = atoi(buf);
  fgets(buf, BUFSIZE, stdin); // Récupérer la hauteur de l'objectif
  int h = atoi(buf);
  int vx = 0;
  int vy = 0;

  print_grid(size, px, py);
  return;
  
  for (;;) {
    // compute new px and new py
    // TODO
    printf("%i\n%i\n", px, py);
    // get the response
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "ERROR\n") == 0) {
      break;
    }
    // TODO
  }

  return 0;
}
