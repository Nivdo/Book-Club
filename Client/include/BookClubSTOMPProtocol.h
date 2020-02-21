//
// Created by nivdo@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOKCLUBSTOMPPROTOCOL_H
#define BOOST_ECHO_CLIENT_BOOKCLUBSTOMPPROTOCOL_H


#include "Book.h"
#include "EncoderDecoder.h"
#include "connectionHandler.h"
#include <boost/thread.hpp>

/**
 * this class is responsible to the club stomp protocol and practically contains most of the logic
 */

class BookClubSTOMPProtocol {

private:
    UserInfo userInfo_;
    EncoderDecoder encdec_;
    pthread_mutex_t lock_;
    ConnectionHandler* Chandler_ = nullptr;
    bool isLoggedIn;

    /**
     * Handles a connected message received from the socket
     * @param frame
     */
    void HandleConnectedMessageFromSocket (Frame& frame);

    /**
     * Handles a message from type message received from the socket
     * @param frame
     */
    void HandleMessageMessageFromSocket (Frame& frame);

    /**
     * Handles a receipt message received from the socket
     * @param frame
     */
    void HandleReceiptMessageFromSocket (Frame& frame);

    /**
     * Handles an error message received from the socket
     * @param frame
     */
    void HandleErrorMessageFromSocket (Frame& frame);

    /**
     * Handles a book status request received from the socket
     * @param frame
     */
    void handleBookStatusRequest(Frame& frame);

    /**
     * Handles a returning book message received from the socket
     * @param msg
     */
    void handleReturningMessage(Frame& msg);

    /**
     * Handles taking (borrowing) a book from someone when such message received from the socket
     * @param msg
     */
    void handleTakingMessage(Frame& msg);

    /**
     * Handles borrow a book request received from the socket
     * @param msg
     */
    void handleBorrowRequest(Frame& msg);

    /**
     * Handles has a book announcement (originally received from one of the club's friends) received from the socket
     * @param msg
     */
    void handleHasBookAnnouncement(Frame& msg);

    /**
     * This method is responsible to the sending frames to the server. all frames sent to the server will use this method.
     * this means both data received from the socket reader and from the keyboard reader will use this method when willing to send a message to the server.
     * @param frame - the frame we wish to transfer to the server
     */
    void SendFrameToServer (Frame& frame);

    /**
     * This method processes a login string received from the keyboard to a connect frame later on sent to the server.
     * @param command
     * @return frame of type connect.
     */
    Frame buildConnectFrame (string command);

    /**
     * This method processes a join string received from the keyboard to a subscribe frame later on sent to the server.
     * @param command
     * @return frame of type subscribe
     */
    Frame buildSubscribeFrame (string command);

    /**
     * This method processes an exit string received from the keyboard to an unsubscribe frame later on sent to the server.
     * @param command
     * @return frame of type unsubscribe
     */
    Frame buildUnsubscribeFrame (string command);

    /**
     * This method processes a string received from the keyboard regarding adding a book to a frame later on sent to the server.
     * @param command
     * @return frame - a send frame
     */

    Frame buildAddSendFrame (string command);

    /**
     * This method processes a borrow request string received from the keyboard to a send frame later on sent to the server.
     * @param command
     * @return a send frame
     */
    Frame buildBorrowSendFrame (string command);

    /**
     * This method processes a return book request string received from the keyboard to a frame, later on sent to the server.
     * @param command
     * @return a send frame
     */
    Frame buildReturnSendFrame (string command);

    /**
     * This method processes a book status request string received from the keyboard to a frame, later on sent to the server.
     * @param command
     * @return frame
     */
    Frame buildStatusSendFrame (string command);

    /**
     * This method processes a logout request string received from the keyboard to a discconect frame, later on sent to the server.
     * @param command
     * @return disconnect frame
     */
    Frame buildDisconnectFrame (string command);

    /**
     * this method is responsible of converting a keyboard command to frame
     * to do so' it uses other methods described above
     * @param commandLine
     * @return
     */
    Frame KeyboardCommandToFrame(string commandLine);

public:
    /**
     * the protocol constructor
     */
    BookClubSTOMPProtocol();

    /**
     * a default copy constructor
     */
    BookClubSTOMPProtocol(const BookClubSTOMPProtocol &other) = default;

    /**
     * a default copy assginment operator constructor
     */
    BookClubSTOMPProtocol& operator=(const BookClubSTOMPProtocol &other) = default;


    /**
     * the protocol destructor
     */
    ~BookClubSTOMPProtocol();

    /**
     * this method is responsible of handling everything received from the keyboard -
     * reading from the keyboard, translating it to frame, processing the data, if needed do changes in the data and replying the server
     * It calls other methods
     *
     * @param commandLine
     */
    void HandleKeyboard (string commandLine);

    /**
     * this method is responsible of handling everything received from the server -
     * reading from the socket, translating it to frame, processing the data, if needed do changes in the data and replying the server
     * It calls other methods
     * @param frame
     * @return
     */
    int HandleSocketMessage(Frame& frame);

    /**
     * get ConnectionHandler method
     * @return a reference to the connection handler.
     */
    ConnectionHandler& getConnectionHandlerR();

    /**
     * get UserInfo method
     * @return a reference to the user info.
     */
    UserInfo& getUserInfoR ();

    /**
     * get encoder decoder method
     * @return a reference to the encoder decoder.
     */
    EncoderDecoder& getEncoderDecoderR();

    /**
     * this method closes the connection Handler
     */
    void closeConnectionHandler ();

    /**
     * this method returns if the user is logged in
     * @return if the user is logged in
     */
    bool getIsLoggedIn ();

    //this is an object used as a lock in order to
    pthread_mutex_t socketLock;
};


#endif //BOOST_ECHO_CLIENT_BOOKCLUBSTOMPPROTOCOL_H
