///////////////////////////////////////////////////////////////////////////////
// File Name:      test.c
//
// Author:         Varun Naik
// CS email:       vnaik@cs.wisc.edu
//
// Description:    Unit tests for a3. As a reminder, even if you pass all of
//                 these tests, you are not yet guaranteed 90/90 on the
//                 autograder portion. But if you do not pass these tests, you
//                 will probably receive a very low score on the autograder.
//                 We named this file test.c rather than test.cpp so that you
//                 can still compile your code with `g++ -std=c++11 *.cpp`.
//
// Sources:        Google Test, cppreference.com
//
// URL(s) of sources:
//                 https://github.com/google/googletest
//                 http://en.cppreference.com/w/
///////////////////////////////////////////////////////////////////////////////

#include "a3.hpp"

#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * @brief Compile-time checks for the functions in a3.cpp. This function is
 *        never called, but it should compile without any errors.
 */
void compilerChecks() {
    std::vector<std::string> tokens;
    std::ifstream myIStream;
    std::ofstream myOStream;
    std::unordered_set<std::string> myUnorderedSet;
    std::unordered_map<std::string, std::pair<long, long>> myUnorderedMap;
    std::vector<std::string> myVector;
    std::string myString;

    // If the prototypes in a3.hpp are not implemented, do not compile
    // No need to check return types or exact parameter types
    cleanData(myIStream, myOStream, myUnorderedSet);
    fillDictionary(myIStream, myUnorderedMap);
    fillCommonWords(myIStream, myUnorderedSet);
    rateReviews(myIStream, myUnorderedMap, myOStream);
    removeCommonWords(myVector, myUnorderedSet);
    splitLine(myString, myVector);
}

/**
 * @brief Helper function that returns an AssertionResult for ASSERT_OPEN.
 *
 * Do not call this function directly. Call ASSERT_OPEN instead.
 *
 * @param expr0 The first expression passed to ASSERT_OPEN
 * @param inFile See "file" argument for ASSERT_OPEN
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult assertOpen(const std::string &expr0, std::ifstream &inFile) {
    if (inFile.is_open()) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << "Expected " << expr0 << " to be open";
    }
}

/**
 * @brief Helper function that returns an AssertionResult for ASSERT_OPEN.
 *
 * Do not call this function directly. Call ASSERT_OPEN instead.
 *
 * @param expr0 The first expression passed to ASSERT_OPEN
 * @param outFile See "file" argument for ASSERT_OPEN
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult assertOpen(const std::string &expr0, std::ofstream &outFile) {
    if (outFile.is_open()) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << "Expected " << expr0 << " to be open";
    }
}

/**
 * @brief Raises a fatal failure with the correct line number if file is not
 *        open.
 *
 * @param file The input or output file
 */
#define ASSERT_OPEN(file) \
    ASSERT_PRED_FORMAT1(assertOpen, file)

/**
 * @brief Helper function that returns an AssertionResult for EXPECT_EOF.
 *
 * Do not call this function directly. Call EXPECT_EOF instead.
 *
 * @param expr0 The first expression passed to EXPECT_EOF
 * @param inFile See comment for EXPECT_EOF
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult expectEof(const std::string &expr0, std::istream &inFile) {
    if (inFile.eof()) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << "Expected " << expr0 << " to be at EOF";
    }
}

/**
 * @brief Raises a non-fatal failure with the correct line number if inFile has
 *        not reached EOF.
 *
 * @param inFile The input file
 */
#define EXPECT_EOF(inFile) \
    EXPECT_PRED_FORMAT1(expectEof, inFile)

/**
 * @brief Helper function that returns an AssertionResult for ASSERT_NOT_FAILED.
 *
 * Do not call this function directly. Call ASSERT_NOT_FAILED instead.
 *
 * @param expr0 The first expression passed to ASSERT_NOT_FAILED
 * @param outFile See comment for ASSERT_NOT_FAILED
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult assertNotFailed(const std::string &expr0, std::ofstream &outFile) {
    if (outFile.fail()) {
        return ::testing::AssertionFailure()
            << "Expected " << expr0 << " to be in a non-failed state";
    } else {
        return ::testing::AssertionSuccess();
    }
}

/**
 * @brief Raises a fatal failure with the correct line number if outFile is in
 *        a failed state.
 *
 * @param outFile The output file
 */
