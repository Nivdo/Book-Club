//
// Created by nivdo@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include "BookClubSTOMPProtocol.h"


void BookClubSTOMPProtocol::HandleKeyboard(string commandLine) {
    if(commandLine=="bye") {
        if (isLoggedIn)
            commandLine = "logout";
        else
            userInfo_.clearUserMaps();
    }
    if(commandLine!="bye") {
        Frame frame = KeyboardCommandToFrame(commandLine);
        if (frame.getStompCommand() !="badInput" && (frame.getStompCommand() != "UNSUBSCRIBE" || frame.getHeaderValue("id") !="-1")) { // making sure that if is an unsubscribe frame than we are really subscruibed to it
            string message = encdec_.frameToString(frame);
            SendFrameToServer(frame);
        }
    }
}


void BookClubSTOMPProtocol::SendFrameToServer(Frame& frame) {
    pthread_mutex_lock(&lock_);
    string receiptId =frame.getHeaderValue("receipt-id");
    if(receiptId!="-1") {
        userInfo_.updateReceiptsMap(stoi(receiptId), frame);
    }
    string message = encdec_.frameToString(frame);
    if(isLoggedIn)
        isLoggedIn=Chandler_->sendLine(message);
    userInfo_.increaseReceipt();
    pthread_mutex_unlock(&lock_);
}

int BookClubSTOMPProtocol::HandleSocketMessage(Frame& frame) {
    cout <<frame.print();
    if(frame.getStompCommand()=="CONNECTED"){
        HandleConnectedMessageFromSocket (frame);
        return 0;
    }
    else if (frame.getStompCommand()=="RECEIPT"){
        HandleReceiptMessageFromSocket(frame);
        return 1;
    }
    else if (frame.getStompCommand()=="MESSAGE"){
        HandleMessageMessageFromSocket (frame);
        return 2;
    }
    else if (frame.getStompCommand()=="ERROR"){
        HandleErrorMessageFromSocket (frame);
        return 3;
    }
    return -1;
}

void BookClubSTOMPProtocol::HandleConnectedMessageFromSocket(Frame& frame) {
    cout << "Login successful\n";
}

void BookClubSTOMPProtocol::HandleMessageMessageFromSocket(Frame& frame) {
    string MessageBody = frame.getFrameBody();
    int wishToBorrow=MessageBody.find("wish to borrow");
    int has=MessageBody.find(" has ");
    int hasAddedTheBook=MessageBody.find("has added the book");
    int notFound =-1;
    size_t found=0;

    if((size_t) (MessageBody.find("Returning"))==found){
        handleReturningMessage(frame);
    }

    else if((size_t)(MessageBody.find("Taking"))==found){
        handleTakingMessage(frame);
    }

    else if((size_t)(MessageBody.find("book status"))==found){
        handleBookStatusRequest(frame);
    }

    else if(wishToBorrow!=notFound){
        handleBorrowRequest(frame);
    }

    else if(hasAddedTheBook!=notFound){

    }
        //in this case - a user announced he has a certain book that someone asked to borrow
    else if(has!=notFound) {
        handleHasBookAnnouncement(frame);
    }

    string receiptId= frame.getHeaderValue("receipt-id");
    int receipt = stoi(receiptId);
    if(receipt!=-1)
        Frame originalFrameSentToTheServer= userInfo_.getFrameByReceipt(receipt);
}

