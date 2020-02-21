//
// Created by yaely on 1/13/20.
//

#include <KeyboardReader.h>

void splitVector(vector<string> vector, string basicString, char i);

using namespace std;

int main (int argc, char *argv[]){
    KeyboardReader keyboardReader;
    boost::thread handleKeyboard(&KeyboardReader::handle,&keyboardReader);
    handleKeyboard.join();
}


