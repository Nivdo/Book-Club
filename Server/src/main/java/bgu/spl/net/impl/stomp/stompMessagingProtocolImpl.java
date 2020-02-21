package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.Frames.*;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.dataCenter;
import bgu.spl.net.srv.user;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class stompMessagingProtocolImpl implements StompMessagingProtocol<BaseFrame> {
    private dataCenter DB = dataCenter.getInstance();
    private Integer connectionId;
    private Connections<BaseFrame> connections;
    private StompEncoderDecoder encdec = new StompEncoderDecoder();
    private boolean shouldTerminate;

    @Override
    public void start(int connectionId, Connections<BaseFrame> connections) {
        this.connectionId=connectionId;
        this.connections=connections;
        shouldTerminate=false;
    }

    /**
     * the method will check the "Stomp Command" of the message and react accordingly
     * @param message - a message recived to the server
     */
    public void process(BaseFrame message){
        switch (message.getStompCommand()) {               //process each message according to its stompCommand
            case ("CONNECT"): {
                connectProcessing((ConnectFrame) message);
                break;
            }
            case ("SEND"): {
                sendProcessing((SendFrame)message);
                break;
            }
            case ("SUBSCRIBE"): {
                processSubscribeFrame((SubscribeFrame)message);
                break;
            }
            case ("UNSUBSCRIBE"): {
                processUnsubscribeFrame((UnsubscribeFrame)message);
                break;
            }
            case ("DISCONNECT"): {
                processDisconnectFrame((DisconnectFrame)message);
                break;
            }
        }
    }

    /**
     * this method will check the user exists. if not than he will create a new user with the provided login and password.
     * otherwise it will check the user is not already logged and gave the right password.
     * the method will send a Error Frame to the user if it failed in one of those checks
     * and a Connected Frame otherwise.
     *
     * @param CF - a Connect Frame
     */
    private void connectProcessing (ConnectFrame CF){
        user theUser= DB.getUserFromName(CF.getLogin());

        if(theUser==null ){//meaning the user dose not exists
            String login = CF.getLogin();
            String password = CF.getPasscode();
            theUser = new user(login,password );    // creates a new user
            DB.addUserToName(theUser);
            DB.addConnectionIdToUser(connectionId,theUser);
            connectedProcessing(CF.getReceiptId(),CF.getAcceptVersion(),CF.getFrameBody());
        }
        else if(theUser.getPassword().equals(CF.getPasscode())){
            if(!(DB.getConnectionIdFromActiveUser(theUser)==null)) {      //meaning the user is already logged in
                String message ="User is already logged in";
                String FrameBody = "you are already logged in to the server \n\u0000";
                System.out.println(message);
                processErrorFrame(CF.getReceiptId(),message,CF,FrameBody);
            }
            else {     //meaning the user exists and not logged so we can connect him
                DB.addConnectionIdToUser(connectionId,theUser);
                connectedProcessing(CF.getReceiptId(), CF.getAcceptVersion(), CF.getFrameBody());
            }
        }
        else {      //meaning the user is existing and he gave a wrong password
            String message ="Wrong Password";
            String FrameBody = "Wrong password for this login name, please try again.\n\u0000";
            System.out.println(message);
            processErrorFrame(CF.getReceiptId(),message,CF,FrameBody);
        }
    }

    /**
     * creates a ConnectedFrame according to the parameters and sends it
     * @param recieptId - the id provided in the connect Frame
     * @param version - the versionId provided in the connect Frame
     * @param FrameBody - the body of the connected Frame (should be empty)
     */
    private void connectedProcessing (Integer recieptId , String version, String FrameBody){
        String StompCommand = "CONNECTED";
        List HeadersName = new LinkedList<>();
        HeadersName.add("receipt-id");
        HeadersName.add("version");
        List HeadersValues = new LinkedList<>();
        Map headers = new HashMap();

        headers.put("receipt-id",recieptId.toString());
        headers.put("version",version);
        ConnectedFrame response = new ConnectedFrame(StompCommand, HeadersName, headers, FrameBody);
        connections.send(connectionId, response);
    }

    /**
     * this method will be called after a problem was detected in a connection request
     * @param receiptId - the id provided in the bad Frame
     * @param message   - the problem in the bad Frame
     * @param TheMessageFrame - the bad Frame
     * @param FrameBody - a short description of problem in the bad Frame
     */
    private void processErrorFrame (Integer receiptId , String message , BaseFrame TheMessageFrame , String FrameBody){
        List HeadersName = new LinkedList<>();
        HeadersName.add("receipt-id");
        String TheMessage= "-----\n"+TheMessageFrame.toString().replace('\u0000','\n')+"-----\n";
        HeadersName.add("message");
        HeadersName.add("The message");
        Map headers = new HashMap();
        headers.put("receipt-id",receiptId.toString());
        headers.put("message",message);
        headers.put("The message",TheMessage);
        ErrorFrame response = new ErrorFrame("ERROR",HeadersName,headers,FrameBody);
        shouldTerminate=true;
        String userName = ((ConnectFrame)TheMessageFrame).getLogin();
        connections.send(connectionId,response);
        DB.removeConnectionsHandlerOfConnectionId(connectionId,DB.getConnectionHandlerFromConnectionId(connectionId));



    }


    /**
     * the method send a message to all the users subscribed to the topic specified in the SendFrame
     * @param SF - a SendFrame received in the server
     */
    private void sendProcessing (SendFrame SF){
        String topic = SF.getDestination();
        List<user> subscribedUsers = DB.getSubscribedUsersFromTopic(topic);     // all the users subscribed to the topic
        if(subscribedUsers==null)        // if the topic dose not exist
            DB.createNewTopic(topic);
        else if(subscribedUsers.size()>0){      // if the topic exist and at least one user subscribed to it
            for (user User : subscribedUsers){  // iterate over all the users and sends them a message Frame
                messageProcessing(DB.getConnectionIdFromActiveUser(User),User.getSubscriptionId(topic),DB.getNewMessageId(),topic,SF.getFrameBody());
            }
        }
    }

    /**
     * after the server received a sendFrame this method creates a MessageFrame according to the parameters
     * and sends it to the subscribed ActiveUser with the specifed connectionId
     * @param connectionId - the connectionId of the user we will send the messageFram to
     * @param subscriptionId - the Id of the User subscription to this destination
     * @param messageId - a unique Id of this Message
     * @param destination - the topic the user subscribed to and responsible for him receiving this message
     * @param FrameBody - the body of the send Frame that will be added to the messageFrame
     */
    private void messageProcessing (int connectionId , Integer subscriptionId, Integer messageId ,  String destination , String FrameBody){
        List HeadersName = new LinkedList<>();
        HeadersName.add("receipt-id");
        HeadersName.add("subscription");
        HeadersName.add("Message-id");
        HeadersName.add("destination");
        Map headers = new HashMap();
        headers.put("receipt-id","-1");
        headers.put("subscription",subscriptionId.toString());
        headers.put("Message-id",messageId.toString());
        headers.put("destination",destination);
        MessageFrame MF = new MessageFrame("MESSAGE",HeadersName,headers,FrameBody);
        connections.send(connectionId,MF);  //sends the message to all users in the topic
    }


    /**
     * subscribe the user to the topic specified in the SubscribeFrame destination header
     * @param SubF - a subscribeFrame
     */
    private void processSubscribeFrame (SubscribeFrame SubF){
        user theUser = DB.getActiveUserFromConnectionId(connectionId);
        DB.addUserToTopic(SubF.getDestination(),Integer.parseInt(SubF.getId()),theUser);
        processReceiptFrame(SubF.getReceiptId());
    }

    /**
     * sends a receipt as a response to a message from the user
     * @param receiptId - the Id the client attached to his original message
     */
    private void processReceiptFrame(Integer receiptId) {
        List HeadersName = new LinkedList<>();
        HeadersName.add("receipt-id");
        List HeadersValues = new LinkedList<>();
        Map headers = new HashMap();
        headers.put("receipt-id",receiptId.toString());
        ReceiptFrame RF = new ReceiptFrame("RECEIPT",HeadersName,headers,"\n\u0000");
        connections.send(connectionId,RF);
;
    }

    /**
     * remove the subscription of the client from the topic specified in the UnSubF
     * @param UnSubF - a Unsubscribe Frame
     */
    private void processUnsubscribeFrame(UnsubscribeFrame UnSubF) {
        user theUser = DB.getActiveUserFromConnectionId(connectionId);
        Integer subscriptionId = Integer.parseInt(UnSubF.getId());
        String topic = theUser.getTopic(subscriptionId);
        DB.removeUserFromTopic(topic,theUser);
        processReceiptFrame(UnSubF.getReceiptId());
    }

    /**
     * the method disconnect the user by deleting all references to his connection Id.
     * afterwards it will send him a reciptId
     * @param DF - a Disconnect Frame
     */
    private void processDisconnectFrame(DisconnectFrame DF) {
        if(DB.getActiveUserFromConnectionId(connectionId) != null) {          //checks the user is currently connected
            shouldTerminate = true;
            processReceiptFrame(DF.getReceiptId());
            connections.disconnect(connectionId);
        }
    }

    /**
     * @return true if the connection should be terminated
     */
    public boolean shouldTerminate(){
        return shouldTerminate;
    }
}
