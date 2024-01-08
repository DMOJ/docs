#!/bin/sh

BLUE="\e[0;34m"
GREEN="\e[0;32m"
RESET="\e[0m"

for suite in testsuite/*; do
    if ! g++ "$suite/main.cpp" -I.. -o test; then
        echo "Compilation of $suite failed."
        return 1
    fi
    echo -e "${BLUE}===Suite $suite===${RESET}"
    for testcase in "$suite"/cases/*; do
        name="$(basename "$suite")/$(basename "$testcase")"
        ./test < "$testcase/input" > output 2> error

        exit_code="$?"
        expected_code="$(cat "$testcase/code")"
        if [ "$exit_code" != "$expected_code" ]; then
            echo "Wrong exit code for $name. Expected $expected_code, got $exit_code."
            echo "stderr:"
            cat error

            rm test output error
            return 1
        fi
        if ! cmp output "$testcase/output"; then
            echo "Incorrect output for $name. Expected:"
            cat "$testcase/output"
            echo "But got:"
            cat output
            echo "stderr:"
            cat error

            rm test output error
            return 1
        fi
        echo -e "$name" "${GREEN}passed.${RESET}"
    done
done

rm test output error
echo -e "${GREEN}All testcases passed.${RESET}"
