//
// Created by nivdo@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USERINFO_H
#define BOOST_ECHO_CLIENT_USERINFO_H
#include <map>
#include "Book.h"
#include "Frame.h"


using namespace std;


class UserInfo {
private:
    int subscriptionId_;//the subscription id counter
    int receipt_;//the receipt counter
    string userName_;
    map<int, string> subscriptionIdToTopic; //this map matches between the user subscriptionId to the subscribed topic
    map<string, int> topicToSubscriptionID; //this map matches between the user subscriptionId to the subscribed topic
    map<string, vector<Book>> InventoryBooksMap;// this is the users inventory map. it sorts the books the user currently poses (weather its his or borrowed)
    // the map key is the genre and the vector is the list of books he has in this genre
    map <string, vector<Book>> waitingForBorrowRequestResponse; //this map holds the books the user wishes to borrow. the key is the genre and the value is the book.
    map <int, Frame> receipts; //this map holds all the receipts. the key is the receipt number and the value is the sent frame;
    map <string,map<string,string>> genreToBookNameToPreviousOwner;
public:
    /**
     * constructor
     */
    UserInfo();

    /**
     * increases the subscription ID by 1.
     */
    void increaseSubscriptionId();

    /**
     * increases the receipt by 1.
     */
    void increaseReceipt();

    /**
     *
     * @return the user subscription ID
     */
    int getSubscriptionId();

    /**
     *
     * @return the user receipt
     */
    int getUserReceipt();

    /**
     *
     * @return the user's name
     */
    string getUserName();

    /**
     * sets user name
     * @param name
     */
    void setUserName(string name);

    /**
     * add the given book to the inventory map
     * @param bookName
     * @param genre
     */
    void addBookToInventory(string bookName, string genre);

    /**
     * erases the given book from the user inventory
     * @param bookName
     * @param genre
     */
    void eraseBookFromInventory(string bookName, string genre);

    /**
     *
     * @param bookName
     * @param genre
     * @return if the user currently has the given book
     */
    bool hasBookInInventory (string bookName, string &genre);

    /**
     * adds the receipt ID and the paired frame to the user receipts map
     * @param receiptId
     * @param frame
     */
    void updateReceiptsMap (int receiptId, Frame frame);

    /**
     *
     * @param receipt
     * @return the frame sent to the server with that receipt
     */
    Frame getFrameByReceipt (int receipt);

    /**
     * adds the given book to the user's list of books he wishes to borrow
     * @param book
     */
    void addBookToWaitingForBorrowRequestResponseMap (Book book);

    /**
     * erases the given book from the user's list of books he wishes to borrow
     * @param genre
     * @param bookName
     */
    void eraseBookFromWaitingForBorrowRequestResponseMap(string genre , string bookName );

    /**
     * checks if the given book exists in the user's wishes to borrow books list.
     * @param name
     * @param genre
     * @return true if it does exists there, false otherwise
     */
    bool isBookInWaitingForBorrowMap (string name, string genre);

    /**
     * adds a new pair to the subscription to topic map (when a user subscribes to a genre)
     * @param SubscriptionId
     * @param topic
     */
    void addSubscriptionIdToTopic (int SubscriptionId , string topic);

    /**
     * erases the pair with the given subscription ID from the subscription to topic map (when a user unsubscribes a genre)
     * @param SubscriptionId
     */
    void eraseSubscriptionToTopic (int SubscriptionId);

    /**
     * adds a new pair to the topic to subscription map (when a user subscribes to a genre)
     * @param SubscriptionId
     * @param topic
     */
    void addTopicToSubscriptionId (int SubscriptionId , string topic);

    /**
     * erases the pair with the given topic from the topic to subscription map (when a user unsubscribes a genre)
     * @param topic
     */
    void eraseTopicToSubscriptionId (string topic);

    /**
     *
     * @param topic
     * @return the subscription Id the user is signed up to with to the given topic
     */
    int getSubscriptionIdByTopic (string topic);

    /**
     *
     * @param topic
     * @return a string of books names the client owns in the given genre
     */
    string getBookList(string topic);

    /**
     *
     * @param SubscriptionId
     * @return the topic the user is signed up to with the given subscription Id
     */
    string getTopicBySubscriptionId(int SubscriptionId);

    /**
     *
     * @param genre
     * @param bookName
     * @return the book's owner's previous name ( means who the client borrowed the book from)
     */
    string getPreviousOwnerName(string genre,string bookName);

    /**
     * sets the book's owner's previous name ( means who the client borrowed the book from)
     * @param genre
     * @param bookName
     * @param prevOwnerName
     */
    void setPreviousOwnerName(string genre, string bookName, string prevOwnerName);

    /**
     * erases from the map PreviousOwnerName the given book
     * @param genre
     * @param bookName
     */
    void erasePreviousOwnerName(string genre,string bookName);

    /**
     * this methods clears all the user's map.
     * it is called when the user is disconnecting, saying bye or getting an Error.
     */
    void clearUserMaps();
};

#endif //BOOST_ECHO_CLIENT_USERINFO_H
