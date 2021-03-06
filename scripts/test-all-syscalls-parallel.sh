#!/bin/bash

if [ ! -d tmp ]; then
  mkdir tmp
fi
chmod 755 tmp
cd tmp

while [ 1 ];
do
  for syscall in $(../trinity -L | grep entrypoint | grep -v AVOID | awk '{ print $4 }' | sort -u)
  do
	MALLOC_CHECK_=2 ../trinity -q -c $syscall -D &
  done
  wait
done
