#!/bin/sh

# \n and space in UCS2 encoding
UCS2_NEWLINE=`printf "\n" | iconv -t UCS2 | xxd -p`
UCS2_SPACE=`printf " " | iconv -t UCS2 | xxd -p`

# Less temporary files could be used (zero, actually),
# but this way it is more understandable
exit_handler() {
  trap - EXIT
  test -r "$TMPFILE" && rm -f "$TMPFILE"
  test -r "$TMPFILE_ANNOTATED" && rm -f "$TMPFILE_ANNOTATED"
  test -r "$TMPFILE_SHUFFLED" && rm -f "$TMPFILE_SHUFFLED"
}

trap exit_handler EXIT HUP INT QUIT PIPE TERM

# Print an UCS2 symbol at specified location in terminal
# $1: hex pattern, $2: y coord, $3: x coord 
print_symbol_at() {
    tput cup $2 $3
    echo -n $1 | xxd -r -p | iconv -f UCS2
}

if [ $# -ge 1 ]; then
    DELAY=$1
else
    DELAY=0.01
fi

TMPFILE=`mktemp`
TMPFILE_ANNOTATED=`mktemp`
TMPFILE_SHUFFLED=`mktemp`

# Read stdin, convert it to UCS2, dump it to hex,
# split by 2 bytes with spaces, then finally write it to $TMPFILE
cat | iconv -t UCS2 | xxd -p -c 0 | sed 's/\([0-9A-Fa-f]\{4\}\)/& /g' > $TMPFILE

# Annotate all symbols with their Y and X position in input,
# split it so one symbol takes one line, write result to $TMPFILE_ANNOTATED
X=0
Y=0
for SYMBOL in $(cat $TMPFILE); do
    if [ $SYMBOL != $UCS2_NEWLINE ]; then  # not a newline
        if [ $SYMBOL != $UCS2_SPACE ]; then  # not a space
            echo $SYMBOL $Y $X >> $TMPFILE_ANNOTATED
        fi
        X=$((X+1))
    else
        Y=$((Y+1))
        X=0
    fi
done

# Randomly shuffle all symbol entries
shuf $TMPFILE_ANNOTATED -o $TMPFILE_SHUFFLED

clear
# Write symbol by symbol
while read SYMBOL; do
    print_symbol_at $SYMBOL
    sleep $DELAY
done < $TMPFILE_SHUFFLED

# Return cursor to the end of the output
tput cup $Y $X

# The best test input:
# cowthink -p У коровы нет других забот, ест траву и молоко даёт. Для кого старается она? Для меня, для меня, для меня!
