#!/bin/sh

BLUE="\e[0;34m"
GREEN="\e[0;32m"
WHITE="\e[0m"

for suite in $(ls testsuite); do
    if ! g++ testsuite/$suite/main.cpp -I.. -o test; then
        echo "Compilation of $suite failed."
        return 1
    fi
    echo -e ${BLUE}"===Suite $suite==="${WHITE}
    for testcase in $(ls testsuite/$suite/cases); do
        casepath=testsuite/$suite/cases/$testcase
        ./test < $casepath/input > output 2> error

        exit_code="$?"
        expected_code=$(cat $casepath/code)
        if [ "$exit_code" != "$expected_code" ]; then
            echo "Wrong exit code for $suite/$testcase. Expected $expected_code, got $exit_code."
            echo "stderr:"
            cat error

            rm test output error
            return 1
        fi
        if ! cmp output $casepath/output; then
            echo "Incorrect output for $suite/$testcase. Expected:"
            cat $casepath/output
            echo "But got:"
            cat output
            echo "stderr:"
            cat error

            rm test output error
            return 1
        fi
        echo -e "$suite/$testcase" ${GREEN}"passed."${WHITE}
    done
done

rm test output error
echo -e ${GREEN}"All testcases passed."${WHITE}
