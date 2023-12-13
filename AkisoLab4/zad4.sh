#!/bin/bash

# set -x
set -u
set -e

sort_by_value_map() {
    local -n mm=$1  # Reference to memory_map

    # Create an array of keys sorted by corresponding values
    sorted_keys=($(for key in "${!mm[@]}"; do
        echo "$key ${mm[$key]}"
    done | sort -k2,2nr | cut -d' ' -f1))

    # Assign the sorted keys to sorted_memory_map
    sorted_memory_map=("${sorted_keys[@]}")

    echo "${sorted_memory_map[@]}"
}

echo_files_by_hashes() {
    local -n common_list=$1  # Reference to the sorted memory list
    local -n hm_reference=$2  # Reference to the hash_map
    count=$(echo "$common_list" | wc -w)
    echo "Number of words: $count"


    for ((i=1; i<=$count; i++)); do
        echo -e "\n"
        temp_hash=$(echo "$common_list" | awk -v j="$i" '{print $j}')

        for file in "${!hash_map[@]}"; do
            if [ "${hash_map["$file"]}" == "$temp_hash" ]; then
                echo "$file"
            fi
        done
    done
}

# Check if a directory is provided as a parameter count params
if [ ! $# -eq 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

directory=$1
declare -A hash_map

# Check if the provided path is a directory
if [ ! -d "$directory" ]; then
    echo "Error: '$directory' is not a directory."
    exit 1
fi

# Use find and while loop to iterate through files in the directory
while IFS= read -r file; do
    # Calculate SHA256 checksum and then we have hash || " " || folder so
    hash_sum=$(sha256sum "$file" | awk '{print $1}')

    # Save result in hash map with only the file name as the key
    file_name=$(realpath "$file")
    hash_map["$file_name"]=$hash_sum  # Assign the calculated hash_sum to file_name

done < <(find "$directory" -type f)

for key in "${!hash_map[@]}"; do
    echo "Key: $key, Hash: ${hash_map[$key]}"
done

declare -A memory_map

# Iterate through hash_map
for file in "${!hash_map[@]}"; do
    temphash=${hash_map["$file"]}
    file_size=$(stat -c %s "$file")
    memory_map["$temphash"]=$file_size
done

# Print the contents of memory_map
for key in "${!memory_map[@]}"; do
    value="${memory_map[$key]}"
    echo "Key: $key, Value: $value"
done

sorted_memory_map=$(sort_by_value_map memory_map)
echo "Sorted Memory Map:"
echo "$sorted_memory_map"

echo "Sorted files:"
echo_files_by_hashes sorted_memory_map hash_map
