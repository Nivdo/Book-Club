package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

public class SendFrame extends BaseFrame {
    public SendFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }

    public String getDestination(){return getHeaderValue("destination"); }

}

