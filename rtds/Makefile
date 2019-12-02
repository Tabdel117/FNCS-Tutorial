CPPFLAGS = -I$(FNCS_INSTALL)/include #  指定头文件（.h文件）的路径
LDFLAGS = -L$(FNCS_INSTALL)/lib # gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置
LIBS = -lfncs -lczmq -lzmq # 告诉链接器要链接哪些库文件

all: simA simB

tcp: tcp.cpp
	$(CXX) tcp.cpp -o tcp $(CPPFLAGS) $(LDFLAGS) $(LIBS)

simA: simA.cpp
	$(CXX) simA.cpp tcp.cpp -o simA $(CPPFLAGS) $(LDFLAGS) $(LIBS)

simB: simB.cpp
	$(CXX) simB.cpp tcp.cpp -o simB $(CPPFLAGS) $(LDFLAGS) $(LIBS)


clean:
	rm -f simA simB *.out *.log *.txt
