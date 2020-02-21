//
// Created by nivdo@wincs.cs.bgu.ac.il on 09/01/2020.
//

/**
 * Frame class is an object represents a communication unit.
 * Each specified communication unit has its own class that extends Frame.
 */
#include "Frame.h"

using namespace std;

/**
    *
    * @param StompCommand identifies the frame type
    * @param HeadersNames identifies the frame fields names
    * @param HeadersValues identifies the frame fields values
    * @param FrameBody contains the frame body.
    */

Frame::Frame( string StompCommand, vector<string> headersNames, map<string,string> headers, string FrameBody):
        stompCommand_(StompCommand),
        headersNames_(headersNames),
        headers (headers) ,
        frameBody_(FrameBody){
}

string Frame::print() {
    string output="\n------------\n";
    output = output + stompCommand_+"\n";
    for(size_t i = 0 ;i<headersNames_.size() ; i++){

        output=output+headersNames_.at(i)+':'+headers.at(headersNames_.at(i))+'\n';
    }
    output=output+'\n'+frameBody_+"\n------------\n";
    return output;
}

string Frame::getStompCommand() {
    return stompCommand_;
}

string Frame::getFrameBody() {
    return frameBody_;
}


vector<string> Frame::getHeadersNames() {
    return headersNames_;
}

string Frame::getHeaderValue(string headerName) {
    return headers.at(headerName);
}

Frame::Frame(string StompCommand): stompCommand_(StompCommand) ,headersNames_(), headers () ,frameBody_() {}

