#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUF_SIZE 4096

int main(){
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected.\n");

    //receive filename length
    int name_len;
    recv(client_fd, &name_len, sizeof(name_len), 0);

    //receive filename
    char filename[256] = {0};
    recv(client_fd, filename, name_len, 0);

    // Receive file size
    long long file_size;
    recv(client_fd, &file_size, sizeof(file_size), 0);

    FILE *fp = fopen(filename, "wb");

    long long received = 0;
    while(received < file_size){
        int bytes = recv(client_fd, buffer, BUF_SIZE, 0);
        fwrite(buffer, 1, bytes, fp);
        received += bytes;
    }

    printf("File received: %s (%lld bytes)\n", filename, file_size);

    fclose(fp);
    close(client_fd);
    close(server_fd);
    return 0;
}
