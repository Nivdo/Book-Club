package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

public class ReceiptFrame extends BaseFrame {
    public ReceiptFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }

}