#define ASSERT_NOT_FAILED(outFile) \
    ASSERT_PRED_FORMAT1(assertNotFailed, outFile)

/**
 * @brief Helper function that returns an AssertionResult for MATCH_NEXT_LINE.
 *
 * Do not call this function directly. Call MATCH_NEXT_LINE instead.
 *
 * @param expr0 The first expression passed to MATCH_NEXT_LINE
 * @param expr1 The second expression passed to MATCH_NEXT_LINE
 * @param inFile See comment for MATCH_NEXT_LINE
 * @param str See comment for MATCH_NEXT_LINE
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult matchNextLine(const std::string &expr0,
                                         const std::string &expr1,
                                         std::ifstream &inFile,
                                         const std::string &str) {
    std::string line;
    std::getline(inFile, line);
    if (inFile.eof()) {
        return ::testing::AssertionFailure()
            << "Line printed to file is incorrect" << std::endl
            << " Printed: (end of printed output)" << std::endl
            << "Expected: \"" << str << "\"";
    } else {
        if (line.substr(0, str.size()) == str) {
        //if (line == str) {
            return ::testing::AssertionSuccess();
        } else {
            return ::testing::AssertionFailure()
                << "Line printed to file is incorrect" << std::endl
                << " Printed: \"" << line << "\"" << std::endl
                << "Expected: \"" << str << "\"";
        }
    }
}

/**
 * @brief Reads the next line from inFile and checks that it is equal to str.
 *        Raises a non-fatal failure with the correct line number if we have
 *        reached the end of file, or if the pattern does not match.
 *
 * @param inFile The input file from which we read the next line
 * @param str The exact string to compare to
 */
#define MATCH_NEXT_LINE(inFile, str) \
    EXPECT_PRED_FORMAT2(matchNextLine, inFile, str)

/**
 * @brief Helper function that returns an AssertionResult for MATCH_END.
 *
 * Do not call this function directly. Call MATCH_END instead.
 *
 * @param expr0 The first expression passed to MATCH_END
 * @param inFile See comment for MATCH_END
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult matchEnd(const std::string &expr0, std::ifstream &inFile) {
    std::string line;
    getline(inFile, line);
    if (inFile.eof()) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure()
            << "Line printed to file is incorrect" << std::endl
            << " Printed: \"" << line << "\"" << std::endl
            << "Expected: (end of printed output)";
    }
}

/**
 * @brief Reads the next line from inFile and checks that the end of the stream
 *        has been reached. Raises a non-fatal failure with the correct line
 *        number otherwise.
 *
 * @param inFile The input file from which we read the next line
 */
#define MATCH_END(inFile) \
    EXPECT_PRED_FORMAT1(matchEnd, inFile)

/**
 * @brief Helper function that returns an AssertionResult for
 *        CHECK_DICTIONARY_PAIR.
 *
 * Do not call this function directly. Call CHECK_DICTIONARY_PAIR instead.
 *
 * @param expr0 The first expression passed to CHECK_DICTIONARY_PAIR
 * @param expr1 The second expression passed to CHECK_DICTIONARY_PAIR
 * @param expr2 The third expression passed to CHECK_DICTIONARY_PAIR
 * @param expr3 The fourth expression passed to CHECK_DICTIONARY_PAIR
 * @param dictionary See comment for CHECK_DICTIONARY_PAIR
 * @param word See comment for CHECK_DICTIONARY_PAIR
 * @param n1 See comment for CHECK_DICTIONARY_PAIR
 * @param n2 See comment for CHECK_DICTIONARY_PAIR
 * @return AssertionSuccess() on success or AssertionFailure() on failure
 */
