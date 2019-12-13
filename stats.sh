#!/bin/bash

# Titre
echo -e "# Classement de ./$1 ($2 exécution)\n"

# Créer un dossier tmp
mkdir tmp

# Éxecuter x fois ./race-server
index=0
while [ $index -lt $2 ]; do
  echo "---[ Exécution $[$index+1]/$2"
  ./race-server ./$1 ./race-hi >> tmp/all.txt 2>> tmp/error.txt
  index=$[$index+1]
done

# Compter le nombre de fois où player arrive premier
grep "(1) ./$1" tmp/all.txt > tmp/order.txt
order=`wc -l tmp/order.txt`

# Affichage du résultat
echo -e "\n${order:0:2} sur $index"

# Supprimer le dossier tmp
rm -rf tmp
