#!/bin/bash

# Default
CONCURRENCY=4
REQUESTS=100

# CLI and config
if [ $1 ]; then
  if [ $1 == "--help" ] || [ $1 == "-h" ]; then
    echo "Usage: ./stress-test [REQUESTS] [CONCURRENCY]"
    exit
  else
    REQUESTS=$1
  fi
fi

if [ $2 ]; then
  CONCURRENCY=$2
fi


for i in `seq 1 $CONCURRENCY`; do
  curl -s "http://localhost:8080/?[1-$REQUESTS]" & pidlist="$pidlist $!"
done

# Execute and wait
FAIL=0
for job in $pidlist; do
  echo $job
  wait $job || let "FAIL += 1"
done

# Verify if any failed
if [ "$FAIL" == "0" ]; then
  echo "SUCCESS!"
else
  echo "FAIL! ($FAIL)"
fi