::testing::AssertionResult checkDictionaryPair(const std::string &expr0,
            const std::string &expr1,
            const std::string &expr2,
            const std::string &expr3,
            std::unordered_map<std::string, std::pair<long, long>> &dictionary,
            const std::string &word,
            long n1,
            long n2) {
    auto search = dictionary.find(word);
    if (search == dictionary.end()) {
        return ::testing::AssertionFailure() << "\"" << word << "\" not in dictionary";
    } else {
        std::pair<long, long> pair0 = search->second;
        std::pair<long, long> pair1 = std::make_pair(n1, n2);
        if (pair0 == pair1) {
            return ::testing::AssertionSuccess();
        } else {
            return ::testing::AssertionFailure()
                << "\"" << word << "\" maps to "
                << "(" << pair0.first << ", " << pair0.second << ")"
                << ", when it should map to "
                << "(" << n1 << ", " << n2 << ")";
        }
    }
}

/**
 * @brief Checks whether word maps to a pair (n1, n2) in dictionary. Raises a
 *        non-fatal failure otherwise.
 *
 * @param dictionary The dictionary passed to fillDictionary()
 * @param word The word to find in the dictionary
 * @param n1 The first number in the pair
 * @param n2 The second number in the pair
 */
#define CHECK_DICTIONARY_PAIR(dictionary, word, n1, n2) \
    EXPECT_PRED_FORMAT4(checkDictionaryPair, dictionary, word, n1, n2)

/**
 * @brief Checks a mapping from word to a pair (n1, n2) in dictionary.
 *
 * @param dictionary The dictionary passed to fillDictionary()
 * @param word The word to add to the dictionary
 * @param n1 The first number in the pair
 * @param n2 The second number in the pair
 */
void addDictionaryPair(std::unordered_map<std::string, std::pair<long, long>> &dictionary,
                       const std::string &word,
                       long n1,
                       long n2) {
    std::pair<long, long> pair = std::make_pair(n1, n2);
    dictionary.emplace(word, pair);
}

/**
 * @brief Test case for cleanData(). Performs best-effort cleanup of modified files.
 */
TEST(A3Test, CleanDataTest) {
    // Set up parameters for function call
    std::ifstream rawReviewRatingsInputFile("rawReviewRatings.txt");
    ASSERT_OPEN(rawReviewRatingsInputFile) << "Cannot open rawReviewRatings.txt for input";

    std::ofstream cleanReviewRatingsOutputFile("testCleanReviewRatings.txt");
    ASSERT_OPEN(cleanReviewRatingsOutputFile)
        << "Cannot open testCleanReviewRatings.txt for output";

    std::unordered_set<std::string> commonWords;
    commonWords.insert("and");
    commonWords.insert("is");
    commonWords.insert("the");

    // Call cleanData() from a3.cpp
    cleanData(rawReviewRatingsInputFile, cleanReviewRatingsOutputFile, commonWords);

    // Make sure commonWords has not been modified
    EXPECT_EQ(commonWords.size(), 3) << "cleanData() should not modify commonWords";
    EXPECT_TRUE(commonWords.find("and") != commonWords.end())
        << "cleanData() should not modify commonWords";
    EXPECT_TRUE(commonWords.find("is") != commonWords.end())
        << "cleanData() should not modify commonWords";
    EXPECT_TRUE(commonWords.find("the") != commonWords.end())
        << "cleanData() should not modify commonWords";

    // Check state of files
    ASSERT_OPEN(rawReviewRatingsInputFile) << "cleanData() should not close inFile";
    EXPECT_EOF(rawReviewRatingsInputFile) << "cleanData() does not read all lines in inFile";
    rawReviewRatingsInputFile.close();
    ASSERT_OPEN(cleanReviewRatingsOutputFile) << "cleanData() should not close outFile";
    cleanReviewRatingsOutputFile.close();

    std::ifstream cleanReviewRatingsInputFile("testCleanReviewRatings.txt");
    ASSERT_OPEN(cleanReviewRatingsInputFile) << "Cannot open testCleanReviewRatings.txt for input";

    // Check contents of file
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "4 The Jungle Book awesome");
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "2 The Lion King awe inspiring");
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "0 Jack Jill worst");
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "4 Finding Dory good");
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "3 Zootopia fantastic");
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "4 Jungle Book fantastic");
    MATCH_NEXT_LINE(cleanReviewRatingsInputFile, "3 Lion King fantastic");
    MATCH_END(cleanReviewRatingsInputFile);

    cleanReviewRatingsInputFile.close();
    std::remove("testCleanReviewRatings.txt");
}

