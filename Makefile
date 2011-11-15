CC=g++
CFLAGS=-Wall -c -I. -Idriver #-std=c++0x
LDFLAGS=-L. -lrelaisdriver -lpcre

SOURCES=main.cpp relaisctrl.cpp argumentparser.cpp commands.cpp

OBJECTS=$(SOURCES:.cpp=.o)

PROGRAM=relaisctrl

all: driver $(SOURCES) $(PROGRAM)

driver: librelaisdriver.so

librelaisdriver.so:
	$(CC) -fPIC $(CFLAGS) driver/relaisdriver.cpp -o driver/relaisdriver.o
	$(CC) -shared -Wl,-soname,librelaisdriver.so -o librelaisdriver.so driver/relaisdriver.o

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAM) *.o driver/*.o lib/*.so *.so *~


