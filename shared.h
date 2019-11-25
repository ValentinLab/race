#ifndef SHARED_H
#define SHARED_H

#include <stddef.h>
#include <stdbool.h>

#define BUFSIZE 256
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

struct player {
  int pos_x;
  int pos_y;
  int speed_x;
  int speed_y;
};

struct target {
  int x;
  int y;
  int w;
  int h;
};

/*
 * ----------------------------------------
 * Général
 * ----------------------------------------
 */

/*
 * Affichfer dans stderr la grille de jeu avec la position du joueur
 */
void print_grid(int size, int px, int py);

/*
 * Calculer la somme des |n| premiers entiers
 */
int sum_1_to_n(int n);

/*
 * ----------------------------------------
 * Struct player
 * ----------------------------------------
 */

void player_init(struct player *self);

void player_update_pos(struct player *self);

int player_dist(struct player *self, struct target *other, bool is_abscissa);

void player_reduce_speed_x(struct player *self);

void player_reduce_speed_y(struct player *self);

void player_increase_speed_x(struct player *self, struct target *other);

void player_increase_speed_y(struct player *self, struct target *other);

/*
 * ----------------------------------------
 * Struct player
 * ----------------------------------------
 */

void target_init(struct target *self);

/*
 * Lire et enregistrer les informations reçues du serveur concernant l'objectif
 */
//void get_obj_coords(int *x, int *y, int *w, int *h);

/*
 * Obtenir la distance sur un axe entre le joueur et l'objectif
 * > 0 si l'objectif est à droite du joueur 
 */
//int axis_dist_2_obj(int player_position, int obj_position);

/*
 * Réduire une vitesse de 1 (positive ou négative) 
 */
//int reduce_v(int v);

/*
 * Augmenter une vitesse de 1 (positive ou négative)
 */
//int increase_v(int v, int player_position, int obj_position);

#endif