//
// Created by nivdo@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include "SocketReader.h"



void SocketReader::handle() {
    pthread_mutex_lock(&protocol_.socketLock);
    while(!logout) {
        if (protocol_.getIsLoggedIn()) {
            string msg;
            protocol_.getConnectionHandlerR().getLine(msg);
            if (msg.length() > 0) {
                UserInfo user = protocol_.getUserInfoR();
                Frame frame = protocol_.getEncoderDecoderR().stringToFrame(msg);
                int whatToDo = protocol_.HandleSocketMessage(frame);
                switch (whatToDo) {
                    case 0: {
                        break;
                    }
                    case 1: {//receipt frame
                        int receiptId = stoi(frame.getHeaderValue("receipt-id"));
                        if (user.getFrameByReceipt(receiptId).getStompCommand() == "DISCONNECT") {
                            protocol_.closeConnectionHandler();
                        }
                        break;
                    }
                    case 2:
                        break;// message frame
                    case 3: {// error frame
                        protocol_.closeConnectionHandler();
                        break;
                    }
                }
            }
        }
    }
    if(protocol_.getIsLoggedIn())
        protocol_.closeConnectionHandler();
    pthread_mutex_unlock(&protocol_.socketLock);
}

SocketReader::SocketReader(BookClubSTOMPProtocol& protocol) : protocol_(protocol) , logout(false){}

void SocketReader::changeLogout() {
    logout=!logout;

}



