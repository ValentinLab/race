#include <stddef.h>

/*
 * Écrire dans stderr la grille de jeu avec la position du joueur
 */
void print_grid(int size, int px, int py);

/**
 * Écrire dans les paramètres les informations reçues du serveur concernant l'objectif
 */
void get_obj_coords(int *x, int *y, int *w, int *h);