#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 4096

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    char filename[256];
    printf("Enter file to send: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    long long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    //send filename + metadata
    int name_len = strlen(filename);
    send(sock, &name_len, sizeof(name_len), 0);
    send(sock, filename, name_len, 0);
    send(sock, &file_size, sizeof(file_size), 0);

    //send file
    char buffer[BUF_SIZE];
    long long sent = 0;

    while(sent < file_size){
        int bytes = fread(buffer, 1, BUF_SIZE, fp);
        send(sock, buffer, bytes, 0);
        sent += bytes;
    }

    printf("File sent successfully.\n");

    fclose(fp);
    close(sock);
    return 0;
}
