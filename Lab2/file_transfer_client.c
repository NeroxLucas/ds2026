#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_transfer.h"

int main(int argc, char *argv[])
{
    CLIENT *clnt;
    FILE *fp;
    char buffer[8192];
    FileChunk chunk;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_host> <filename>\n", argv[0]);
        exit(1);
    }

    // Create RPC client
    clnt = clnt_create(argv[1], FILETRANSFER_PROG, FILETRANSFER_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror("clnt_create");
        exit(1);
    }

    fp = fopen(argv[2], "rb");
    if (!fp) {
        perror("client fopen");
        exit(1);
    }

    while (1) {
        chunk.filename = argv[2];
        chunk.size = fread(buffer, 1, sizeof(buffer), fp);

        chunk.data.data_val = buffer;
        chunk.data.data_len = chunk.size;

        chunk.last = (chunk.size == 0);

        int *result = send_chunk_1(&chunk, clnt);

        if (!result || *result == 0) {
            fprintf(stderr, "send_chunk RPC failed\n");
            break;
        }

        if (chunk.last)
            break;
    }

    fclose(fp);
    clnt_destroy(clnt);

    printf("[Client] File sent!\n");
    return 0;
}
