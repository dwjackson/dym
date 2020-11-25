#!/bin/sh

# edist.sh
#
# Calculate the edit distance between two strings
# The distance calculated is the Damerau-Levenshtein distance

USAGE='edit [WORD_1] [WORD_2]'

if [ "$#" -ne 2 ]
then
	echo "$USAGE"
	exit 1
fi

dym -D -d -e "$1" "$2"
