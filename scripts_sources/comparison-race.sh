#!/bin/bash

# Couleurs et format
color_default="\033[39m"
color_blue="\033[34m"
color_red="\033[31m"
format_default="\033[0m"
format_bold="\033[1m"

# Affichage de l'aide
if [ $1 = "-h" ] || [ $1 = "--help" ]; then
  echo -e "${color_blue}${format_bold}Statistiques pour Race${format_default}${color_default}"
  echo "  Utilisation : ./stats.sh <JOUEUR> <ADVERSAIRE> <ITERATION>"
  echo ""
  echo "  Paramètres : <JOUEUR>      premier joueur à exécuter (exemple : race-hi)"
  echo "               <ADVERSAIRE>  deuxième joueur à exécuter"
  echo "               <ITERATION>   nombre d'exécutions"
  exit
fi

# Vérifier les paramètres
if [ $# -ne 3 ]; then
  echo -e "${color_red}Erreur${color_default} : Nombre de paramètres incorrecte."
  exit
fi
player1=$1
player2=$2
iteration=$3

# Créer un dossier tmp
mkdir tmp

# Boucle d'exécution
echo -e "${color_blue}${format_bold}Statistiques de ${player1} vs ${player2}${format_default}${color_default}\n"
index=0
victories=0
defeats=""
while [ $index -lt $3 ]; do
  # Exécuter le fichier
  echo "---[ Exécution $[$index+1]/$iteration"
  ./race-server ./$player1 ./$player2 > tmp/all.txt 2>> tmp/error.txt

  # Compter le nombre de fois où player1 arrive premier
  current=`grep "(1) ./$player1" tmp/all.txt`
  if [ -z "$current" ]; then
    defeat=`grep "Seed" tmp/all.txt | cut -c7-`
    defeats="${defeats} ${defeat}"
  else
    victories=$[$victories+1]
  fi

  index=$[$index+1]
done

# Affichage su résultat
echo -e "${color_blue}\nRésultat :${color_default}"
echo    "./$player1 : $victories victoire(s)"
echo    "./$player2 : $[$iteration - $victories] victoire(s)"

# Affichage des seeds
if [ -n defeats ]; then
  echo -e "${color_blue}\nSeeds de défaite :${color_default}"
  for seed in $defeats; do
    echo " - $seed"
  done
fi

# Supprimer le dossier tmp
rm -rf tmp
