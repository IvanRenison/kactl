#!/usr/bin/env bash

# Remove 'set -e' to prevent premature exit
# set -e

# Create a temporary directory for test outputs
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

# Number of parallel jobs
N=$(nproc)
echo "Running tests using $N parallel jobs..."

# Function to test a single problem
test_test_problem() {
    local file="$1"
    local outfile="$2"
    local start=$(date +%s.%N)

    {
        echo "$(basename "$file"): "
        # Compile the test file
        g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++20 "$file" -o "$TMPDIR/$(basename "$file").out"
        compile_exit_code=$?
        if [ $compile_exit_code -ne 0 ]; then
            echo "Compilation failed with exit code $compile_exit_code"
            echo $compile_exit_code > "$outfile.retcode"
        else
            # Run the Python script
            python3 ./doc/scripts/checkTestProblems.py "$file"
            script_exit_code=$?
            echo $script_exit_code > "$outfile.retcode"
        fi

        local end=$(date +%s.%N)
        local runtime=$(echo "$end - $start" | bc -l)
        echo "Took $runtime seconds"
        echo
    } &> "$outfile.log"
}

pass=0
fail=0
failTests=""

# Counter for parallel jobs
pids=()

# Run tests in parallel
for file in ./test-problems/*/*.cpp; do
    outfile="$TMPDIR/$(basename "$file")"
    test_test_problem "$file" "$outfile" &
    pids+=($!)

    # Wait if the number of background jobs reaches the limit
    if [ ${#pids[@]} -ge $N ]; then
        wait -n
        # Remove completed jobs from the list
        pids=($(jobs -pr))
    fi
done

# Wait for all background jobs to finish
wait

# Process results
for file in ./test-problems/*/*.cpp; do
    outfile="$TMPDIR/$(basename "$file")"
    retcode=$(<"$outfile.retcode")
    cat "$outfile.log"

    if [ "$retcode" != "0" ]; then
        fail=$((fail + 1))
        failTests="$failTests$file\n"
    else
        pass=$((pass + 1))
    fi
    rm -f "$TMPDIR/$(basename "$file").out"
done

total=$((pass + fail))
echo "$pass/$total tests passed"
if [ "$fail" -eq 0 ]; then
    echo "No tests failed"
    exit 0
else
    echo -e "These tests failed: \n$failTests"
    exit 1
fi
