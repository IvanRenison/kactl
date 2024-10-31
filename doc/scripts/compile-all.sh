#!/usr/bin/env bash
DIR=${1:-.}

# use a precompiled header for the template to improve perf
g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++20 -x c++-header $DIR/content/contest/template.cpp
trap "rm -f $DIR/content/contest/template.cpp.gch" EXIT

SCRIPT_DIR=$DIR/doc/scripts
tests="$(find $DIR/content -name '*.h' | grep -vFf $SCRIPT_DIR/skip_headers)"
echo "skipped: "
find $DIR/content -name '*.h' | grep -Ff $SCRIPT_DIR/skip_headers

# Create a temporary directory for test outputs
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

# Function to run a single test
run_test() {
    local test=$1
    local outfile=$2
    local test_tmpdir="$TMPDIR/$(basename "$test")"

    # Create temporary directory for this test
    mkdir -p "$test_tmpdir"

    # Compile the test, capturing output
    {
        echo "$(basename $test): "
        $SCRIPT_DIR/test-compiles.sh "$test" "$test_tmpdir"
        echo $? > "$outfile.retcode"
        echo
    } &> "$outfile.log"

    # Cleanup test directory
    rm -rf "$test_tmpdir"
}

# Run tests in parallel with a maximum of N jobs (N = number of CPU cores)
N=$(nproc)
echo "Running compilation tests using $N parallel jobs..."

# Process tests in batches
count=0
for test in $tests; do
    outfile="$TMPDIR/$(basename $test)"
    run_test "$test" "$outfile" &

    # Limit number of parallel jobs
    ((++count % N == 0)) && wait
done
wait # Wait for remaining jobs

# Process results
declare -i pass=0
declare -i fail=0
failHeaders=""

for test in $tests; do
    outfile="$TMPDIR/$(basename $test)"
    retcode=$(<"$outfile.retcode")
    cat "$outfile.log"

    if (($retcode != 0)); then
        fail+=1
        failHeaders="$failHeaders$test\n"
    else
        pass+=1
    fi
done

total=$((pass + fail))
echo "$pass/$total tests passed"
if (($pass == 0)); then
    echo "No tests found (make sure skip_headers doesn't have whitespace lines)"
    exit 1
elif (($fail == 0)); then
    echo "No tests failed"
    exit 0
else
    echo -e "These tests failed: \n $failHeaders"
    exit 1
fi
