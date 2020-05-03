#!/bin/bash

usage() {
cat <<EOF   
usage: $0 <command> <list-of-primes>
EOF
}

THREADS=1

while getopts "j:h" OPTION; do
    case $OPTION in
	j)
	    THREADS="$OPTARG"
	    ;;
	h)
	    usage
	    exit 0
	    ;;
	"?")
	    usage
	    exit 1
	    ;;
    esac
done

shift $((OPTIND-1))

if [[ $# -ne 2 ]]; then
    usage
    exit 1
fi

COMMAND="$1"
FILE="$2"

PRIMES=$(cat $FILE)

EXITNO=0

N=2


for PRIME in $PRIMES; do
    if [ -z ${PRIME//[[:space:]]/} ]; then
	continue
    fi
    while [[ $N -lt $PRIME ]]; do
	# expect compisite
	if [ "$($COMMAND $N)" == "prime" ]; then
	    echo "$N: false prime"
	    EXITNO=1
	fi
	(( ++N ))
    done
    if [ "$($COMMAND $N)" == "composite" ]; then
	echo "$N: false composite"
	EXITNO=1
    fi
    (( ++N ))
done

exit $EXITNO
