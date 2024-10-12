#! /usr/bin/env bash
lastword=""

while read -r line;
do
    # Remove trailing whitespace
    line=$(echo "$line" | sed 's/[[:space:]]*$//')
    # Check if the line is not the same as the last word
    #because the array was sorted this algorithm works
    if [[ "$line" != "$lastword" ]]; then
        echo "$line"
        lastword="$line"
    fi
done
