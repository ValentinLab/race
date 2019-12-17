#ifndef SHARED_H
#define SHARED_H

#include <stdbool.h>
#include <stddef.h>

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
  int xright;
  int ybottom;
  int value;
};

/*
 * ----------------------------------------
 * Général
 * ----------------------------------------
 */

/*
 * Afficher dans stderr la grille de jeu avec la position du joueur
 */
void print_grid(const int *grid, const int SIZE);

/*
 * Retourner la valeur absolue de n
 */
int absol(int n);

/*
 * Calculer la somme des |n| premiers entiers
 */
int sum_1_to_n(int n);

/*
 * Calculer la coordonnée d'arrivée si on freine maintenant avec une coordonnée et une vitesse données
 */
int coord_if_we_break_now(int pos, int speed);

/*
 * ----------------------------------------
 * Struct player
 * ----------------------------------------
 */

/*
 * Initialiser une structure joueur avec les valeurs envoyées par le serveur
 */
void player_init(struct player *self, char *buf);

void player_dump(const struct player *self);

/*
 * On fait une copie wesh
 */
void player_copy(struct player *self, struct player *copy);

void target_dump(const struct target *self);

/*
 * Mettre à jour la position du joueur
 */
void player_update_pos(struct player *self);

/*
 * Calculer la distance entre le joueur et l'objectif
 */
int player_dist(const  struct player *self, const struct target *target, bool is_abscissa);

/* Réduire la vitesse sur un axe */
void player_reduce_speed_x(struct player *self);
void player_reduce_speed_y(struct player *self);

/* Augmenter la vitesse sur un axe, en direction de la cible */
void player_increase_speed_x(struct player *self, struct target *target);
void player_increase_speed_y(struct player *self, struct target *target);

/*
 * Met à jour la vitesse du joueur
 * S'il est trop proche de l'objectif par rapport à sa vitesse (delta < v + v-1 + ... + 1), il ralentit
 * S'il est assez loin de l'objectif, il accélère
 * Sinon, il garde la même vitesse.
 */
void update_speed(struct player *self, struct target *target);

/* Retourne vrai si on arrive sur le même axe que la cible en freinant maintenant */
bool is_on_target_X_if_brake_now(const struct player *self, const struct target *target);
bool is_on_target_Y_if_brake_now(const struct player *self, const struct target *target);

/* Retourne vrai si on dépasse la cible sur un axe en freinant maintenant */
bool is_overshooting_target_X_if_brake_now(const struct player *self, const struct target *target);
bool is_overshooting_target_Y_if_brake_now(const struct player *self, const struct target *target);

/* Retourne vrai si le joueur va atteindre l'axe de la cible en maintenant sa vitesse actuelle */
bool will_player_touch_target_X_with_current_speed(const struct player *self, const struct target *target);
bool will_player_touch_target_Y_with_current_speed(const struct player *self, const struct target *target);
bool will_player_touch_target_with_current_speed(const struct player *self, const struct target *target);

/*
 * ----------------------------------------
 * Struct target
 * ----------------------------------------
 */

/*
 * Initialiser une structure target avec les valeurs envoyées par le serveur
 */
void target_init(struct target *self, char *buf);

void target_dump(const struct target *self);

/*
 * Copie self dans copy
 */
void target_copy(const struct target *self, struct target *copy);

/*
 * Renvoie vrai si le joueur est dans la cible
 */
bool target_is_player_on(const struct target *self, const struct player *player);
#endif
