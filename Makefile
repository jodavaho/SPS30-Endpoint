CXX_FLAGS = -Wall -Ideps -O3 
LD_FLAGS = -lpthread -Ldeps/libconfig/build-$(ARCH)/out -lconfig -static
prefix=/usr/local
CC = g++
ARCH := $(shell gcc -dumpmachine)
VERBOSE=1

bin/piaq-$(ARCH): src/main.cpp
	mkdir -p bin
	$(ARCH)-$(CC) src/main.cpp $(CXX_FLAGS) $(LD_FLAGS) -o bin/piaq-$(ARCH)

install: bin/piaq-$(ARCH)
	cp bin/piaq-$(ARCH) bin/piaq
	install bin/piaq $(DESTDIR)$(prefix)/bin
	install man/piaq.1 $(DESTDIR)$(prefix)/share/man/man1
	install config/piaq.config $(DESTDIR)/etc/piaq

clean:
	rm bin -rf
