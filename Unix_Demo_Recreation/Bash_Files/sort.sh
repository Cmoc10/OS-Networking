#!/usr/bin/env bash

# Function to sort words alphabetically using bubble sort
bubble_sort() {
    #make an array variable with all inputs
    local array=("$@")
    #get the length of the array
    local n=${#array[@]}
    # make a temp array
    local temp

    # Bubble sort algorithm
    for ((i = 0; i < n; i++)); do
        for ((j = 0; j < n - i - 1; j++)); do
        #compare the two words if the first is greater than the second
            if [[ "${array[j]}" > "${array[j+1]}" ]]; then
                # Swap the elements
                temp="${array[j]}"
                array[j]="${array[j+1]}"
                array[j+1]="$temp"
            fi
        done
    done

    # Print sorted array
    for word in "${array[@]}"; do
        echo "$word"
    done
}

# Read words from standard input into an array
readarray -t words

# Call the bubble_sort function with the array of words
bubble_sort "${words[@]}"
