\title{Race - Rapport}
\author{Nathanaël Houn, Valentin Perignon}
\maketitle
\tableofcontents

# Arborescence du projet
- `/lib` : les `.c` des différents joueurs, et la librairie commune et son header. `shared.c` contient la déclaration des structures, les fonctions pour les créer/détruire/copier etc;
- `Makefile` pour construire tous les joueurs et placer les exécutables à la racine

# Liste des joueurs
 - **slow.c** : premier joueur réalisé, il se déplace avec une vitesse constante de |1|. *depreciated* car il n'utilise pas les structures;
 - **cugnot.c** : le joueur adapte sa vitesse en fonction de sa distance à l'objectif pour réduire le nombre de coups;
 - **multipla.c** : en plus d'augmenter sa vitesse, le joueur sélectionne parmi les cases de la cible celle qui possède le plus faible score sur les bords de la cible.
 - **trottinette.c** : _En cours de développement_ Accélère autant tant qu'il touche la cible et ne sort pas du terrain. Ne choisit pas de meilleure case. 

# Nos structures
Les structures sont toutes accompagnées d'une fonction `structure_init()` qui instancie la structure avec les paramètres envoyés par le serveur.

## Joueur
```c
struct player {
  int pos_x;
  int pos_y;
  int speed_x;
  int speed_y;
};
```
Structure pour le joueur.
La vitesse est un `int` car elle est négative si on se dirige vers la gauche ou vers le haut.
La fonction `player_update_pos()` permet de mettre à jour la position du joueur en fonction de ses vitesses actuelles.

## Cible
```c
struct target {
  int x;
  int y;
  int w;
  int h;
  int xright;
  int ybottom;
  int value;
};
```
Structure pour une cible.
Nous y stockons ce qui est donné par le serveur `(x, y, w, h)` mais aussi les coordonnées `(xright, ybottom)` du coin inférieur droit, qui sont calculés à l'initialisation de la cible, afin de simplifier nos calculs dans plusieurs fonctions.

# Nos algorithmes

## Adapter la vitesse - cugnot et multipla
Tout au long du jeu, le joueur adapte sa vitesse en fonction de sa position par rapport au checkpoint.
À chaque coup, la distance de freinage est calculée en utilisant la formule des n premiers entiers. Par exemple, si la vitesse du joueur est de 3, alors la distance de freinage est de *3+2+1=6*.
Si cette distance est inférieure à la distance avec l'objectif, le joueur réduit sa vitesse.

## Trouver la meilleure case de l'objectif - multipla
Pour réduire le score, le joueur cherche, à chaque fois qu'une nouvelle cible est disponible, la case qui contient la plus faible valeur.  
Selon l'algorithme de régulation de la vitesse, le joueur arrive dans l'objectif avec une vitesse de |1|. Celui-ci ne peut donc toucher que les cases du bord de la direction dans laquelle il arrive.
L'algorithme sélectionne la case avec le plus faible poid en réduisant la recherche aux cases accessibles.  
Il existe deux cas. Soit le joueur arrive face à l'objectif (depuis la gauche, la droite, le haut ou le bas), dans ce cas la recherche se limite à une ligne ou colonne (celle qui lui fait face). Soit le joueur arrive depuis une diagonale, dans ce cas la recherche se fait sur une ligne et une colonne (les deux qui sont accessibles). Voir figure \ref{meilleurecase} (les cases accessibles sont en vert).

![Choix de la meilleure case\label{meilleurecase}](meilleurecase.png)
