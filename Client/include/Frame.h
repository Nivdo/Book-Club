//
// Created by nivdo@wincs.cs.bgu.ac.il on 09/01/2020.
//
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <list>
#include <vector>

using namespace std;

#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H


class Frame {
protected:
    string stompCommand_;
    vector<string> headersNames_;
    map<string,string> headers;
    string frameBody_;
public:

     /**
      * the Frame constructor
      * @param StompCommand - the command name (like disconnect, subscribe...)
      * @param headers - a map with the headers
      * @param FrameBody
      */
    Frame(string StompCommand, vector<string> headersNames, map<string,string> headers, string FrameBody);

    /**
     * a bad Input Frame Constructor
     * @param StompCommand - will be "badInput"
     */
    Frame(string StompCommand);

    /**
     *
     * @return the stomp command (the command name)
     */
    string getStompCommand();

    /**
     *
     * @return the frame body
     */
    string getFrameBody();

    /**
     *
     * @return  a vector of type string contains all the headers names.
     */
    vector<string> getHeadersNames();

    /**
     *
     * @return  a vector of type string contains all the headers values.
     */
    string getHeaderValue(string headerName);

    /**
    * this method is responsible of creating a string message that can later be printed to the client.
    * @return the string message that will be printed
    */
    string print();



};


#endif //BOOST_ECHO_CLIENT_FRAME_H
