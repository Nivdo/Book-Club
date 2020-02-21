package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

public class MessageFrame extends BaseFrame {
    public MessageFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }

    public String getSubscription(){return getHeaderValue("subscription"); }

    public String getMessageId(){
        return getHeaderValue("Message-id");
    }

    public String getDestination(){
        return getHeaderValue("destination");
    }

}


