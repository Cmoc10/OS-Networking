#! /usr/bin/env bash
# tr command accepts sets of characters and replaces them with different sets
while IFS= read -r line; do
# print the line using tr from space to newline
    echo "$line" | tr ' ' '\n'
done