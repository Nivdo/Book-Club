//
// Created by nivdo@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_KEYBOARDREADER_H
#define BOOST_ECHO_CLIENT_KEYBOARDREADER_H


#include "BookClubSTOMPProtocol.h"


class KeyboardReader {
private:
    BookClubSTOMPProtocol protocol_;
public:
    /**
     * constructor of the keyboard reader
     */
    KeyboardReader();

    /**
     * the method is responsible of handling all the stuff received in the keyboard.
     * this method operated by a thread.
     */
    void handle();


};


#endif //BOOST_ECHO_CLIENT_KEYBOARDREADER_H
