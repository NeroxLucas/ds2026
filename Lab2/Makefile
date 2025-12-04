CFLAGS = -I/usr/include/tirpc
LDFLAGS = -ltirpc

all: server client

server: file_transfer_svc.c file_transfer_xdr.c file_transfer_server.c
	gcc $(CFLAGS) -o server file_transfer_svc.c file_transfer_xdr.c file_transfer_server.c $(LDFLAGS)

client: file_transfer_clnt.c file_transfer_xdr.c file_transfer_client.c
	gcc $(CFLAGS) -o client file_transfer_clnt.c file_transfer_xdr.c file_transfer_client.c $(LDFLAGS)

clean:
	rm -f server client *.o