void BookClubSTOMPProtocol::HandleReceiptMessageFromSocket(Frame& frame) {
    string receiptId= frame.getHeaderValue("receipt-id");
    int receipt = stoi(receiptId);
    Frame originalFrameSentToTheServer= userInfo_.getFrameByReceipt(receipt);
    string genre;
    if (originalFrameSentToTheServer.getStompCommand()=="SUBSCRIBE"){
        genre=originalFrameSentToTheServer.getHeaderValue("destination");
        cout<<"Joined club "+genre+"\n";
        userInfo_.addSubscriptionIdToTopic(stoi(originalFrameSentToTheServer.getHeaderValue("id")),genre);
        userInfo_.addTopicToSubscriptionId(stoi(originalFrameSentToTheServer.getHeaderValue("id")),genre);
    }
    else if(originalFrameSentToTheServer.getStompCommand()=="UNSUBSCRIBE"){
        genre=userInfo_.getTopicBySubscriptionId(stoi(originalFrameSentToTheServer.getHeaderValue("id")));
        cout<< "Exited club "+genre+"\n";
        userInfo_.eraseSubscriptionToTopic(stoi(originalFrameSentToTheServer.getHeaderValue("id")));
        userInfo_.eraseTopicToSubscriptionId(genre);
    }
    else if(originalFrameSentToTheServer.getStompCommand()=="DISCONNECT"){
        userInfo_.clearUserMaps();

    }

}

void BookClubSTOMPProtocol::HandleErrorMessageFromSocket(Frame& frame) {
    string errorMessage = frame.getHeaderValue("message");
    userInfo_.clearUserMaps();
    if (errorMessage=="Wrong Password\n")
        cout<< "Wrong Password";
    else if(errorMessage=="User is already logged in")
        cout<< "User already logged in\n";
}

void BookClubSTOMPProtocol::handleBookStatusRequest(Frame& frame) {
    string genre =frame.getHeaderValue("destination");
    string frameBody = userInfo_.getUserName()+':'+userInfo_.getBookList(genre);
    string receipt= to_string(userInfo_.getUserReceipt());
    string stompCommand= "SEND";
    vector<string> headersNames= {"receipt-id","destination"};
    map<string,string>  headers;
    headers.insert({"receipt-id",receipt});
    headers.insert({"destination",genre});
    vector<string> headersValues = {receipt,genre};
    Frame sendFrame(stompCommand, headersNames, headers, frameBody);
    SendFrameToServer(sendFrame);
}

void BookClubSTOMPProtocol::handleReturningMessage(Frame& msg) {
    string genre = msg.getHeaderValue("destination");
    vector<string> parsedBody =parser::parse(msg.getFrameBody(),' ');
    string returnedTo = parsedBody.at(parsedBody.size()-1);
    string bookName;
    for(size_t i =1 ; i<parsedBody.size()-2;i++){
        bookName=bookName+parsedBody.at(i)+' ';
    }
    bookName.erase(bookName.length()-1,bookName.length());      //erasing the extra last ' '
    if(returnedTo==userInfo_.getUserName())
        userInfo_.addBookToInventory(bookName,genre);
}

void BookClubSTOMPProtocol::handleTakingMessage(Frame& msg) {
    string genre = msg.getHeaderValue("destination");;
    vector<string> parsedBody =parser::parse(msg.getFrameBody(),' ');
    string bookOwner = parsedBody.at(parsedBody.size()-1);
    string bookName;
    for(size_t i =1 ; i<parsedBody.size()-2;i++){
        bookName=bookName+parsedBody.at(i)+' ';
    }
    bookName.erase(bookName.length()-1,bookName.length());      //erasing the extra last ' '
    if(bookOwner==userInfo_.getUserName())
        userInfo_.eraseBookFromInventory(bookName,genre);
}

void BookClubSTOMPProtocol::handleBorrowRequest(Frame& msg) {
    string genre = msg.getHeaderValue("destination");
    vector<string> parsedRequest = parser::parse(msg.getFrameBody(),' ');
    string bookName;
    for(size_t i = 4 ; i< parsedRequest.size();i++){
        bookName=bookName+parsedRequest.at(i)+' ';
    }
    bookName.erase(bookName.size()-1,bookName.size());
    if(userInfo_.hasBookInInventory(bookName,genre)){
        string body = userInfo_.getUserName()+ " has "+bookName+'\0';
        string receipt= to_string(userInfo_.getUserReceipt());
        map<string,string> headers;
        headers.insert({"receipt-id",receipt});
        headers.insert({"destination",genre});
        Frame frame=Frame("SEND",{"receipt-id","destination"},headers,body);
        SendFrameToServer(frame);
    }
}

