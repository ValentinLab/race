#!/bin/bash
set -m

color_default="\033[39m"
color_blue="\033[34m"
color_red="\033[31m"
format_default="\033[0m"
format_bold="\033[1m"

if [ "$1" == "-h" ] ||  [ "$1" == "--help" ]; then
        echo -e "${color_blue}${format_bold}Viewer-simple for Race ${color_default}${format_default}"
        echo "Made by Nathanaël H. & Valentin P."
        echo ""
        echo "  Usage :"
        echo "      viewer-simple [-v2] [-s seed] <player1> [player2] [player3] [player4]"
        echo "      viewer-simple (-h | --help)"
        echo ""
        echo "  Options : "
        echo "      <players> Path to an executable player (for example, './myplayer')"
        echo "      [-v2] Use the race-viewer-v2 version"
        exit 0
    fi

if [ "$#" -lt 1 ] || [ "$#" -gt 7 ] ; then
    echo -e "${color_red}viewer-simple : Illegal number of parameters.${color_default} Please try 'viewer-simple --help'"
    exit 0
fi

if [ "$1" == "-v2" ]; then
    args=$2" "$3" "$4" "$5" "$6" "$7
    ./race-viewer-v2 & (sleep 1 && ./race-server $args) && fg
    exit 0
fi

args=$1" "$2" "$3" "$4" "$5" "$6
./race-viewer & (sleep 1 && ./race-server $args) && fg
exit 0
