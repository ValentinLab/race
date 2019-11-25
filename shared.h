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
 * Afficher dans stderr la grille de jeu avec la position du joueur
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

/*
 * Initialiser une structure joueur avec les valeurs envoyées par le serveur
 */
void player_init(struct player *self, char *buf);

/*
 * Mettre à jour la position du joueur
 */
void player_update_pos(struct player *self);

/*
 * Calculer la distance entre le joueur et l'objectif
 */
int player_dist(struct player *self, struct target *other, bool is_abscissa);

/*
 * Réduire la vitesse (axe x)
 */
void player_reduce_speed_x(struct player *self);

/*
 * Réduire la vitesse (axe y)
 */
void player_reduce_speed_y(struct player *self);

/*
 * Augmenter la vitesse (axe x)
 */
void player_increase_speed_x(struct player *self, struct target *other);

/*
 * Augmenter la vitesse (axe y)
 */
void player_increase_speed_y(struct player *self, struct target *other);

/*
 * ----------------------------------------
 * Struct player
 * ----------------------------------------
 */

/*
 * Initialiser une structure target avec les valeurs envoyées par le serveur
 */
void target_init(struct target *self, char *buf);

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