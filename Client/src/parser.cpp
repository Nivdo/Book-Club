//
// Created by nivdo on 14/01/2020.
//

#include <parser.h>


vector<string> parser::parse(string sentence, char delimiter) {
    vector<string> output;
    size_t sentenceIndex=0;
    while (sentenceIndex<sentence.length()){
        if(sentence.at(sentenceIndex)==delimiter){
            string word =sentence.substr(0,sentenceIndex);
            output.push_back(word);
            sentence=sentence.substr(sentenceIndex+1,sentence.length());
            sentenceIndex=0;
        }
        else{
            sentenceIndex++;
        }
    }
    output.push_back(sentence);
    return output;
}