void BookClubSTOMPProtocol::handleHasBookAnnouncement(Frame& msg) {
    vector<string> parsedWords = parser::parse(msg.getFrameBody(),' ');
    string genre = msg.getHeaderValue("destination");
    string ownerName = parsedWords.at(0);
    string bookName;
    for(size_t i =2 ; i<parsedWords.size() ; i++) {
        bookName = bookName + parsedWords.at(i) + ' ';
    }
    bookName.erase(bookName.length()-1,bookName.length());
    if(userInfo_.isBookInWaitingForBorrowMap(bookName,genre)){
        userInfo_.eraseBookFromWaitingForBorrowRequestResponseMap(genre,bookName);
        userInfo_.setPreviousOwnerName(genre,bookName,ownerName);
        userInfo_.addBookToInventory(bookName,genre);
        string body =  "Taking "+bookName+" from "+ownerName+"\0";
        string receipt= to_string(userInfo_.getUserReceipt());
        map<string,string> headers;
        headers.insert({"receipt-id",receipt});
        headers.insert({"destination",genre});
        Frame frame=Frame("SEND",{"receipt-id","destination"},headers,body);
        SendFrameToServer(frame);
    }
}

ConnectionHandler &BookClubSTOMPProtocol::getConnectionHandlerR() {
    return *Chandler_;
}

EncoderDecoder &BookClubSTOMPProtocol::getEncoderDecoderR() {
    return encdec_;
}

UserInfo &BookClubSTOMPProtocol::getUserInfoR() {
    return userInfo_;
}

void BookClubSTOMPProtocol::closeConnectionHandler() {
    isLoggedIn=false;
    Chandler_->close();
}

BookClubSTOMPProtocol::BookClubSTOMPProtocol(): userInfo_() , encdec_() , lock_() , Chandler_() , isLoggedIn(false) , socketLock() {}



Frame BookClubSTOMPProtocol::KeyboardCommandToFrame(string commandLine){
    string commandName= commandLine.substr(0, commandLine.find_first_of(" ")); //parsing the command name
    if (commandName=="login"){
        return buildConnectFrame(commandLine);
    }
    else if(commandName=="join"){
        return buildSubscribeFrame(commandLine);
    }
    else if (commandName=="exit"){
        return buildUnsubscribeFrame(commandLine);
    }
    else if(commandName=="add"){
        return buildAddSendFrame(commandLine);
    }
    else if (commandName=="borrow"){
        return buildBorrowSendFrame(commandLine);
    }
    else if (commandName=="return"){
        return buildReturnSendFrame(commandLine);
    }
    else if (commandName=="status"){
        return buildStatusSendFrame(commandLine);
    }
    else if (commandName=="logout"){
        return buildDisconnectFrame(commandLine);
    }
    return Frame("badInput");
}


/**
 * Creating all Frames types
 */


/**
 * Creating connectFrame
 * @param command - the command was typed
 * @return the converted command in connect frame type
 */
Frame BookClubSTOMPProtocol::buildConnectFrame(string command) {
    vector<string> parsedCommand = parser::parse(command,' ');
    vector<string> parsedPortHost = parser::parse(parsedCommand.at(1),':');

    string stompCommand = "CONNECT";
    vector<string> headersNames = {"receipt-id","accept-version", "host","login", "passcode"};
    map<string,string> headers;
    string receipt = to_string(userInfo_.getUserReceipt());
    string acceptVersionValue = "1.2";
    string host = parsedPortHost.at(0);
    short port = stoi(parsedPortHost.at(1));
    string userName=parsedCommand.at(2);
    string password= parsedCommand.at(3);//commandString;
    headers.insert({"receipt-id",receipt});
    headers.insert({"accept-version",acceptVersionValue});
    headers.insert({"host",host});
    headers.insert({"login",userName});
    headers.insert({"passcode",password});
    string frameBody ="\0";//there is no frameBody, the frame is over.
    //creating the Frame with the parsed values
    Frame ConnectFrame = {stompCommand, headersNames, headers, frameBody};
    //updating userName according to the parsed name
    userInfo_.setUserName(userName);
    Chandler_ = new ConnectionHandler(host,port);
    if(Chandler_->connect())
        isLoggedIn=true;
    else
        cout<<"Could not connect to server\n";
    return ConnectFrame;
}


