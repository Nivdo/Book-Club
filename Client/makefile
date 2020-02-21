CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib -pthread -lboost_system -lpthread -lboost_thread

all: StompBookClubClient

StompBookClubClient: bin/BookClubSTOMPProtocol.o bin/Book.o bin/connectionHandler.o bin/EncoderDecoder.o bin/Frame.o bin/main.o bin/UserInfo.o bin/KeyboardReader.o bin/SocketReader.o bin/parser.o
	@echo 'Building target: StompBookClubClient'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/StompBookClubClient bin/BookClubSTOMPProtocol.o bin/Book.o bin/connectionHandler.o bin/EncoderDecoder.o bin/Frame.o bin/main.o bin/UserInfo.o bin/KeyboardReader.o bin/SocketReader.o bin/parser.o $(LFLAGS)
	@echo 'Finished building target: StompBookClubClient'
	@echo ' '

bin/BookClubSTOMPProtocol.o: src/BookClubSTOMPProtocol.cpp include/BookClubSTOMPProtocol.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/BookClubSTOMPProtocol.o src/BookClubSTOMPProtocol.cpp

bin/Book.o: src/Book.cpp include/Book.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Book.o src/Book.cpp

bin/connectionHandler.o: src/connectionHandler.cpp include/connectionHandler.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

bin/EncoderDecoder.o: src/EncoderDecoder.cpp include/EncoderDecoder.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/EncoderDecoder.o src/EncoderDecoder.cpp

bin/Frame.o: src/Frame.cpp include/Frame.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Frame.o src/Frame.cpp

bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp

bin/UserInfo.o: src/UserInfo.cpp include/UserInfo.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/UserInfo.o src/UserInfo.cpp

bin/KeyboardReader.o: src/KeyboardReader.cpp include/KeyboardReader.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/KeyboardReader.o src/KeyboardReader.cpp

bin/SocketReader.o: src/SocketReader.cpp include/SocketReader.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/SocketReader.o src/SocketReader.cpp

bin/parser.o: src/parser.cpp include/parser.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/parser.o src/parser.cpp

clean:
	rm -f bin/*
