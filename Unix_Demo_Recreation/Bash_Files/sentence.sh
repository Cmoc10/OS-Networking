#!/usr/bin/env bash
filename="unix_sentence.text"
# Read lines from the file 
while IFS= read -r line || [[ -n "$line" ]]
do
    #print the line from the file
    echo "$line"
done < "$filename"