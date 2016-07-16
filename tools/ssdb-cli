#!/bin/sh
DIR=`S=\`readlink "$0"\`; [ -z "$S" ] && S=$0; dirname $S`

if [ -e $DIR/../deps/cpy/cpy ]; then
	CPY=$DIR/../deps/cpy/cpy
else
	CPY=$DIR/deps/cpy/cpy
fi

$CPY $DIR/ssdb-cli.cpy $@
