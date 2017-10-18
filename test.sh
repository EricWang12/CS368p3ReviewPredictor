#!/bin/bash

# Global constants
readonly DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
readonly LIB_DIR="/p/course/cs368-isung/public/libraries"
readonly GTEST_INCLUDE="$LIB_DIR/googletest/googletest/include/"
readonly INPUT_DIR="/u/i/s/isung/public/html/cs368/a3"
readonly INPUT_FILES=( \
    "a3.hpp" \
    "commonwords.txt" \
    "main.cpp" \
    "rawReviewRatings.txt" \
    "rawReviews.txt" \
    "test.c" \
    "test.sh" \
)

readonly CPP_FILES=("a3.cpp")
readonly VALID_ARGS=("CleanDataTest" "FillDictionaryTest" "RateReviewsTest" "EndToEndTest")
#readonly VALID_ARGS=("FillDictionaryTest" "RateReviewsTest")

trap "cleanup; exit 1" SIGINT

# Cleans up intermediate files
cleanup() {
    rm -rf ./a3test ./testCleanReviewRatings.txt ./testCleanReviews.txt ./testRatings.txt
}

# Exits if the necessary files do not exist, or if unnecessary files exist.
check_files_exist() {
    local flag=0
    if [ ! -f "./a3.cpp" ]; then
        (>&2 echo "Error: could not find a3.cpp")
        flag=1
    fi

    local f
    for f in "${INPUT_FILES[@]}"; do
        if [ ! -f "./$f" ]; then
            (>&2 echo "Error: could not find $f")
            flag=1
        fi
    done

    if ((flag == 1)); then
        (>&2 echo "Please fix the above errors, exiting")
        exit 1
    fi
}

# Prints warnings if test files have been changed.
check_files_unchanged() {
    local cmp_msg
    local f
    for f in "${INPUT_FILES[@]}"; do
        cmp_msg=$(cmp "./$f" "$INPUT_DIR/$f" 2>&1)
        if (($? != 0)); then
            (>&2 echo "Warning: $cmp_msg")
        fi
    done
}

# Greps through the code files, and exits or warns if anything is wrong.
check_code_text() {
    local mutable_count=$(grep -ow "mutable" ./a3.cpp | wc -l)
    local volatile_count=$(grep -ow "volatile" ./a3.cpp | wc -l)
    local flag=0
    if ((mutable_count > 0)); then
        (>&2 echo "Error: there are more than 0 occurrences of the word \"mutable\" in a3.cpp")
        flag=1
    fi
    if ((volatile_count > 0)); then
        (>&2 echo "Error; there are more than 0 occurrences of the word \"volatile\" in a3.cpp")
        flag=1
    fi
    if ((flag == 1)); then
        (>&2 echo "Please fix the above errors, exiting")
        exit 1
    fi

    local include_string="#include"
    local include_count=$(grep -ow "$include_string" ./a3.cpp | wc -l)
    if ((include_count < 8)); then
        (>&2 echo "Warning: there are fewer than 8 occurrences of \"$include_string\" in a3.cpp")
    fi
    if ((include_count > 8)); then
        (>&2 echo "Warning: there are more than 8 occurrences of \"$include_string\" in a3.cpp")
    fi
    cmp <(grep -v "^//" "$INPUT_DIR/a3.cpp" | head -n11) <(grep -v "^//" ./a3.cpp | head -n11 | tr -d "\r") > /dev/null 2>&1
    if (($? != 0)); then
        (>&2 echo "Warning: changes to first 11 lines of code (ignoring the file comment) in a3.cpp")
    fi
}

# Returns whether the name of the argument is a test case or not.
# @param $1: the name of a possible test case
# @return 0 if $1 is a test case, or 1 otherwise
is_valid_arg() {
    local arg
    for arg in "${VALID_ARGS[@]}"; do
        if [ "$1" == "$arg" ]; then
            return 0
        fi
    done
    return 1
}

# Runs the test case specified by the argument.
# @param $1: the name of the test case
# @return 0 if the test passes, or non-zero if it fails
run_test() {
    local test_status
    ./a3test --gtest_filter="A3Test.$1"
    test_status=$?
    rm -rf ./testCleanReviewRatings.txt ./testCleanReviews.txt ./testRatings.txt
    return $test_status
}

main() {
    cd "$DIR"

    if (($# == 0)); then
        (>&2 echo "Warning: no arguments specified.")
        (>&2 echo "USAGE: ./test.sh <TEST1> <TEST2> ...")
        (>&2 echo "Only performing preliminary checks.")
    fi

    check_files_exist

    check_files_unchanged

    check_code_text

    # Compile code into ./a3test
    g++ -std=c++11 -isystem "$GTEST_INCLUDE" -pthread "${CPP_FILES[@]/#/.\/}" ./test.c "$LIB_DIR/libgtest.a" -o ./a3test
    if (($? != 0)); then
        (>&2 echo "Error: could not compile a3.cpp, exiting")
        exit 1
    fi

    # Run tests
    local passed_test_count=0
    local test_count=0
    local arg
    for arg in "$@"; do
        is_valid_arg "$arg"
        if (($? != 0)); then
            echo "$arg is not a valid test, ignoring"
            continue
        fi
        run_test "$arg"
        if (($? == 0)); then
            ((passed_test_count += 1))
        fi
        ((test_count += 1))
        echo
    done
    cleanup

    # Success (possibly)
    echo "$passed_test_count/$test_count tests passed"
    if ((passed_test_count < test_count)); then
        exit 1
    else
        return 0
    fi
}

main "$@"
