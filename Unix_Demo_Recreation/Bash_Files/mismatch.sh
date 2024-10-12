#! /usr/bin/env bash

file="unix_dict.text"
# Read words from standard input
while read -r word; do
    # Check if the word exists in the file
    if ! grep -qw "$word" "$file"; then
        #wow this was so much easier than the c version
        echo "$word"
    fi
done