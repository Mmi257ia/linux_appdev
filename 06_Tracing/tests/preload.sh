#!/bin/sh

# Usage: ./tests/preload.sh <move binary path> <preserver library path>

# Checking:
# preserver preloading

exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE
    exit $EXIT_STATUS
}

trap exit_handler EXIT HUP INT QUIT PIPE TERM

BINARY=$1
PRESERVER=$2
PROTECT=PROTECT
INPUT_FILE=`mktemp`
OUTPUT_FILE=`mktemp`

echo "blahblah" >> $INPUT_FILE # for $INPUT_FILE to be non-empty
mv $INPUT_FILE "$INPUT_FILE$PROTECT"
INPUT_FILE="$INPUT_FILE$PROTECT"

LD_PRELOAD=$PRESERVER $BINARY $INPUT_FILE $OUTPUT_FILE
test -f $INPUT_FILE || exit 1
