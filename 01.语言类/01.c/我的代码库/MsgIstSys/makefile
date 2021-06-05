.PHONY:ALL BIN

WORKDIR=.
VPATH=$(WORKDIR)/src

CC=gcc
CFLGS=-Wall -g -I$(WORKDIR)/ink/
LIBFLAG=-L$(HOME)/lib

BIN=Client Server

ALL:$(BIN)

Client:Client.o clientop.o
	$(CC) -o $@ $^ $(LIBFLAG) -lpthread

Server:Server.o serverop.o
	$(CC) -o $@ $^ $(LIBFLAG) -lpthread

%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
