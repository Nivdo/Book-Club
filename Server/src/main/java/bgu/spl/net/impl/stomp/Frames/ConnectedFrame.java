package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

/**
 * ConnectedFrame class extends Frame. This object represents the frame of each connected communication unit.
 */
public class ConnectedFrame extends BaseFrame {
    /**
     *
     * @param StompCommand identifies the frame type
     * @param HeadersNames identifies the frame fields names
     * @param FrameBody contains the frame body.
     */
    public ConnectedFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }

    public String getVersion(){
        return getHeaderValue("version");
    }


}
