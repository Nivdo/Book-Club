//
// Created by nivdo@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_SOCKETREADER_H
#define BOOST_ECHO_CLIENT_SOCKETREADER_H

#include "EncoderDecoder.h"
#include "BookClubSTOMPProtocol.h"

using namespace std;


class SocketReader {
private:
    BookClubSTOMPProtocol& protocol_;
    bool logout;
public:
    SocketReader(BookClubSTOMPProtocol& protocol);
    /**
     * this method is responsible of handling every thing received from the server through the socket.
     */
    void handle();

    /**
     * this method changes the status of the user from logged in to logged out and vise versa.
     */
    void changeLogout();

};


#endif //BOOST_ECHO_CLIENT_SOCKETREADER_H
