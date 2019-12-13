#!/bin/bash

if [ "$1" == "-h" ] ||  [ "$1" == "--help" ]; then
        echo -e "\e[36mStats for Race \e[39m"
        echo "Made by Nathanaël H."
        echo ""
        echo "  Usage :"
        echo "      stats-for-race <iteration> <player1> [player2] [player3] [player4]"
        echo "      stats-for-race (-h | --help)"
        echo ""
        echo "  Options : "
        echo "      <iteration> Number of tests"
        echo "      <players> Path to an executable player (for example, './myplayer')"
        exit 0
    fi

if [ "$#" -lt 2 ] || [ "$#" -gt 5 ] ; then
    echo -e "\e[91mstats-for-race : Illegal number of parameters.\e[39m Please try 'stats-for-race --help'"
    exit 0
fi

iteration_nb=$1
player_nb=$(expr $# - 1)
p1_name=$2
p2_name=$3
p3_name=$4
p4_name=$5

echo -e "\e[91mStatistiques pour Race"
echo -e "\e[36mNombre d'itérations : $iteration_nb \e[39m"

players=$p1_name" "$p2_name" "$p3_name" "$p4_name
for i in `seq 1 $iteration_nb`; do
    echo -e "\e[36mItération numéro $i \e[39m"
    ./race-server $players >> stats.log
done

lines_to_delete_nb=$(expr $player_nb + 3)
for i in `seq 1 $iteration_nb`; do
    echo -e "\e[36mRécupération des résultats pour itération n° $i \e[39m"
    tail stats.log -n $player_nb >> stats-2.log
    head -n -$lines_to_delete_nb stats.log > temp.txt
    mv temp.txt stats.log
done

mv stats-2.log stats.log

p1_total=0
p1_moves=0
p1_score=0
p1_victories=0
p2_total=0
p2_moves=0
p2_score=0
p2_victories=0
p3_total=0
p3_moves=0
p3_score=0
p3_victories=0
p4_total=0
p4_moves=0
p4_score=0
p4_victories=0

for i in `seq 1 $iteration_nb`; do
    for j in `seq 1 $player_nb`; do
        number=$(expr $i \* $j)
        echo -e "\e[36mTraitement de la participation n°$number \e[39m"

        player=$(tail -n 1 stats.log | cut -d' ' -f2 | rev | cut -c 2- | rev)

        is_disqualified=$(tail -n 1 stats.log | grep 'DISQUALIFIED!')
        if [ -n $is_disqualified ]; then
            total=$(tail -n 1 stats.log | cut -d' ' -f3)
            moves=$(tail -n 1 stats.log | cut -d' ' -f5 | rev | cut -c 2- | rev)
            score=$(tail -n 1 stats.log | cut -d' ' -f7 | rev | cut -c 2- | rev)

            if [ "$p1_name" == "$player" ]; then
                p1_total=$(expr $p1_total + $total)
                p1_moves=$(expr $p1_moves + $moves)
                p1_score=$(expr $p1_score + $score)
                if [ $j -eq $(expr $player_nb) ]; then
                        p1_victories=$(expr $p1_victories + 1)
                    fi
            fi

            if [ "$p2_name" == "$player" ]; then
                p2_total=$(expr $p2_total + $total)
                p2_moves=$(expr $p2_moves + $moves)
                p2_score=$(expr $p2_score + $score)
                if [ $j -eq $(expr $player_nb ) ]; then
                        p2_victories=$(expr $p2_victories + 1)
                    fi
            fi

            if [ "$p3_name" == "$player" ]; then
                p3_total=$(expr $p3_total + $total)
                p3_moves=$(expr $p3_moves + $moves)
                p3_score=$(expr $p3_score + $score)
                if [ $j -eq $(expr $player_nb) ]; then
                        p3_victories=$(expr $p3_victories + 1)
                    fi
            fi

            if [ "$p4_name" == "$player" ]; then
                p4_total=$(expr $p4_total + $total)
                p4_moves=$(expr $p4_moves + $moves)
                p4_score=$(expr $p4_score + $score)
                if [ $j -eq $(expr $player_nb) ]; then
                        p4_victories=$(expr $p4_victories + 1)
                    fi
            fi

            echo -e "Résultat : $player $total $moves $score"

        else 
            echo -e "Argh : $player DISQUALIFIÉ ! "
        fi 
        head -n -1 stats.log > temp.txt
        mv temp.txt stats.log
    done
done

echo 
echo -e "\e[91mRésultats finaux :\e[39m"

if [ $player_nb -eq 1 ]; then
    echo -e "$p1_name: $p1_total (moves: $p1_moves, score: $p1_score)"
    echo ""
    exit 0
fi
        
if [ $player_nb -eq 2 ]; then 
    if [ $p1_total -gt $p2_total ]; then
        echo -e "(1) $p2_name: $p2_victories victoires [$p2_total (moves: $p2_moves, score: $p2_score)]"
        echo -e "(2) $p1_name: $p1_victories victoires [$p1_total (moves: $p1_moves, score: $p1_score)]"
    else 
        echo -e "(1) $p1_name: $p1_victories victoires [$p1_total (moves: $p1_moves, score: $p1_score)]"
        echo -e "(2) $p2_name: $p2_victories victoires [$p2_total (moves: $p2_moves, score: $p2_score)]"
    fi
else
    echo -e "$p1_name: $p1_victories victoires [$p1_total (moves: $p1_moves, score: $p1_score)]"
    echo -e "$p2_name: $p2_victories victoires [$p2_total (moves: $p2_moves, score: $p2_score)]"
    echo -e "$p3_name: $p3_victories victoires [$p3_total (moves: $p3_moves, score: $p3_score)]"

    if [ $player_nb -eq 4 ]; then 
        echo -e "$p4_name: $p4_victories victoires [$p4_total (moves: $p4_moves, score: $p4_score)]"
    fi
fi
echo ""

rm stats.log