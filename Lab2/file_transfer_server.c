#include "file_transfer.h"
#include <stdio.h>

int *
send_chunk_1_svc(FileChunk *chunk, struct svc_req *rqstp)
{
    static int result = 1;
    static FILE *fp = NULL;

    // If last chunk, close file and finish
    if (chunk->last == 1) {
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }
        printf("[Server] Transfer complete.\n");
        return &result;
    }

    // If this is the first chunk, open the output file
    if (fp == NULL) {
        fp = fopen(chunk->filename, "wb");
        if (!fp) {
            perror("server fopen");
            result = 0;
            return &result;
        }
        printf("[Server] Receiving file: %s\n", chunk->filename);
    }

    // Write chunk to file
    fwrite(chunk->data.data_val, 1, chunk->size, fp);

    return &result;
}
