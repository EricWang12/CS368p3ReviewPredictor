////////////////////////////////////////////////////////////////////////////////
// File Name:      a3.cpp
//
// Author:         Gerald, Isaac, Varun
// CS email:       gerald@cs.wisc.edu
//                 cisung@wisc.edu
//                 vnaik@cs.wisc.edu
//
// Description:    The source file for a3.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A3.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
//                   4. Import statements.
//
// Sources:        <Sources, if any>
//
// URL(s) of sources:
//                 <URLs of your sources, if any>
////////////////////////////////////////////////////////////////////////////////

#include "a3.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

using namespace std;

void cleanData(std::istream &inFile, std::ostream &outFile,
               std::unordered_set<std::string> &commonWords) {
    string Line;
    std::vector<std::string> words;
    while(true){
        words.clear();
        getline ( inFile, Line);
        if   (inFile.fail()) {
            break;
        }

        splitLine(Line, words);
        removeCommonWords(words, commonWords);
        for (auto i : words) {
            outFile << i << " ";
        }
        outFile << endl;



    }
    // TODO: Implement this function.
    // Remember to call the helper functions below!
}

void fillDictionary(std::istream &newInFile,
                    std::unordered_map<std::string, std::pair<long, long>> &dict) {

    string word;
    std::vector<string> words;
    long currRating = 0 ;

    while(true){
        words.clear();
        getline(newInFile, word );
        if(newInFile.fail()){
            break;
        }
        splitLine(word,words);
        currRating = stoi(words[0]);

        for(int i = 1; i < words.size(); i++) {

            if (dict.find(words[i]) == dict.end()) {
                dict[words[i]] = pair<long, long>(currRating, 1);
            } else {
                dict[words[i]].first += currRating;
                dict[words[i]].second ++;
            }

        }
    }







    // TODO: Implement this function.
}


void fillCommonWords(std::istream &inFile,
                     std::unordered_set<std::string> &commonWords) {

    string word;

    while(true) {
        getline(inFile, word);
        if (inFile.fail()) {
            break;
        }
        commonWords.insert(word);

    }

    // TODO: Implement this function.
}

void rateReviews(std::istream &testFile,
                 std::unordered_map<std::string, std::pair<long, long>> &dict,
                 std::ostream &ratingsFile) {

    string word;
    std::vector<string> words;
    double currRating;


    while(true){
        words.clear();
        currRating = 0.0;

        getline(testFile, word );
        if(testFile.fail()){
            break;
        }
        splitLine(word,words);

        for( auto i : words){

            if(dict.find(i) == dict.end()){
                currRating += NEUTRAL_RATING;
            }else{
                currRating += ((double)dict[i].first / (double)dict[i].second);
            }

        }
        currRating /= words.size();
        ratingsFile << fixed << setprecision(2)  << currRating << endl;
    }


    // TODO: Implement this function.
}

void removeCommonWords(std::vector<std::string> &tokens,
                       std::unordered_set<std::string> &commonWords) {


    for(int  i = 0 ; i < tokens.size() ; i++) {
        if( commonWords.find(tokens[i]) != commonWords.end()){
            tokens.erase(remove(tokens.begin(), tokens.end(),tokens[i]),tokens.end());
            i --;// TODO: Implement this function.
        }
    }
}

void splitLine(std::string &line, std::vector<std::string> &words) {
    // This code is provided for you and is correct.
    // You should NOT make any changes to this function!
    std::stringstream ss;
    ss.str(line);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        if (token.length() != 0) {
            words.push_back(token);
        }
    }
}
