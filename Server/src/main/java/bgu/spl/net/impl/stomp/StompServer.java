package bgu.spl.net.impl.stomp;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if(args[1].toUpperCase().equals("REACTOR")){
            Server server = Server.reactor(Runtime.getRuntime().availableProcessors(),Integer.parseInt(args[0]),stompMessagingProtocolImpl::new ,StompEncoderDecoder::new);
            server.serve();
        }
        else if(args[1].toUpperCase().equals("TPC")){
            Server server = Server.threadPerClient(Integer.parseInt(args[0]),stompMessagingProtocolImpl::new ,StompEncoderDecoder::new);
            System.out.println("TPC server created and starting to serve");
            server.serve();
        }
    }
}
