#!/bin/bash -
#===============================================================================
#
#          FILE: test.sh
#
#         USAGE: ./test.sh
#
#   DESCRIPTION:
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (),
#  ORGANIZATION:
#       CREATED: 05/01/2019 23:02
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error
qlist -I | sort | uniq -c | awk '{if ($1 != 1) print $2}' | xargs equery l
