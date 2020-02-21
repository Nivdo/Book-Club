package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

public class DisconnectFrame extends BaseFrame {

    public DisconnectFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);    }
}
