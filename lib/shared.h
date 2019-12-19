#ifndef SHARED_H
#define SHARED_H

#include <stdbool.h>
#include <stddef.h>

#define BUFSIZE 256
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

/**
 * pos_x, pos_y : position sur la grille
 * speed_x, speed_y : vitesse actuelle
 * has_accelerated_x, has_accelerated_y : <0 s'il a ralenti, 0 si il n'a pas modifié sa vitesse, +1 s'il a accéléré
 */
struct player {
  int pos_x;
  int pos_y;
  int speed_x;
  int speed_y;
  int has_accelerated_x;
  int has_accelerated_y;
};

struct target {
  int x;
  int y;
  int xright;
  int ybottom;
  int value;
};

/**
 * ----------------------------------------
 * Général
 * ----------------------------------------
 */

/**
 * Afficher dans stderr la grille de jeu avec la position du joueur
 */
void print_grid(const int *grid, const int SIZE);

/**
 * Retourner la valeur absolue de n
 */
int absol(int n);

/**
 * Calculer la somme des |n| premiers entiers
 */
int sum_1_to_n(int n);

/**
 * Calculer la coordonnée d'arrivée si on freine maintenant avec une coordonnée et une vitesse données
 */
int coord_if_we_break_now(int pos, int speed);

/**
 * ----------------------------------------
 * Struct player
 * ----------------------------------------
 */

/**
 * Initialiser une structure joueur avec les valeurs envoyées par le serveur
 */
void player_init(struct player *self, char *buf);

void player_dump(const struct player *self);

/**
 * On fait une copie wesh
 */
void player_copy(struct player *self, struct player *copy);

void target_dump(const struct target *self);

/**
 * Mettre à jour la position du joueur
 */
void player_update_pos(struct player *self);

/**
 * Calculer la distance entre le joueur et l'objectif
 */
int player_dist(const  struct player *self, const struct target *target, bool is_abscissa);

/** 
 * Réduire la vitesse sur un axe (ou sur les deux) 
 */
void player_reduce_speed_x(struct player *self);
void player_reduce_speed_y(struct player *self);
void player_reduce_speed(struct player *self);

/** 
 * Augmenter la vitesse sur un axe (ou sur les deux), en direction de la cible 
 */
void player_increase_speed_x(struct player *self, struct target *target);
void player_increase_speed_y(struct player *self, struct target *target);
void player_increase_speed(struct player *self, struct target *target);

/**
 * Renvoie vrai si le joueur est dans la cible
 */
bool player_is_on_target( const struct player *self, const struct target *target);

/** Retourne vrai si on arrive sur le même axe que la cible en freinant maintenant */
bool is_on_target_X_if_brake_now(const struct player *self, const struct target *target);
bool is_on_target_Y_if_brake_now(const struct player *self, const struct target *target);

/**
 *  Retourne vrai si on dépasse la cible sur un axe (ou sur un des deux) en freinant maintenant 
 */
bool is_overshooting_target_X_if_brake_now(const struct player *self, const struct target *target);
bool is_overshooting_target_Y_if_brake_now(const struct player *self, const struct target *target);
bool is_overshooting_target_if_brake_now(const struct player *self, const struct target *target);

/** 
 * Retourne le nombre d'itération pour la toucher si le joueur va atteindre l'axe de la cible (ou la cible elle-même) en maintenant sa vitesse actuelle
 * -1 s'il ne touchera pas la cible
 */
int will_player_touch_target_X_with_current_speed(const struct player *self, const struct target *target);
int will_player_touch_target_Y_with_current_speed(const struct player *self, const struct target *target);
int will_player_touch_target_with_current_speed(const struct player *self, const struct target *target);

/**
 * Ralentit pour s'assurer que le joueur ne sorte pas du terrain
 */
void player_slow_down_to_avoid_borders(struct player *self, const int GRID_SIZE);

/**
 * ----------------------------------------
 * Struct target
 * ----------------------------------------
 */

/**
 * Initialiser une structure target avec les valeurs envoyées par le serveur
 */
void target_init(struct target *self, char *buf);

void target_dump(const struct target *self);

/**
 * Copie self dans copy
 */
void target_copy(const struct target *self, struct target *copy);

#endif
