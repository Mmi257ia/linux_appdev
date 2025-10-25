#!/bin/sh

# Usage: ./tests/05.sh <move binary path>

# Checking:
# opening outfile
# openat(AT_FDCWD, "f2", O_WRONLY|O_CREAT|O_TRUNC, 0666) = 3
# output file must not be deleted and changed, input must not be deleted

exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE $OUTPUT_FILE_2 $REFERENCE
    exit $EXIT_STATUS
}

trap exit_handler EXIT HUP INT QUIT PIPE TERM

BINARY=$1
INPUT_FILE=`mktemp`
OUTPUT_FILE=`mktemp`
OUTPUT_FILE_2=`mktemp`
ERR_NAME=ENOENT
ERR_CODE=2
ERR_DESCRIPTION="No such file or directory"
OUTPUT_STRING="$BINARY: error while opening file $OUTPUT_FILE: $ERR_DESCRIPTION"
EXITED_STRING="+++ exited with $ERR_CODE +++"

REFERENCE=`mktemp`
printf '%s\n%s\n' "$OUTPUT_STRING" "$EXITED_STRING" > $REFERENCE

echo "blahblah" >> $OUTPUT_FILE # for $INPUT_FILE to be non-empty
cp $OUTPUT_FILE $OUTPUT_FILE_2 # preserving original $OUTPUT_FILE content
strace -e fault=openat:error=$ERR_NAME -P $OUTPUT_FILE $BINARY $INPUT_FILE $OUTPUT_FILE 2>&1 | grep -E "\+\+\+|$BINARY:" | cmp $REFERENCE || exit 1
test -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1
cmp $OUTPUT_FILE $OUTPUT_FILE_2 || exit 1
