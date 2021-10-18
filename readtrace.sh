#!/bin/sh

if test ! -f "$1"
then
    echo "Error: executable $1 does not exist."
    exit 1
fi
if test ! -f "$2"
then
    echo "Error: trace log $2 does not exist."
    exit 1
fi

EXECUTABLE="$1"
TRACELOG="$2"
while read LINETYPE FADDR CADDR CTIME; do
    FNAME="$(addr2line -f -e ${EXECUTABLE} ${FADDR} | head -1 | c++filt)"
    #CDATE="$(date -Iseconds -d @${CTIME})"
    if test "${LINETYPE}" = "e"; then
        FLINE="$(addr2line -s -e ${EXECUTABLE} ${FADDR})"
        #CALLER="$(addr2line -f -e ${EXECUTABLE} ${CADDR} | head -1 | c++filt)"
        #echo "Enter ${FNAME} at ${CDATE}, called from ${CALLER} (${FLINE})"
        echo "Enter ${FLINE} : ${FNAME}"
    fi
    if test "${LINETYPE}" = "x"; then
        #echo "Exit  ${FNAME} at ${CDATE}"
        echo "Exit  ${FNAME}"
    fi
done < "${TRACELOG}"
