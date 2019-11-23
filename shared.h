#ifndef SHARED_H
#define SHARED_H

#include <stddef.h>

#define BUFSIZE 256
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

/*
 * Affichfer dans stderr la grille de jeu avec la position du joueur
 */
void print_grid(int size, int px, int py);

/*
 * Lire et enregistrer les informations reçues du serveur concernant l'objectif
 */
void get_obj_coords(int *x, int *y, int *w, int *h);

/*
 * Obtenir la distance sur un axe entre le joueur et l'objectif
 * > 0 si l'objectif est à droite du joueur 
 */
int axis_dist_2_obj(int player_position, int obj_position);

/*
 * Réduire une vitesse de 1 (positive ou négative) 
 */
int reduce_v(int v);

/*
 * Augmenter une vitesse de 1 (positive ou négative)
 */
int increase_v(int v, int player_position, int obj_position);

/*
 * Calculer la somme des |n| premiers entierss 
 */
int sum_1_to_n(int n);

#endif