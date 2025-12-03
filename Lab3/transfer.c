#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK 4096
#define TAG_DATA 0
#define TAG_END  1

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0)
            printf("This program requires at least 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    char buffer[CHUNK];

    if (rank == 0) {
        // -------------------------------
        // Rank 0 = Sender
        // -------------------------------
        FILE* f = fopen("input.bin", "rb");
        if (!f) {
            perror("Cannot open input.bin");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int bytes;
        while ((bytes = fread(buffer, 1, CHUNK, f)) > 0) {
            MPI_Send(&bytes, 1, MPI_INT, 1, TAG_DATA, MPI_COMM_WORLD);
            MPI_Send(buffer, bytes, MPI_BYTE, 1, TAG_DATA, MPI_COMM_WORLD);
        }

        // End-of-file message
        int zero = 0;
        MPI_Send(&zero, 1, MPI_INT, 1, TAG_END, MPI_COMM_WORLD);

        fclose(f);
        printf("[Rank 0] File sent successfully.\n");
    }

    else if (rank == 1) {
        // -------------------------------
        // Rank 1 = Receiver
        // -------------------------------
        FILE* f = fopen("output.bin", "wb");
        if (!f) {
            perror("Cannot open output.bin");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int bytes;
        MPI_Status status;

        while (1) {
            MPI_Recv(&bytes, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == TAG_END)
                break;

            MPI_Recv(buffer, bytes, MPI_BYTE, 0, TAG_DATA, MPI_COMM_WORLD, &status);
            fwrite(buffer, 1, bytes, f);
        }

        fclose(f);
        printf("[Rank 1] File received successfully.\n");
    }

    MPI_Finalize();
    return 0;
}
