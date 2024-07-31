#!/bin/bash

# Define the Makefile name
MAKEFILE="Makefile"

# Extract the SOURCES list from the Makefile
SOURCE_LIST=$(grep '^SOURCES =' "$MAKEFILE" | sed -e 's/^SOURCES = //' -e 's/\\//g' -e 's/ \+/ /g')

# Convert SOURCE_LIST to an array, removing extra spaces
IFS=' ' read -r -a SOURCE_ARRAY <<< "$SOURCE_LIST"

# List *.c files in the current directory
CURRENT_FILES=$(ls *.c 2>/dev/null)

# Convert CURRENT_FILES to an array
IFS=$'\n' read -r -d '' -a CURRENT_FILES_ARRAY <<< "$CURRENT_FILES"

# Sort and compare
SORTED_SOURCES=$(printf "%s\n" "${SOURCE_ARRAY[@]}" | sort)
SORTED_CURRENT_FILES=$(printf "%s\n" "${CURRENT_FILES_ARRAY[@]}" | sort)

# Check for files in SOURCE list that are not in the current directory
echo "Checking for missing files..."
for src_file in $(echo "$SORTED_SOURCES"); do
    if ! echo "$SORTED_CURRENT_FILES" | grep -q "^$src_file$"; then
        echo "Missing source file: $src_file"
    fi
done

# Check for files in the current directory that are not in the SOURCE list
echo "Checking for extra files..."
for current_file in $(echo "$SORTED_CURRENT_FILES"); do
    if ! echo "$SORTED_SOURCES" | grep -q "^$current_file$"; then
        echo "Extra source file: $current_file"
    fi
done

echo "Comparison complete."

