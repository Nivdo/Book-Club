//
// Created by nivdo on 14/01/2020.
//

#include <string>
#include <vector>

#ifndef BOOST_ECHO_CLIENT_PARSER_H
#define BOOST_ECHO_CLIENT_PARSER_H

#endif //BOOST_ECHO_CLIENT_PARSER_H

using namespace std;
/**
 * this class is meant to help us parse strings easily and avoid copied code.
 */
class parser {
public:
    /**
     * static method used to parse strings by certain delimiters.
     *
     * @param sentence  - the sentence we wish to parse
     * @param delimiter - the delimiter that we pich that delimits the string.
     * @return  a vector of strings. each "cell" in the vector contains a parsed part of the string.
     */
    static vector<string> parse(string sentence,char delimiter);
};