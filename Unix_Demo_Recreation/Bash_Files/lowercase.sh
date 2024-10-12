#! /usr/bin/env bash
#tr can use [:upper:] and [:lower:] for upper and lowercase
while IFS= read -r line; do
#print the line using tr from upper to lower
    echo "$line" | tr '[:upper:]' '[:lower:]'
done