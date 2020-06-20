#!/bin/sh

# edist.sh
#
# Calculate the edit distance between two strings
# The distance calculated is the Levenshtein distance

USAGE='edit [WORD_1] [WORD_2]'

if [ "$#" -ne 2 ]
then
	echo "$USAGE"
	exit 1
fi

dym -v -e "$1" "$2" | awk -e '{ print $2; }'