/**
 * Creating subscribeFrame
 * @param command - the command was typed
 * @return the converted command in subscribe frame type
 */

Frame BookClubSTOMPProtocol::buildSubscribeFrame(string command) {
    string commandString= command;
    commandString.erase(0, commandString.find_first_of(" ")+1);
    string stompCommand = "SUBSCRIBE";
    vector<string> headersNames = {"destination", "id","receipt-id"};
    map<string,string> headers;
    //parsing headers values
    string destination = commandString;
    string id=to_string(userInfo_.getSubscriptionId());
    userInfo_.increaseSubscriptionId();
    string receipt = to_string(userInfo_.getUserReceipt());
    headers.insert({"receipt-id",receipt});
    headers.insert({"destination",destination});
    headers.insert({"id",id});
    string frameBody ="\0";//there is no frameBody, the frame is over.
    //creating the Frame with the parsed values
    Frame SubscribeFrame = {stompCommand, headersNames, headers, frameBody};
    return SubscribeFrame;
}


/**
 * Creating SendFrame
 * @param command - the command was typed
 * @return the converted command in send frame type
 */

Frame BookClubSTOMPProtocol::buildAddSendFrame(string command) {
    string commandString= command;
    commandString.erase(0, commandString.find_first_of(" ")+1);
    string stompCommand = "SEND";
    vector<string> headersNames = {"receipt-id","destination"};
    map<string,string> headers;
    string receipt = to_string(userInfo_.getUserReceipt());
    string destination = commandString.substr(0, commandString.find_first_of(" "));
    commandString.erase(0, commandString.find_first_of(" ")+1);
    headers.insert({"receipt-id",receipt});
    headers.insert({"destination",destination});
    string bookName= commandString;
    userInfo_.addBookToInventory(bookName,destination);
    string userName= userInfo_.getUserName();
    string frameBody =userName+" has added the book "+bookName+"\0";//there is no frameBody, the frame is over.
    Frame sendFrame = {stompCommand, headersNames, headers, frameBody};
    return sendFrame;
}


/**
 * Creating sendFrame
 * @param command - the command was typed
 * @return the converted command in send frame type
 */

Frame BookClubSTOMPProtocol::buildBorrowSendFrame(string command) {
    string commandString= command;
    commandString.erase(0, commandString.find_first_of(" ")+1);
    string stompCommand = "SEND";
    vector<string> headersNames = {"receipt-id","destination"};
    map<string,string> headers;
    //parsing headers values
    string receipt = to_string(userInfo_.getUserReceipt());
    string destination = commandString.substr(0, commandString.find(" "));
    commandString.erase(0, commandString.find_first_of(" ")+1);
    headers.insert({"receipt-id",receipt});
    headers.insert({"destination",destination});
    //creating frameBody field
    string bookName= commandString;
    string userName= userInfo_.getUserName();
    string frameBody =userName+" wish to borrow "+bookName+ "\0";
    //creating the Frame with the parsed values
    Frame sendFrame = {stompCommand, headersNames, headers, frameBody};
    Book bookWhishedToBorrow = Book(bookName,destination);
    //add the book to the waiting to borrow map.
    userInfo_.addBookToWaitingForBorrowRequestResponseMap(bookWhishedToBorrow);

    return sendFrame;
}

/**
 * Creating sendFrame
 * @param command - the command was typed
 * @return the converted command in send frame type
 */

