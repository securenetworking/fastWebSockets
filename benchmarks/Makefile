default:
	g++ -flto -march=native parser.cpp -O3 -I../fastSockets/src -o parser
	clang -flto -O3 -DLIBUS_USE_OPENSSL -I../fastSockets/src ../fastSockets/src/*.c ../fastSockets/src/eventing/*.c ../fastSockets/src/crypto/*.c broadcast_test.c load_test.c scale_test.c -c
	clang++ -flto -O3 -DLIBUS_USE_OPENSSL -I../fastSockets/src ../fastSockets/src/crypto/*.cpp -c -std=c++17
	clang++ -flto -O3 -DLIBUS_USE_OPENSSL `ls *.o | grep -Ev "load_test|scale_test"` -lssl -lcrypto -o broadcast_test
	clang++ -flto -O3 -DLIBUS_USE_OPENSSL `ls *.o | grep -Ev "broadcast_test|scale_test"` -lssl -lcrypto -o load_test
	clang++ -flto -O3 -DLIBUS_USE_OPENSSL `ls *.o | grep -Ev "broadcast_test|load_test"` -lssl -lcrypto -o scale_test

