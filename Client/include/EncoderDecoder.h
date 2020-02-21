//
// Created by nivdo@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_ENCODERDECODER_H
#define BOOST_ECHO_CLIENT_ENCODERDECODER_H

#include <stdlib.h>
#include "UserInfo.h"
#include "parser.h"

using namespace std;

class EncoderDecoder {
public:

    /**
     * this method converts a string to a frame
     * @param s
     * @return frame
     */
    Frame stringToFrame (string s);

    /**
     * this method converts Frame to a string
     * @param frame
     * @return string
     */
    string frameToString (Frame frame);


};


#endif //BOOST_ECHO_CLIENT_ENCODERDECODER_H


