//
// Created by nivdo@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <SocketReader.h>
#include "KeyboardReader.h"
using namespace std;

void KeyboardReader::handle() {
    bool logout= false;
    bool flagToActivateSocketThreadOnlyOneTime =false;
    SocketReader socketReader (protocol_);
    boost::thread handleSocket;
    while (!logout){
        const short bufferSize = 1024;
        char buffer[bufferSize];
        cin.getline(buffer,bufferSize);
        string commandLine(buffer);
        if((!flagToActivateSocketThreadOnlyOneTime) & (commandLine.find("login")==0)){
            flagToActivateSocketThreadOnlyOneTime=true;
            handleSocket=boost::thread (&SocketReader::handle,&socketReader);
        }
        else if (!flagToActivateSocketThreadOnlyOneTime)
            cout<<"you must first log in\n";
        protocol_.HandleKeyboard(commandLine);
        string commandName = commandLine.substr(0,commandLine.find_first_of(" "));
        if (commandName=="bye"){
            logout=true;
            socketReader.changeLogout();
        }
    }
    handleSocket.join();
}

KeyboardReader::KeyboardReader(): protocol_() {

}


