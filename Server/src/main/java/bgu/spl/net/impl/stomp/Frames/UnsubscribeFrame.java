package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

public class UnsubscribeFrame extends BaseFrame {

    public UnsubscribeFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }

    public String getId(){return getHeaderValue("id"); }
}
