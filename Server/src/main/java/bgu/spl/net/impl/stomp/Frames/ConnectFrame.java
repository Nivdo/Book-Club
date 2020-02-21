package bgu.spl.net.impl.stomp.Frames;
import java.util.List;
import java.util.Map;

/**
 * ConnectedFrame class extends Frame. This object represents the frame of each connect communication unit.
 */

public class ConnectFrame extends BaseFrame {

    public ConnectFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        super(StompCommand, HeadersNames, Headers, FrameBody);
    }

    public String getAcceptVersion(){
        return getHeaderValue("accept-version");
    }
    public String getHost(){
        return getHeaderValue("host");
    }

    public String getLogin(){
        return getHeaderValue("login");
    }

    public String getPasscode(){
        return getHeaderValue("passcode");
    }




}
