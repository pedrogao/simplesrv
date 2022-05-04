server:
	g++ -std=c++17 util.cpp client.cpp -o client && \
	g++ -std=c++17 util.cpp server.cpp Channel.cpp Epoll.cpp InetAddress.cpp Socket.cpp -o server
clean:
	rm server && rm client