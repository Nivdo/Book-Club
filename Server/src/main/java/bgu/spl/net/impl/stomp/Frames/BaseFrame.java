package bgu.spl.net.impl.stomp.Frames;

import java.util.List;
import java.util.Map;

/**
 * Frame class is an object represents a communication unit.
 * Each specified communication unit has its own class that extends Frame.
 */

public abstract class BaseFrame {


    protected String stompCommand;
    protected List<String> headersNames;
//    protected List<String> headersValues;
    protected Map<String,String> headers;
    protected String frameBody;

    /**
     *
     * @param StompCommand identifies the frame type
     * @param HeadersNames identifies the frame fields names
     * @param FrameBody contains the frame body.
     */

    public BaseFrame(String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        stompCommand=StompCommand;
        headersNames=HeadersNames;
        headers=Headers;
        frameBody=FrameBody;
    }

    public String getStompCommand(){
        return stompCommand;
    }

    public String getFrameBody(){
        return frameBody;
    }

    protected String getHeaderValue(String headerName){
        return headers.get(headerName);
    }

    public int getReceiptId(){
        return Integer.parseInt(headers.get("receipt-id"));
    }   //the convention for all frames is that the receipt is in the first header

    public String toString(){
        String output =stompCommand+'\n';
        for(String headerName : headersNames) {
        //for(int i =0 ; i<headersNames.size() ; i++){
            String name = headerName;
            String value = getHeaderValue(headerName);
            String s = name +':'+value+'\n';
            output=output+s;
        }
        output=output+'\n'+frameBody;
        return output;
    }

}
