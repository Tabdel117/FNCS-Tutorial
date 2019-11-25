CPPFLAGS = -I$(FNCS_INSTALL)/include #  指定头文件（.h文件）的路径
LDFLAGS = -L$(FNCS_INSTALL)/lib # gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置
LIBS = -lfncs -lczmq -lzmq # 告诉链接器要链接哪些库文件

all: server client

server: server.cpp
	$(CXX) server.cpp -o server $(CPPFLAGS) $(LDFLAGS) $(LIBS)

client: client.cpp
	$(CXX) client.cpp -o client $(CPPFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f server client *.out *.log *.txt
