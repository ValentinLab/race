#!/bin/bash

if [ "$1" == "-h" ] ||  [ "$1" == "--help" ]; then
        echo -e "\e[36mViewer-simple for Race \e[39m"
        echo "Made by Nathanaël H."
        echo ""
        echo "  Usage :"
        echo "      viewer-simple <player1> [player2] [player3] [player4]"
        echo "      viewer-simple (-h | --help)"
        echo ""
        echo "  Options : "
        echo "      <players> Path to an executable player (for example, './myplayer')"
        exit 0
    fi

if [ "$#" -lt 1 ] || [ "$#" -gt 4 ] ; then
    echo -e "\e[91mviewer-simple : Illegal number of parameters.\e[39m Please try 'viewer-simple --help'"
    exit 0
fi

players=$1" "$2" "$3" "$4

./race-viewer & (sleep 1 && ./race-server $players) && fg

exit 0
