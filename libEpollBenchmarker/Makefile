# You need to link with wrapped syscalls
override CFLAGS += -Wl,--wrap=recv,--wrap=bind,--wrap=listen,--wrap=send,--wrap=socket,--wrap=epoll_wait,--wrap=accept4,--wrap=epoll_ctl

# Include fastSockets and fastWebSockets
override CFLAGS += -DUWS_NO_ZLIB -I../src -I../fastSockets/src

default:
	make -C ../fastSockets
	$(CXX) -flto -O3 -std=c++17 ../examples/HelloWorld.cpp epoll_benchmarker.cpp $(CFLAGS) -o HelloWorld ../fastSockets/fastSockets.a
