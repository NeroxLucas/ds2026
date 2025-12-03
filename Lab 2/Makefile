CC=mpicc
CFLAGS=-O2

all: transfer

transfer: transfer.c
	$(CC) $(CFLAGS) transfer.c -o transfer

clean:
	rm -f transfer *.o output.bin
