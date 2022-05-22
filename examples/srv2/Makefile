SOURCE=$(wildcard ./*.cpp)

server_mac:
	g++ -std=c++17 -pthread \
	$(SOURCE) \
	-DOS_MACOS \
	-o server

server_linux:
	g++ -std=c++17 -pthread \
	$(SOURCE) \
	-DOS_LINUX \
	-o server

clean:
	rm server 