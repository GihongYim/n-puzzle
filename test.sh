#!/bin/bash

GEN=./npuzzle-gen.py
SOLVER=./n_puzzle
TESTS=10
SIZE=3

for ((i=1; i<=TESTS; i++)); do
    echo "============================="
    echo "Generating test $i"

    TMPFILE=$(mktemp)

    # 퍼즐 생성
    python3 $GEN $SIZE > $TMPFILE

    echo "Initial puzzle:"
    cat $TMPFILE | sed 's/^/   /'

    echo
    echo "Running solver on test $i"

    OUTPUT=$($SOLVER $TMPFILE)

    if echo "$OUTPUT" | grep -q "Goal reached"; then
        echo "Result: ✅ VALID (solvable)"
    else
        echo "Result: ❌ INVALID (unsolvable)"
    fi

    echo "Solver output:"
    echo "$OUTPUT" | sed 's/^/   /'

    rm $TMPFILE
    echo
done
