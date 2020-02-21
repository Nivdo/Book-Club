//
// Created by nivdo@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include <UserInfo.h>

using namespace std;

UserInfo::UserInfo(): subscriptionId_(0), receipt_(0) ,userName_() , subscriptionIdToTopic() , topicToSubscriptionID() , InventoryBooksMap() , waitingForBorrowRequestResponse() , receipts() , genreToBookNameToPreviousOwner() {
}

void UserInfo::increaseSubscriptionId() {
    subscriptionId_= subscriptionId_+1;
}

void UserInfo::increaseReceipt() {
    receipt_=receipt_+1;
    
}

int UserInfo::getSubscriptionId() {
    return subscriptionId_;
}

int UserInfo::getUserReceipt() {
    return receipt_;
    
}

string UserInfo::getUserName() {
    return userName_;
}

void UserInfo::setUserName(string name) {
    userName_=name;
    

}

void UserInfo::addBookToInventory(string bookName, string genre) {
    Book newBook = Book(bookName, genre);
    if(InventoryBooksMap.find(genre) == InventoryBooksMap.end()) {
        vector<Book> BookVector;
        BookVector.push_back(newBook);
        InventoryBooksMap.insert({genre,BookVector});
    }
    else
        InventoryBooksMap.at(genre).push_back(newBook);
    
}

void UserInfo::eraseBookFromInventory(string bookName, string genre) {
    bool found=false;
    for (vector<Book>::iterator it = InventoryBooksMap.at(genre).begin() ; !found && it != InventoryBooksMap.at(genre).end(); ++it){
        string currentBook = it->getName();
        if (currentBook==bookName){
            InventoryBooksMap.at(genre).erase(it);
            found = true;
        }
    }
    
}

bool UserInfo::hasBookInInventory(string bookName, string &genre) {
    bool found=false;
    if(InventoryBooksMap.find(genre)!=InventoryBooksMap.end()){
        for (vector<Book>::iterator it = InventoryBooksMap.at(genre).begin() ; !found && it != InventoryBooksMap.at(genre).end(); ++it){
            string currentBook = it->getName();
            if (currentBook==bookName)
                found = true;
        }
    }
    return found;
}

void UserInfo::updateReceiptsMap(int receiptId, Frame frame) {
    pair<int, Frame> newPair(receiptId, frame);
    receipts.insert(newPair);
    
}

Frame UserInfo::getFrameByReceipt(int receipt) {
    return receipts.at(receipt);
}

void UserInfo::addBookToWaitingForBorrowRequestResponseMap(Book book) {
    string genre= book.getGenre();
    if(waitingForBorrowRequestResponse.find(genre)==waitingForBorrowRequestResponse.end()){
        vector<Book> bookV;
        bookV.push_back(book);
        waitingForBorrowRequestResponse.insert({genre,bookV});
    }
    else
        waitingForBorrowRequestResponse.at(genre).push_back(book);
    
}

void UserInfo::eraseBookFromWaitingForBorrowRequestResponseMap(string genre , string bookName ) {
    bool found = false;
    if (waitingForBorrowRequestResponse.find(genre) != waitingForBorrowRequestResponse.end()) {
        for (vector<Book>::iterator it = waitingForBorrowRequestResponse.at(genre).begin() ; !found && it != waitingForBorrowRequestResponse.at(genre).end(); ++it){
            if(it!=waitingForBorrowRequestResponse.at(genre).end()) {
                string currentBookName = it->getName();
                if (currentBookName == bookName) {
                    waitingForBorrowRequestResponse.at(genre).erase(it);
                    found = true;
                }
            }
        }
    }
    
}



void UserInfo::addSubscriptionIdToTopic(int SubscriptionId, string topic) {
    pair<int,string> newPair(SubscriptionId,topic);
    subscriptionIdToTopic.insert(newPair);
    
}

void UserInfo::eraseSubscriptionToTopic(int SubscriptionId) {
    subscriptionIdToTopic.erase(SubscriptionId);
    
}

bool UserInfo::isBookInWaitingForBorrowMap(string name, string genre) {
    bool found = false;
    if (waitingForBorrowRequestResponse.find(genre) != waitingForBorrowRequestResponse.end()) {
        for(Book waitingBook :waitingForBorrowRequestResponse.at(genre)) {
            if ((waitingBook.getName() == name) & (!found)) {
                if (InventoryBooksMap.find(genre) != InventoryBooksMap.end()) {
                    found = true;
                    for (Book book : InventoryBooksMap.at(genre)) {
                        found = (book.getName() != name);
                    }
                } else {
                    found = true;
                }
            }
        }
    }
    return found;
}

void UserInfo::eraseTopicToSubscriptionId(string topic) {
    topicToSubscriptionID.erase(topic);
    
}

void UserInfo::addTopicToSubscriptionId(int SubscriptionId, string topic) {
    pair<string,int> newPair(topic,SubscriptionId);
    topicToSubscriptionID.insert(newPair);
    
}

int UserInfo::getSubscriptionIdByTopic(string topic) {
    if(topicToSubscriptionID.find(topic)==topicToSubscriptionID.end()){
        return -1;
    }
    return topicToSubscriptionID.at(topic);
}

string UserInfo::getBookList(string topic) {
    string output;
    if(InventoryBooksMap.find(topic)!=InventoryBooksMap.end()){         //meaning there's a value under the topic
        for(Book book :InventoryBooksMap.at(topic)){
            output=output+book.getName()+',';
        }
        output = output.substr(0,output.length()-1);
    }

    return output;


}

string UserInfo::getTopicBySubscriptionId(int subscriptionId) {
    return subscriptionIdToTopic.at(subscriptionId);
}

string UserInfo::getPreviousOwnerName(string genre, string bookName) {
    string output = "not found";
    if(genreToBookNameToPreviousOwner.find(genre)!=genreToBookNameToPreviousOwner.end()){
        if(genreToBookNameToPreviousOwner.at(genre).find(bookName)!=genreToBookNameToPreviousOwner.at(genre).end()){
            output = genreToBookNameToPreviousOwner.at(genre).at(bookName);
        }
    }
    return output;

}

void UserInfo::setPreviousOwnerName(string genre, string bookName, string prevOwnerName) {
    if(genreToBookNameToPreviousOwner.find(genre)==genreToBookNameToPreviousOwner.end()) {
        map<string, string> newMap;
        newMap.insert({bookName,prevOwnerName});
        genreToBookNameToPreviousOwner.insert({genre, newMap});
    }
    else
        genreToBookNameToPreviousOwner.at(genre).insert({bookName,prevOwnerName});
}

void UserInfo::erasePreviousOwnerName(string genre, string bookName) {
    if(genreToBookNameToPreviousOwner.find(genre)!=genreToBookNameToPreviousOwner.end()) {
        map<string,string> bookNameToOwnerNameOfGenre=genreToBookNameToPreviousOwner.at(genre);
        if(bookNameToOwnerNameOfGenre.find(bookName)!=bookNameToOwnerNameOfGenre.end())
            genreToBookNameToPreviousOwner.at(genre).erase(genreToBookNameToPreviousOwner.at(genre).find(bookName));
    }
}

void UserInfo::clearUserMaps() {
    subscriptionIdToTopic.clear();
    topicToSubscriptionID.clear();
    InventoryBooksMap.clear();
    waitingForBorrowRequestResponse.clear();
    receipts.clear();
    genreToBookNameToPreviousOwner.clear();
}