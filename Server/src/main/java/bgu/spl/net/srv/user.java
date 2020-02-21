package bgu.spl.net.srv;

import java.util.*;

public class user {
    private String name;
    private String password;
    private Map<String,Integer> topicsToSubscriptionIdMap;
    private Map<Integer,String> SubscriptionIdToTopicsMap;

    public user(String Name , String Password){
        name=Name;
        password=Password;
        topicsToSubscriptionIdMap=new HashMap<>();
        SubscriptionIdToTopicsMap=new HashMap<>();
    }

    public String getName(){
        return name;
    }

    public String getPassword(){
        return password;
    }

    public Set<String> getSetOfTopics (){
        return topicsToSubscriptionIdMap.keySet();
    }

    public int getSubscriptionId (String topic){
        return topicsToSubscriptionIdMap.get(topic);
    }

    public String getTopic (int subscriptionId){
        return SubscriptionIdToTopicsMap.get(subscriptionId);
    }


    public void SubscribeToTopic (int SubscriptionId , String topic){
        topicsToSubscriptionIdMap.putIfAbsent(topic,SubscriptionId);
        SubscriptionIdToTopicsMap.putIfAbsent(SubscriptionId,topic);
    }

    public void UnsubscribeFromTopic (String topic){
        int SubscriptionId =topicsToSubscriptionIdMap.get(topic);
        topicsToSubscriptionIdMap.remove(topic,SubscriptionId);
        SubscriptionIdToTopicsMap.remove(SubscriptionId,topic);

    }

}
