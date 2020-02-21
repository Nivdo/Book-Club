package bgu.spl.net.srv;

import java.util.List;

public class ConnectionsImpl<T> implements Connections<T>{
    private dataCenter DB = dataCenter.getInstance();

    public boolean send(int connectionId, T msg){
        ConnectionHandler Chandler= DB.getConnectionHandlerFromConnectionId(connectionId);
        if(!Chandler.equals(null)) {
            Chandler.send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, T msg){
        List<user> activeUsers = DB.getSubscribedUsersFromTopic(channel);
        if(!activeUsers.equals(null)){
            for(user activeUser : activeUsers) {
                Integer connectionId = DB.getConnectionIdFromActiveUser(activeUser);
                if(!connectionId.equals(null)) {
                    ConnectionHandler Chandler = DB.getConnectionHandlerFromConnectionId(connectionId);
                    if(!Chandler.equals(null)){
                        Chandler.send(msg);
                    }
                }
            }
        }
    }

    public void disconnect(int connectionId) {
        DB.removesAfterDisconnect(connectionId);

    }
}
