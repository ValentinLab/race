#include <stddef.h>

/*
 * Écrire dans stderr la grille de jeu avec la position du joueur
 */
void print_grid(int size, int px, int py);

/**
 * Écrire dans les paramètres les informations reçues du serveur concernant l'objectif
 */
void get_obj_coords(int *x, int *y, int *w, int *h);

/** Retourne la distance sur un axe entre le joueur et l'objectif
* > 0 si l'objectif est à droite du joueur */
int axis_dist_2_obj(int player_position, int obj_position);

/** Réduire une vitesse de 1 (positive ou négative) */
int reduce_v(int v);

/** Augmenter une vitesse de 1 (positive ou négative) vers un objectif donnée */
int increase_v(int v, int player_position, int obj_position);

/** Calculer la somme de 1 à |n| */
int sum_1_to_n(int n);