package bgu.spl.net.srv;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class dataCenter {
    private static class SingletonHolder {
        private static dataCenter instance = new dataCenter();
    }
    private AtomicInteger connectionIdCounter= new AtomicInteger(0);
    private AtomicInteger messageIdCounter= new AtomicInteger(0);
    private Map<String,user> nameToUser;
    private Map<String, List<user>> topicsToUsers;
    private Map<user,Integer> activeUsersToConnectionId;//this map holds a unique identifier for each client (id) and relates it to its stompMessagingProtocol.
    private Map<Integer,user> ConnectionIdToActiveUsers;
    private Map<Integer, ConnectionHandler> connectionIdToConnectionsHandler; //this map holds a unique identifier for each client (id) and relates it to its connectionHandler.

    private dataCenter (){
        nameToUser=new ConcurrentHashMap();
        topicsToUsers=new ConcurrentHashMap();
        activeUsersToConnectionId=new ConcurrentHashMap();
        connectionIdToConnectionsHandler = new ConcurrentHashMap();
        ConnectionIdToActiveUsers = new ConcurrentHashMap();
    }

    public static dataCenter getInstance(){
        return SingletonHolder.instance;
    }

    /**
     *
     * @param name - the login of the user
     * @return the user with this login name
     */
    public user getUserFromName (String name){
        return nameToUser.get(name);
    }

    /**
     * registers a new user with this name
     * @param User - the User we want to add to the map
     */
    public void addUserToName(user User){
        nameToUser.put(User.getName(),User);
    }

    /**
     *
     * @param name - the name of the user we want to delete
     */
    public void removeUser (String name){
        user User = nameToUser.get(name);
        nameToUser.remove(name,User);
    }

    /**
     * returns the ConnectionId that was given to the User or null if it dose not exist
     * @param User
     * @return a ConnectionId
     */
    public Integer getConnectionIdFromActiveUser (user User) {
        synchronized (User) {
            return activeUsersToConnectionId.get(User);
        }
    }


    /**
     * updates the Users map with the connectionId after a user was connected
     * @param ConnectionId - a unique ID representing the connection
     * @param User - the User that was connected
     */
    public void addConnectionIdToUser (int ConnectionId , user User){
        synchronized (User) {
            activeUsersToConnectionId.putIfAbsent(User, ConnectionId);
            ConnectionIdToActiveUsers.putIfAbsent(ConnectionId, User);
        }
    }

    /**
     * updates the Users map with the connectionId after a user was disconnected
     * @param ConnectionId - a unique ID representing the connection
     * @param User - the User that was disconnected
     */
    public void removeConnectionIdOfUser (Integer ConnectionId , user User){
        synchronized (User) {
            synchronized (ConnectionId) {
                activeUsersToConnectionId.remove(User, ConnectionId);
                ConnectionIdToActiveUsers.remove(ConnectionId, User);
            }
        }
    }

    /**
     * return the user Object that received this ConnectionId after being connected
     * @param ConnectionId - a unique ID representing the connection
     * @return the connected User
     */
    public user getActiveUserFromConnectionId (Integer ConnectionId) {
        synchronized (ConnectionId) {
            return ConnectionIdToActiveUsers.get(ConnectionId);
        }
    }


    /**
     * returns the ConnectionHandler that belongs to the User
     * @param ConnectionId - a unique ID representing the connection
     * @return the ConnectionHandler of this connection
     */
    public ConnectionHandler getConnectionHandlerFromConnectionId (Integer ConnectionId){
        synchronized (ConnectionId) {
            return connectionIdToConnectionsHandler.get(ConnectionId);
        }
    }

    /**
     * after a user was connected he receives a ConnectionHandler that is updated to this map
     * @param ConnectionId - a unique ID representing the connection
     * @param handler - the ConnectionHandler of this connection
     */
    public void addConnectionsHandlerToConnectionId (Integer ConnectionId ,ConnectionHandler handler){
        synchronized (ConnectionId) {
            synchronized (handler) {
                connectionIdToConnectionsHandler.putIfAbsent(ConnectionId, handler);
            }
        }
    }

    /**
     * after a user was disconnected we delete the ConnectionHandler from this map
     * @param ConnectionId
     * @param handler
     */
    public void removeConnectionsHandlerOfConnectionId (Integer ConnectionId ,ConnectionHandler handler){
        synchronized (ConnectionId) {
            synchronized (handler) {
                connectionIdToConnectionsHandler.remove(ConnectionId, handler);
            }
        }
    }

    /**
     * returns a list of the users subscribed to the topic
     * @param topic
     * @return a list of the users subscribes to the topic
     */
    public List<user> getSubscribedUsersFromTopic (String topic){
        synchronized (topic) {
            return topicsToUsers.get(topic);
        }
    }

    /**
     * updates the Map according to a subscription event
     * @param topic a string representing a topic users can subscribe to
     * @param subscriptionId an Id representing the subscription to a topic by a user
     * @param User the user subscribing to the topic
     */
    public void addUserToTopic (String topic , Integer subscriptionId , user User){
        synchronized (topic) {
            if (!topicsToUsers.containsKey(topic)) {              // if no the topic dosent exists
                createNewTopic(topic);
            }
            synchronized (User) {
                if (!topicsToUsers.get(topic).contains(User)) {  // checks the user was not previously subscribed to the Topic
                    topicsToUsers.get(topic).add(User);
                    User.SubscribeToTopic(subscriptionId, topic);  // updates the user Object with the subscription
                }
            }
        }
    }

    /**
     * creates a new topic in the dataCenter
     * @param topic - a topic users will subscribe and send messages to
     */
    public void createNewTopic (String topic){
        synchronized (topic) {
            List<user> users = new LinkedList<>();
            topicsToUsers.put(topic, users);
        }
    }

    /**
     * unsubscribe a user from a topic
     * @param topic - a topic users will subscribe and send messages to
     * @param User - the user we wish to unsubscribe from the topic
     */
    public void removeUserFromTopic (String topic , user User){
        synchronized (topic) {
            synchronized (User) {
                topicsToUsers.get(topic).remove(User);
                User.UnsubscribeFromTopic(topic);
            }
        }
    }

    /**
     * removes all references to the connectionId from the data centerand unsubscribe the user from all topics
     * @param connectionId
     */
    public void removesAfterDisconnect (Integer connectionId){
        ConnectionHandler Chandler=getConnectionHandlerFromConnectionId(connectionId);
        removeConnectionsHandlerOfConnectionId(connectionId,Chandler);
        user currentUser = ConnectionIdToActiveUsers.get(connectionId);
        removeConnectionIdOfUser(connectionId,currentUser);
        Set<String> SubscribedTopicsSet =currentUser.getSetOfTopics();
        List<String> SubscribedTopicsList = new LinkedList<>();
        for (String topic : SubscribedTopicsSet) {// iterates over the subscribed topics and unsubscribe the user from all of them
            SubscribedTopicsList.add(topic);
        }
        for(String topic : SubscribedTopicsList){
            removeUserFromTopic(topic,currentUser);
        }
    }

    /**
     * used to receive a new connectionId
     * @return a new Unique connectionId
     */
    public Integer getNewConnectionId (){
        return connectionIdCounter.incrementAndGet();
    }

    /**
     * used to receive a new messageId
     * @return a new Uniqu MessageId
     */
    public Integer getNewMessageId (){
        return messageIdCounter.incrementAndGet();
    }
}
