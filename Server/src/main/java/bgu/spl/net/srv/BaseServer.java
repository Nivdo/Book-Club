package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {
    private final int port;
    private final Supplier<StompMessagingProtocol<T>> protocolFactory;
    private final Supplier<MessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;
    private final ConnectionsImpl<T> connections;
    private dataCenter DB;

    public BaseServer(
            int port,
            Supplier<StompMessagingProtocol<T>> protocolFactory,
            Supplier<MessageEncoderDecoder<T>> encdecFactory) {
        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.connections = new ConnectionsImpl<>();
		this.sock = null;
		DB=dataCenter.getInstance();
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {            //creates a new socket
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close              //assign the new socket to the private field

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();
                StompMessagingProtocol<T> SMP = protocolFactory.get();
                BlockingConnectionHandler<T> Chandler = new BlockingConnectionHandler(       //creates a new BlockingConnectionHandler
                        clientSock,
                        encdecFactory.get(),
                        SMP);
                Integer connectionId =DB.getNewConnectionId();
                DB.addConnectionsHandlerToConnectionId(connectionId,Chandler); //update Handlers map
                SMP.start(connectionId,connections);

                execute(Chandler);

            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

}
