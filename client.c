//
// Created by allec on 05.03.18.
//

#include <stdio.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024



int mySocket = 0;
struct sockaddr_in adres;

void* myT(void* args){

    while (1){
        char* readBuf = malloc(BUF_SIZE);
        int len = sizeof(adres);
        if(recvfrom(mySocket, readBuf, BUF_SIZE, 0, (struct sockaddr *) &adres, (socklen_t *) &len) == -1){
            perror("MESSAGE RECEIVED");
            exit(1);
        }

        printf("%s", readBuf);
    }

}



int main(int argc, char *argv[]) {
    pthread_t tid;

    memset(&adres, 0, sizeof(adres));
    adres.sin_family = AF_INET;
    adres.sin_port = htons((uint16_t) atoi(argv[2]));
    adres.sin_addr.s_addr = inet_addr(argv[1]);
    if( (mySocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("SOCKET CREATE");
        exit(1);
    }

    pthread_create(&tid, NULL, myT, NULL);
    int slen = sizeof(adres);
    while(1){
        char* buf = malloc(BUF_SIZE);
        fgets(buf, BUF_SIZE, stdin);

        if(sendto(mySocket, buf, BUF_SIZE, 0, (struct sockaddr*) &adres,
                                    (socklen_t) slen) < 0){
            perror("MSG SENT");
            exit(1);
        }

        free(buf);

    }
}