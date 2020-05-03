#!/bin/bash

usage() {
cat <<EOF   
usage: $0 <command> <list-of-primes>
EOF
}

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
