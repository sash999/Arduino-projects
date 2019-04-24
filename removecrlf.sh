#!/bin/bash
[[ $# -ne 1 ]] && echo "Filename to operate on is required!!" && exit 99
FNAME=$1
cat $FNAME | sed 's///g' > /tmp/remcrlf.tmp
mv /tmp/remcrlf.tmp $FNAME