Frame BookClubSTOMPProtocol::buildReturnSendFrame(string command) {
    vector<string> parsedWords = parser::parse(command, ' ');
    string stompCommand = "SEND";
    vector<string> headersNames = {"receipt-id", "destination"};
    map<string,string> headers;
    //parsing headers values
    string receipt = to_string(userInfo_.getUserReceipt());
    string destination = parsedWords.at(1);
    vector<string> headersValues = {receipt, destination};
    headers.insert({"receipt-id",receipt});
    headers.insert({"destination",destination});
    //creating frameBody field
    string bookName;
    for (size_t i = 2; i < parsedWords.size(); i++) {
        bookName = bookName + parsedWords.at(i) + ' ';
    }
    bookName.erase(bookName.size() - 1, bookName.size());
    string returningTheBookTo;
    if (userInfo_.hasBookInInventory(bookName, destination)) {
        userInfo_.eraseBookFromInventory(bookName, destination);
        returningTheBookTo = userInfo_.getPreviousOwnerName(destination, bookName);
        userInfo_.erasePreviousOwnerName(destination, bookName);
        string frameBody ="Returning "+bookName+" to "+returningTheBookTo+ "\0";
        Frame sendFrame = {stompCommand, headersNames, headers, frameBody};
        return sendFrame;
    } else
        return Frame("badInput");

    //creating the Frame with the parsed values

}

/**
 * Creating sendFrame
 * @param command - the command was typed
 * @return the converted command in send frame type
 */

Frame BookClubSTOMPProtocol::buildStatusSendFrame(string command) {
    string stompCommand = "SEND";
    vector<string> headersNames = {"receipt-id","destination"};
    map<string,string> headers;
    //parsing headers values
    string receipt = to_string(userInfo_.getUserReceipt());
    string destination = command.substr(command.find(" ")+1);
    headers.insert({"receipt-id",receipt});
    headers.insert({"destination",destination});
    //creating frameBody field
    string userName= userInfo_.getUserName();
    string frameBody = "book status\0";
    Frame sendFrame = {stompCommand, headersNames, headers, frameBody};
    return sendFrame;
}

/**
 * builds the disconnect frame
 * @param command
 * @return
 */

Frame BookClubSTOMPProtocol::buildDisconnectFrame(string command) {
    string commandString= command;
    commandString.erase(0, commandString.find_first_of(" "));
    string stompCommand = "DISCONNECT";

    vector<string> headersNames = {"receipt-id"};
    map<string,string> headers;
    //parsing headers values
    string receipt = to_string(userInfo_.getUserReceipt());
    headers.insert({"receipt-id",receipt});
    //creating frameBody field
    string frameBody = "\0";//there is no frameBody, the frame is over.

    //creating the Frame with the parsed values
    Frame disconnectFrame = {stompCommand, headersNames, headers, frameBody};
    return disconnectFrame;
}

bool BookClubSTOMPProtocol::getIsLoggedIn() {
    return isLoggedIn;
}

Frame BookClubSTOMPProtocol::buildUnsubscribeFrame(string command) {
    string commandString= command;
    commandString.erase(0, commandString.find_first_of(" ")+1);
    string stompCommand = "UNSUBSCRIBE";
    vector<string> headersNames = {"receipt-id","id"};
    map<string,string> headers;
    string receipt = to_string(userInfo_.getUserReceipt());
    string genre = commandString.substr(0, commandString.find(" "));
    int subscriptionID= userInfo_.getSubscriptionIdByTopic(genre);
    commandString.erase(0, commandString.find_first_of(" "));
    headers.insert({"receipt-id",receipt});
    headers.insert({"id",to_string(subscriptionID)});
    string frameBody ="\0";
    Frame sendFrame = {stompCommand, headersNames, headers, frameBody};
    return sendFrame;

}

BookClubSTOMPProtocol::~BookClubSTOMPProtocol() {
    delete(Chandler_);
    Chandler_= nullptr;
}

