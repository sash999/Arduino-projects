#!/bin/bash
[[ $# -ne 1 ]] && echo "Filename to operate on is required!!" && exit 99
FNAME=$1
cat $FNAME | sed 's/
mv /tmp/remcrlf.tmp $FNAME

