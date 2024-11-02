#!/usr/bin/env bash
DIR=${1:-.}
GROUP=${2:-}  # Optional group parameter

# use a precompiled header for the template to improve perf
g++ -Wall -Wfatal-errors -Wconversion -std=c++20 -O2 $DIR/stress-tests/utilities/template.h
trap "rm -f $DIR/stress-tests/utilities/template.h.gch" EXIT

# If GROUP is provided, search in that subdirectory, otherwise search all
if [ -n "$GROUP" ]; then
    tests="$(find $DIR/stress-tests/$GROUP -name '*.cpp')"
else
    tests="$(find $DIR/stress-tests -name '*.cpp')"
fi

declare -i total=$(echo "$tests" | wc -l)
declare -i pass=0
declare -i fail=0
failTests=""

# Create a temporary directory for test outputs
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

# Function to run a single test
run_test() {
    local test=$1
    local outfile=$2
    local start=$(date +%s.%N)

    # Compile and run the test, capturing all output
    {
        echo "$(basename $test): "
        g++ -Wall -Wfatal-errors -Wconversion -std=c++20 -O2 "$test" -o "$TMPDIR/$(basename $test).out" && \
        ulimit -s 524288 && "$TMPDIR/$(basename $test).out"
        echo $? > "$outfile.retcode"

        local end=$(date +%s.%N)
        local runtime=$(echo "$end - $start" | bc -l)
        echo "Took $runtime seconds"
        echo
    } &> "$outfile.log"
}

# Run tests in parallel with jobs from make
N=${JOBS:-1}
if [ -n "$GROUP" ]; then
    echo "Running $GROUP tests using $N parallel jobs..."
else
    echo "Running all tests using $N parallel jobs..."
fi

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
for test in $tests; do
    outfile="$TMPDIR/$(basename $test)"
    retcode=$(<"$outfile.retcode")
    cat "$outfile.log"

    if (($retcode != 0)); then
        fail+=1
        failTests="$failTests$test\n"
    else
        pass+=1
    fi
    rm -f "$TMPDIR/$(basename $test).out"
done

echo "$pass/$total tests passed"
if (($fail == 0)); then
    echo "No tests failed"
    exit 0
else
    echo -e "These tests failed: \n $failTests"
    exit 1
fi
