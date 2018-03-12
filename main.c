
// client - serwer przy pomocy datagramów

#include <stdio.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define CLIENTS_NUMBER 10
#define BUF_SIZE 1024

int main(int argc, char* argv[]) {

    struct sockaddr_in address;

    memset(&address, 0, sizeof(struct sockaddr_in));

    address.sin_family = AF_INET;
    address.sin_port = htons((uint16_t) atoi(argv[1]));
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int mySocket = 0;
    if( (mySocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("SOCKET CREATE");
        exit(1);
    }

    if( bind(mySocket, (const struct sockaddr *) &address, sizeof(struct sockaddr)) == -1){
        perror("SOCKET BIND");
        exit(1);
    }

    struct sockaddr_in clients[CLIENTS_NUMBER];
    for(int i = 0; i < CLIENTS_NUMBER; i++){
        memset(&clients[i], 0, sizeof(struct sockaddr_in));
    }

    int clientsConnected = 0;
    while (1) {

        char* clientBuf = malloc(BUF_SIZE);

        struct sockaddr_in client;
        memset(&client, 0, sizeof(struct sockaddr_in));
        int clientLen = sizeof(client);

        size_t messageLength = 0;
        if((messageLength = (size_t) recvfrom(mySocket, clientBuf, BUF_SIZE, 0, (struct sockaddr *) &client, (socklen_t *) &clientLen)) == -1){
            perror("MESSAGE RECEIVED");
            exit(1);
        }

        int clientTalking = -1;
        if(clientsConnected == 0){
            clients[clientsConnected] = client;
            clientTalking = clientsConnected;
            clientsConnected++;
            printf("Client connected!");
        } else if(clientsConnected == 9){
            printf("Server full!!!\n");
            char* newBuf = "Server full!";
            if(sendto(mySocket, newBuf, messageLength, 0, (struct sockaddr*) &client,
                      (socklen_t) clientLen) < 0){
                perror("MSG SENT");
                exit(1);
            }
        }{
            int contains = 0;
            for(int i = 0; i < clientsConnected; i++){
                if( clients[i].sin_port == client.sin_port ){
                    clientTalking = i;
                    contains = 1;
                }
            }

            if(contains == 0){
                clients[clientsConnected] = client;
                clientTalking = clientsConnected;
                clientsConnected++;
                printf("Client connected!");
            }
        }

        printf(" Clients: %d\n", clientsConnected);

        for(int i = 0; i < clientsConnected; i++){
            if(i != clientTalking){
                if(sendto(mySocket, clientBuf, messageLength, 0, (struct sockaddr*) &clients[i],
                          (socklen_t) clientLen) < 0){
                    perror("MSG SENT");
                    exit(1);
                }
            }
        }
    }
}