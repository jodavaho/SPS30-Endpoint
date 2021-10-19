CXX_FLAGS = -Wall -Ideps -lpthread -static -O3
prefix=/usr/local
CC = g++
ARCH := $(shell gcc -dumpmachine)
VERBOSE=1

bin/piaq-$(ARCH):
	mkdir -p bin
	$(ARCH)-$(CC) src/main.cpp $(CXX_FLAGS) -o bin/piaq-$(ARCH)

install: bin/piaq-$(ARCH)
	cp bin/piaq-$(ARCH) bin/piaq
	install bin/piaq $(DESTDIR)$(prefix)/bin
	install man/piaq.1 $(DESTDIR)$(prefix)/share/man/man1

deb: bin/piaq-$(ARCH)
	mkdir -p fakeroot/$(DESTDIR)$(prefix)/bin
	mkdir -p fakeroot/$(DESTDIR)$(prefix)/share/man/man1
	cp bin/piaq-$(ARCH) bin/piaq
	install bin/piaq fakeroot/$(DESTDIR)$(prefix)/bin
	install man/piaq.1 fakeroot/$(DESTDIR)$(prefix)/share/man/man1

clean:
	rm bin -rf
	rm fakeroot -rf
