#!/bin/bash

# Check if at least one argument is provided
if [ "$#" -eq 0 ]; then
    echo "Usage: $0 num1 num2 num3 ..."
    exit 1
fi

# Convert input arguments into an array format with floats
output="["
for num in "$@"; do
    # Ensure float format (add .0 if it's an integer)
    if [[ "$num" =~ ^-?[0-9]+$ ]]; then
        num="${num}.0"
    fi
    output+="$num,"
done

# Remove the trailing comma and close the array
output="${output%,}]"

# Print the formatted array
echo "$output"
