#!/bin/bash

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NONE='\033[0m' # No color

# Compile
make all
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed${NONE}"
    exit 1
fi

echo "Starting tests..."
echo ""

test_inputs=()
test_outputs=()

# Read inputs and expected outputs from TESTS.md
while IFS= read -r line
do
    # Ignore blank lines and headers (lines beginning with #)
    if [[ -z "$line" || "$line" == \#* ]]; then
        # echo "ignore: $line"
        continue
    fi

    # Split line on '|' and trim leading and trailing whitespace
    read_in=$(echo "$line" | cut -d '|' -f 1 | sed 's/[[:space:]]*$//')
    read_out=$(echo "$line" | cut -d '|' -f 2 | sed 's/^[[:space:]]*//')
    
    # echo "Input: '$read_in', output: '$read_out'"

    test_inputs+=("$read_in")
    test_outputs+=("$read_out")

done < TESTS.md

# Run program with test inputs
output=$(./memoize -q <<< "$(printf "%s\n" "${test_inputs[@]}")")

# Get each output
IFS=$'\n' read -r -d '' -a output_lines <<< "$output"

failed_tests=0

# Print test results
for i in "${!test_outputs[@]}"; do
    echo "Test: ${test_inputs[i]}"
    echo "Expected: ${test_outputs[i]}"
    echo "Received: ${output_lines[i]}"
    if [ "${output_lines[i]}" != "${test_outputs[i]}" ]; then
        ((failed_tests++))
        echo -e "${RED}TEST FAILED${NONE}"
    else
        echo -e "${GREEN}TEST PASSED${NONE}"
    fi
    echo ""
done

if [ $failed_tests -eq 0 ]; then
    echo "All tests passed"
else
    echo "$failed_tests test(s) failed"
fi
