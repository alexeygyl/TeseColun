CC=gcc
CFLAGS=-c
LDFLAGS=-g -Wall -lpthread -lasound -lm audio.c
SOURCES = deamon.c arg.c socketUdpClient.c start.c sockfuncs.c thrd.c funcs.c
OBJECTS = $(SOURCES:.c=.o)
BINARY = binary




all:$(SOURCES) $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -f ./*.o

.c.o:
	$(CC) $(CFLAGS)   $< -o $@ 

clean:
	rm -f ./*.o

install:
	cp -f ./binary /bin/audio-client
