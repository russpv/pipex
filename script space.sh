#!/bin/bash

#read from stdin
echo "hey text\n"

while read line
do
  echo "$line"
done

echo "Some more text"
