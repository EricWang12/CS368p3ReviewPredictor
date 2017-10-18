////////////////////////////////////////////////////////////////////////////////
// File Name:      a3.hpp
//
// Author:         Gerald, Isaac, Varun
// CS email:       gerald@cs.wisc.edu
//                 cisung@wisc.edu
//                 vnaik@cs.wisc.edu
//
// Description:    The header file for a3.
//
// IMPORTANT NOTE: THIS FILE SHOULD NOT BE MODIFIED.
////////////////////////////////////////////////////////////////////////////////

// Include Guard
// See: https://en.wikipedia.org/wiki/Include_guard
#ifndef A3_HPP
#define A3_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// The rating associated with a neutral review.
static const double NEUTRAL_RATING = 2.00;

/**
 * @brief Cleans the review data by removing common words.
 *
 * @param inFile The input file with the raw review data.
 * @param outFile The output file where the clean review data is written.
 * @param commonwords A set of common words to be removed from the raw review data.
 */
void cleanData(std::istream &inFile, std::ostream &outFile,
               std::unordered_set<std::string> &commonwords);

/**
 * @brief Reads the reviews and ratings from the file and creates a map from
 *        each word to a pair consisting of the total rating for this word and
 *        the total number of times this word has occurred in the reviews.
 *
 * @param file The file with the review data.
 * @param dict The map/dictionary that we are creating
 *             for each word in the review.
 */
void fillDictionary(std::istream &file,
                    std::unordered_map<std::string, std::pair<long, long>> &dict);

/**
 * @brief Read the common words from a file and store them in a set.
 *
 * @param inFile The file with the common words.
 * @param commonwords The set to store the common words.
 */
void fillCommonWords(std::istream &inFile,
                   std::unordered_set<std::string> &commonwords);

/**
 * @brief Read the reviews from a file and give a rating for each review.
 *
 * @param testFile The input file that contains the reviews to be rated.
 * @param dict The map/dictionary that we use to rate the reviews.
 * @param ratingsFile The output file into which we write our ratings
 *                    for each review.
 */
void rateReviews(std::istream &testFile,
                 std::unordered_map<std::string, std::pair<long, long>> &dict,
                 std::ostream &ratingsFile);

/**
 * @brief Removes the common words from a vector of strings.
 *
 * @param tokens The vector of strings which may contain common words.
 * @param commonwords The set of common words that'll be removed from the vector.
 */
void removeCommonWords(std::vector<std::string> &tokens,
                     std::unordered_set<std::string> &commonwords);

/**
 * @brief Splits a line of text into a vector of strings
 *        using the whitespace as the delimiter.
 *
 * @param line The input line of text to be split into multiple words.
 * @param tokens The vector which holds the strings after the line is split.
 */
void splitLine(std::string &line, std::vector<std::string> &tokens);

#endif //A3_HPP
