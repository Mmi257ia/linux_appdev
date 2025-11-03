# to be used via source

exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $TMP_FILE
    exit $EXIT_STATUS
}

trap exit_handler EXIT HUP INT QUIT PIPE TERM

RHASHER=./rhasher
TMP_FILE=`mktemp`

# test hashing a file
# $1 is reference program, $2 is hash identifier for rhasher, $3 is 'base64' or 'hex'
test_file() {
    TEST_FILE=src/$RHASHER.c
    if [ $1 != "tthsum" ]; then
        if [ $3 = "base64" ]; then
            $1 $TEST_FILE | grep -Eo "^[^ ]*" | xxd -r -p | base64 > $TMP_FILE
        else
            $1 $TEST_FILE | grep -Eo "^[^ ]*" > $TMP_FILE
        fi
    else
        if [ $3 = "base64" ]; then
            $1 $TEST_FILE | grep -Eo "^[^ ]*" | base32 -d | base64 > $TMP_FILE
        else
            $1 $TEST_FILE | grep -Eo "^[^ ]*" | base32 -d | xxd -p > $TMP_FILE
        fi
    fi
    echo "$2 $TEST_FILE" | $RHASHER 2>&1 | grep -Eo "[^ >]*" | tail -n 1 | cmp $TMP_FILE || exit $?
}

# test hashing a string
# $1 is reference program, $2 is hash identifier for rhasher, $3 is 'base64' or 'hex'
test_string() {
    TEST_STRING=loremipsumdolorsitamet
    if [ $1 != "tthsum" ]; then
        if [ $3 = "base64" ]; then
            echo -n "$TEST_STRING" | $1 | grep -Eo "^[^ ]*" | xxd -r -p | base64 > $TMP_FILE
        else
            echo -n "$TEST_STRING" | $1 | grep -Eo "^[^ ]*" > $TMP_FILE
        fi
    else
        if [ $3 = "base64" ]; then
            echo -n "$TEST_STRING" | $1 | grep -Eo "^[^ ]*" | base32 -d | base64 > $TMP_FILE
        else
            echo -n "$TEST_STRING" | $1 | grep -Eo "^[^ ]*" | base32 -d | xxd -p > $TMP_FILE
        fi
    fi
    echo "$2 \"$TEST_STRING" | $RHASHER 2>&1 | grep -Eo "[^ >]*" | tail -n 1 | cmp $TMP_FILE || exit $?
}
