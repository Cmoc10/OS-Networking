#!/usr/bin/env bash

### compile all c code in the current directory

# Function to find local .h includes in a file (only those in "")
find_local_h_includes() {
    local file="$1"
    # grep to find #include and then look for strings that end in .h
    # cut takes out the part between the quotes 
    # sed changes the extension of the .h file to be the .c version
    local includes=$(grep '#include *".*\.h"' "$file" | cut -d'"' -f2 | sed 's/\.h$/.c/')
    #if no includes were found return
    if [ -z "$includes" ]; then
        return 0
    fi

    echo "$includes"
}

# Get current directory
location=$(pwd)

### checks for files that end in .c
if ls *.c &> /dev/null; then
    files=$(ls *.c)
    for file in $files
    do
        dependencies=$(find_local_h_includes "$file")
        if [ -n "$dependencies" ];
        then
            gcc -Wall -Werror -o "${file%.c}" "$file" "$dependencies"
        else
            gcc -Wall -Werror -o "${file%.c}" "$file"
        fi
    done

else
    >&2 echo "Oops, we found no C code in the directory: $location"
fi

### how do you get the current directory in bash?

### how do you store the output of variables in bash?