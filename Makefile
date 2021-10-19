CXX_FLAGS = -Wall -Ideps -lpthread -static -O3
all:
	mkdir -p bin
	g++ src/main.cpp $(CXX_FLAGS) -o bin/piaq_x64 

arm:
	mkdir -p bin
	/usr/bin/arm-linux-gnueabi-g++ src/main.cpp $(CXX_FLAGS) -o bin/piaq_arm

clean:
	rm bin -rf
