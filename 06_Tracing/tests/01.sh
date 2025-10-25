#!/bin/sh

# Usage: ./tests/01.sh <move binary path>

# Checking:
# opening infile
# openat(AT_FDCWD, "f", O_RDONLY)         = 3
# no output file must be created

exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $REFERENCE
    exit $EXIT_STATUS
}

trap exit_handler EXIT HUP INT QUIT PIPE TERM

BINARY=$1
INPUT_FILE=`mktemp`
OUTPUT_FILE=blahblahblah
ERR_NAME=ENOENT
ERR_CODE=2
ERR_DESCRIPTION="No such file or directory"
OUTPUT_STRING="$BINARY: error while opening file $INPUT_FILE: $ERR_DESCRIPTION"
EXITED_STRING="+++ exited with $ERR_CODE +++"

REFERENCE=`mktemp`
printf '%s\n%s\n' "$OUTPUT_STRING" "$EXITED_STRING" > $REFERENCE

strace -e fault=openat:error=$ERR_NAME -P $INPUT_FILE $BINARY $INPUT_FILE $OUTPUT_FILE 2>&1 | grep -E "\+\+\+|$BINARY:" | cmp $REFERENCE || exit 1
test ! -f $OUTPUT_FILE || exit 1
