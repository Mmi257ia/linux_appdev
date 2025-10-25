#!/bin/sh

# Usage: ./tests/03.sh <move binary path>

# Checking:
# read(3, "aergneirgjerogier\n\nse'rgisoerige"..., 4096) = 36
# no output file must be created, input must not be deleted

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
OUTPUT_STRING="$BINARY: fread() failed on file $INPUT_FILE: $ERR_DESCRIPTION"
EXITED_STRING="+++ exited with $ERR_CODE +++"

REFERENCE=`mktemp`
printf '%s\n%s\n' "$OUTPUT_STRING" "$EXITED_STRING" > $REFERENCE

echo "blahblah" >> $INPUT_FILE # for $INPUT_FILE to be non-empty
strace -e fault=read:error=$ERR_NAME -P $INPUT_FILE $BINARY $INPUT_FILE $OUTPUT_FILE 2>&1 | grep -E "\+\+\+|$BINARY:" | cmp $REFERENCE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1
