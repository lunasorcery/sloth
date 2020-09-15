CXX ?= clang

all: bin/sloth

bin/sloth: main.cpp
	mkdir -p bin
	$(CXX) main.cpp -o bin/sloth -std=c++11 -lstdc++ -Wall -Wextra -O3

install: bin/sloth
	cp bin/sloth /usr/local/bin/sloth

uninstall:
	rm -f /usr/local/bin/sloth

clean:
	rm -rf bin

