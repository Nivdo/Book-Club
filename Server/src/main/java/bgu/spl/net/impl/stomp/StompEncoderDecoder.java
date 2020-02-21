package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.Frames.*;

import java.nio.charset.StandardCharsets;
import java.util.*;

/**
 * This class is responsible of encoding and decoding the messages
 */
public class StompEncoderDecoder implements MessageEncoderDecoder<BaseFrame> {
    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;


    @Override
    public BaseFrame decodeNextByte(byte nextByte) {
        if (nextByte == '\u0000') {
            return popString();
        }

        pushByte(nextByte);
        return null; //not a line yet
    }

    @Override
    public byte[] encode(BaseFrame message) {
        String msg=FrametoString(message);
        return (msg + "\u0000").getBytes(); //uses utf8 by default
    }

    /**
     * This method converts Frame to String
     * @param baseFrame
     * @return the frame as a string
     */
    public String FrametoString(BaseFrame baseFrame) {
        return baseFrame.toString();
    }

    /**
     * This method adds a byte to bytes
     * @param nextByte
     */
    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }
        bytes[len++] = nextByte;
    }

    /**
     * This methods return the full frame once finishing reading the Bytes message
     * @return the frame that was sent as a message
     */
    private BaseFrame popString() {
        String result = new String(bytes, 0, len, StandardCharsets.UTF_8);
        len = 0;
        BaseFrame output = StringToFrame(result);
        return output;
    }

    /**
     * this method converts String to Frame
     * @param s - the message as a string
     * @return thr message as a frame
     */
    public BaseFrame StringToFrame (String s){
        if(s.length()>0) {
            String stompCommand;
            String headers;
            List<String> headersNames = new LinkedList<>();
            Map<String,String> headersMap = new HashMap<>();
            List<String> headersValues = new LinkedList<>();
            String frameBody;
            stompCommand = s.substring(0, s.indexOf('\n'));
            int start = s.indexOf('\n') + 1;
            int end = s.indexOf("\n\n");
            headers = s.substring(start, end);//meaning - from the second line until we /home/nivdo/Desktop/Boost_Echo_Clientget to 2 lines drop (which means an empty line)
            int i = 0;
            while (i < headers.length()) { //parsing the headers - names and values
                if (headers.charAt(i) == ':') {
                    headersNames.add(headers.substring(0, i));
                    headers = headers.substring(i + 1);
                    i = 0;
                } else if (headers.charAt(i) == '\n') {
                    //headersValues.add(headers.substring(0, i));
                    headersMap.put(headersNames.get(headersNames.size()-1),headers.substring(0, i));
                    headers = headers.substring(i + 1);
                    i = 0;
                } else
                    i++;
            }
            headersMap.put(headersNames.get(headersNames.size()-1),headers.substring(0, i));
            frameBody = s.substring(s.indexOf("\n\n") + 2);

            return createFrame(stompCommand, headersNames, headersMap, frameBody);
        }
        return null;
    }

    /**
     * This method creates the appropriate type of Frame according to the stompCommand
     * @param StompCommand
     * @param HeadersNames
     * @param FrameBody
     * @return return the created Frame
     */
    private BaseFrame createFrame (String StompCommand, List<String> HeadersNames, Map<String,String> Headers, String FrameBody){
        BaseFrame output=null;
        if(StompCommand.equals("CONNECT"))
            output= new ConnectFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("CONNECTED"))
            output= new ConnectedFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("MESSAGE"))
            output= new MessageFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("RECEIPT"))
            output= new ReceiptFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("ERROR"))
            output= new ErrorFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("SEND"))
            output= new SendFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("SUBSCRIBE"))
            output= new SubscribeFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("UNSUBSCRIBE"))
            output= new UnsubscribeFrame(StompCommand, HeadersNames, Headers,FrameBody);
        else if(StompCommand.equals("DISCONNECT"))
            output= new DisconnectFrame(StompCommand, HeadersNames, Headers,FrameBody);
        return output;
        }

    }






