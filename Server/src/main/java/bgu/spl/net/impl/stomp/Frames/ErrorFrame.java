package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

public class ErrorFrame extends BaseFrame {

    public ErrorFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }


    public String getMessage() { return getHeaderValue("message");}

    public String getTheMessage(){ return getHeaderValue("The message");}

}
