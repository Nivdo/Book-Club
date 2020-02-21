//
// Created by nivdo@wincs.cs.bgu.ac.il on 09/01/2020.
//

//This Class is responsible for converting the command recieved for the client to a frame which then is sent to the server.


#include "EncoderDecoder.h"

using namespace std;



/**
 * this method converts a frame to a string
 * @param frame
 * @return the converted Frame to string
 */

string EncoderDecoder::frameToString(Frame frame) {
    string output =frame.getStompCommand()+'\n';
    for (string headerName : frame.getHeadersNames()){
        string s = (headerName+ ":" +frame.getHeaderValue(headerName)+'\n');
        output=output+s;
    }
    output=output+"\n"+frame.getFrameBody()+'\0';
    return output;
}

Frame EncoderDecoder::stringToFrame(string s) {
    string stompCommand;
    vector<string> headersNames;
    map<string,string> headers;
    string frameBody;
    vector<string> parsedString = parser::parse(s,'\n');
    stompCommand=parsedString.at(0);
    size_t i=1;
    int notFound=-1;
    bool isHeaders=true;
    while((isHeaders) && (i<parsedString.size()-1)){
        int delimiterLocation =parsedString.at(i).find(':');
        if(delimiterLocation!=notFound) {
            vector<string> parsedHeader = parser::parse(parsedString.at(i), ':');
            headersNames.push_back(parsedHeader.at(0));
            headers.insert({parsedHeader.at(0),parsedHeader.at(1)});
        }
        else
            isHeaders=false;
        i++;
    }
    while (i<parsedString.size()){
        frameBody=frameBody+parsedString.at(i)+'\n';
        i++;
    }
    frameBody=frameBody.erase(frameBody.length()-1,frameBody.length());
    return Frame(stompCommand, headersNames, headers, frameBody);

}