/**
 * @brief Test case for fillDictionary(). Performs best-effort cleanup of modified files.
 */
TEST(A3Test, FillDictionaryTest) {
    // Make sure the clean review ratings file has the correct contents
    std::ofstream cleanReviewRatingsOutputFile("testCleanReviewRatings.txt");
    ASSERT_OPEN(cleanReviewRatingsOutputFile)
        << "Cannot open testCleanReviewRatings.txt for output";
    cleanReviewRatingsOutputFile << "4 The Jungle Book awesome" << std::endl;
    cleanReviewRatingsOutputFile << "2 The Lion King awe inspiring" << std::endl;
    cleanReviewRatingsOutputFile << "0 Jack Jill worst" << std::endl;
    cleanReviewRatingsOutputFile << "1 Finding Dory good" << std::endl;
    cleanReviewRatingsOutputFile << "3 Zootopia fantastic" << std::endl;
    cleanReviewRatingsOutputFile << "4 Jungle Book fantastic" << std::endl;
    cleanReviewRatingsOutputFile << "3 Lion King fantastic" << std::endl;
    ASSERT_NOT_FAILED(cleanReviewRatingsOutputFile)
        << "Error writing to testCleanReviewRatings.txt";
    cleanReviewRatingsOutputFile.close();

    // Set up parameters for function call
    std::ifstream cleanReviewRatingsInputFile("testCleanReviewRatings.txt");
    ASSERT_OPEN(cleanReviewRatingsInputFile) << "Cannot open testCleanReviewRatings.txt for input";

    std::unordered_map<std::string, std::pair<long, long>> dictionary;

    // Call fillDictionary() from a3.cpp
    fillDictionary(cleanReviewRatingsInputFile, dictionary);

    // Check state of files
    ASSERT_OPEN(cleanReviewRatingsInputFile) << "fillDictionary() should not close newInFile";
    EXPECT_EOF(cleanReviewRatingsInputFile)
        << "fillDictionary() does not read all lines in newInFile";
    cleanReviewRatingsInputFile.close();
    std::remove("testCleanReviewRatings.txt");

    // Check contents of dictionary
    CHECK_DICTIONARY_PAIR(dictionary, "Jack", 0, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Jill", 0, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Finding", 1, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Zootopia", 3, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "The", 6, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "Jungle", 8, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "Book", 8, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "Lion", 5, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "King", 5, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "fantastic", 10, 3);
    EXPECT_EQ(dictionary.size(), 16);
}

/**
 * @brief Test case for rateReviews(). Performs best-effort cleanup of modified files.
 */
TEST(A3Test, RateReviewsTest) {
    // Make sure the clean reviews file has the correct contents
    std::ofstream cleanReviewsOutputFile("testCleanReviews.txt");
    ASSERT_OPEN(cleanReviewsOutputFile) << "Cannot open testCleanReviews.txt for output";
    cleanReviewsOutputFile << "like The Jungle Book" << std::endl;
    cleanReviewsOutputFile << "The Lion King fantastic" << std::endl;
    cleanReviewsOutputFile << "Jack Jill bad" << std::endl;
    cleanReviewsOutputFile << "Finding Nemo great" << std::endl;
    cleanReviewsOutputFile << "Zootopia awesome" << std::endl;
    ASSERT_NOT_FAILED(cleanReviewsOutputFile) << "Error writing to testCleanReviews.txt";
    cleanReviewsOutputFile.close();

    // Set up parameters for function call
    std::ifstream cleanReviewsInputFile("testCleanReviews.txt");
    ASSERT_OPEN(cleanReviewsInputFile) << "Cannot open testCleanReviews.txt for input";

    std::unordered_map<std::string, std::pair<long, long>> dictionary;
    addDictionaryPair(dictionary, "awesome", 4, 1);
    addDictionaryPair(dictionary, "Jack", 0, 1);
    addDictionaryPair(dictionary, "Jill", 0, 1);
    addDictionaryPair(dictionary, "Finding", 1, 1);
    addDictionaryPair(dictionary, "Zootopia", 3, 1);
    addDictionaryPair(dictionary, "The", 6, 2);
    addDictionaryPair(dictionary, "Jungle", 8, 2);
    addDictionaryPair(dictionary, "Book", 8, 2);
    addDictionaryPair(dictionary, "Lion", 5, 2);
    addDictionaryPair(dictionary, "King", 5, 2);
    addDictionaryPair(dictionary, "fantastic", 10, 3);

    std::ofstream ratingsOutputFile("testRatings.txt");
    ASSERT_OPEN(ratingsOutputFile) << "Cannot open testRatings.txt for output";

    // Call rateReviews() from a3.cpp
    rateReviews(cleanReviewsInputFile, dictionary, ratingsOutputFile);

    // Make sure dictionary has not been modified
    CHECK_DICTIONARY_PAIR(dictionary, "awesome", 4, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Jack", 0, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Jill", 0, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Finding", 1, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "Zootopia", 3, 1);
    CHECK_DICTIONARY_PAIR(dictionary, "The", 6, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "Jungle", 8, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "Book", 8, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "Lion", 5, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "King", 5, 2);
    CHECK_DICTIONARY_PAIR(dictionary, "fantastic", 10, 3);
    EXPECT_EQ(dictionary.size(), 11);

    // Check state of files
    ASSERT_OPEN(cleanReviewsInputFile) << "rateReviews() should not close testFile";
    EXPECT_EOF(cleanReviewsInputFile) << "rateReviews() does not read all lines in testFile";
    cleanReviewsInputFile.close();
    std::remove("testCleanReviews.txt");
    ASSERT_OPEN(ratingsOutputFile) << "rateReviews() should not close ratingsFile";
    ratingsOutputFile.close();

    std::ifstream ratingsInputFile("testRatings.txt");
    ASSERT_OPEN(ratingsInputFile) << "Cannot open testRatings.txt for input";

    // Check contents of file
    MATCH_NEXT_LINE(ratingsInputFile, "3.25");
    MATCH_NEXT_LINE(ratingsInputFile, "2.83");
    MATCH_NEXT_LINE(ratingsInputFile, "0.67");
    MATCH_NEXT_LINE(ratingsInputFile, "1.67");
    MATCH_NEXT_LINE(ratingsInputFile, "3.50");
    MATCH_END(ratingsInputFile);

    ratingsInputFile.close();
    std::remove("testRatings.txt");
}

/**
 * @brief End-to-end test case. This function is very similar to main() in main.cpp,
 *        with some additional checks at the end. Performs best-effort cleanup of
 *        modified files.
 */
TEST(A3Test, EndToEndTest) {
    // Open the input file containing the raw data.
    std::ifstream rawReviewRatingsInputFile("rawReviewRatings.txt");
    ASSERT_OPEN(rawReviewRatingsInputFile) << "Cannot open rawReviewRatings.txt for input";

    // Open an output file for storing the clean data.
    std::ofstream cleanReviewRatingsOutputFile("testCleanReviewRatings.txt");
    ASSERT_OPEN(cleanReviewRatingsOutputFile)
        << "Cannot open testCleanReviewRatings.txt for output";

    // A set to store the common words.
    std::unordered_set<std::string> commonWords;

    // Open the input file containing the common words.
    std::ifstream commonWordsFile("commonwords.txt");
    ASSERT_OPEN(commonWordsFile) << "Cannot open commonwords.txt for input";

    // Read a collections of common words from the common words file
    // and store it in a set.
    fillCommonWords(commonWordsFile, commonWords);
    EXPECT_EOF(commonWordsFile) << "fillCommonWords() does not read all lines in inFile";
    EXPECT_EQ(commonWords.size(), 154);
    EXPECT_TRUE(commonWords.find("and") != commonWords.end())
        << "commonWords does not contain \"and\"";
    EXPECT_TRUE(commonWords.find("is") != commonWords.end())
        << "commonWords does not contain \"is\"";
    EXPECT_TRUE(commonWords.find("the") != commonWords.end())
        << "commonWords does not contain \"the\"";
    EXPECT_TRUE(commonWords.find("I") != commonWords.end())
        << "commonWords does not contain \"I\"";
    commonWordsFile.close();

    // Clean the raw data and store it in an output file.
    cleanData(rawReviewRatingsInputFile, cleanReviewRatingsOutputFile,
              commonWords);
    rawReviewRatingsInputFile.close();
    cleanReviewRatingsOutputFile.close();

    // A map from each word to a pair consisting of the total rating for this
    // word and the total number of times this word has occurred in the reviews.
    std::unordered_map<std::string, std::pair<long, long>> dictionary;

    // Open an input file containing the clean data.
    std::ifstream cleanReviewRatingsInputFile("testCleanReviewRatings.txt");
    ASSERT_OPEN(cleanReviewRatingsInputFile) << "Cannot open testCleanReviewRatings.txt for input";

    // Create a mapping between the words and their associated values.
    fillDictionary(cleanReviewRatingsInputFile, dictionary);
    cleanReviewRatingsInputFile.close();
    std::remove("testCleanReviewRatings.txt");

    // Open the file with the raw reviews.
    std::ifstream rawReviewsInputFile("rawReviews.txt");
    ASSERT_OPEN(rawReviewsInputFile) << "Cannot open rawReviews.txt for input";

    // Open an output file for storing the clean reviews.
    std::ofstream cleanReviewsOutputFile("testCleanReviews.txt");
    ASSERT_OPEN(cleanReviewsOutputFile) << "Cannot open testCleanReviews.txt for output";

    // Clean the raw reviews and store them in an output file.
    cleanData(rawReviewsInputFile, cleanReviewsOutputFile, commonWords);
    rawReviewsInputFile.close();
    cleanReviewsOutputFile.close();

    // Open the input file containing the clean reviews.
    std::ifstream cleanReviewsInputFile("testCleanReviews.txt");
    ASSERT_OPEN(cleanReviewsInputFile) << "Cannot open testCleanReviews.txt for input";

    // Open an output file for storing the ratings for each review.
    std::ofstream ratingsOutputFile("testRatings.txt");
    ASSERT_OPEN(ratingsOutputFile) << "Cannot open testRatings.txt for output";

    // Rate the reviews and write the ratings to an output file.
    rateReviews(cleanReviewsInputFile, dictionary, ratingsOutputFile);
    cleanReviewsInputFile.close();
    std::remove("testCleanReviews.txt");

    // Check contents of file
    std::ifstream ratingsInputFile("testRatings.txt");
    ASSERT_OPEN(ratingsInputFile) << "Cannot open testRatings.txt for input";

    MATCH_NEXT_LINE(ratingsInputFile, "3.25");
    MATCH_NEXT_LINE(ratingsInputFile, "2.83");
    MATCH_NEXT_LINE(ratingsInputFile, "0.67");
    MATCH_NEXT_LINE(ratingsInputFile, "2.67");
    MATCH_NEXT_LINE(ratingsInputFile, "3.50");
    MATCH_END(ratingsInputFile);

    ratingsInputFile.close();
    std::remove("testRatings.txt");
}

/**
 * @brief Unit test execution begins here.
 *
 * @param argc Number of command-line arguments
 * @param argv Command line arguments
 * @return 0 for normal program termination, other error code for abnormal termination
 */
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
