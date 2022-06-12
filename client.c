/*
 Create a TCP socket
*/
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

#define BUFFER_SIZE 12000000
#define NOTIF_BUFFER_SIZE 15

// typedef declaration
typedef char *string;
typedef FILE *file;

// Global variables declaration
char buffer[BUFFER_SIZE];
char notif_buffer[NOTIF_BUFFER_SIZE];

int main(int argc, char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message, server_reply[2000], num[50], num2[50]; //! delete these later
    int recv_size;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
    printf("Failed. Error Code : %d", WSAGetLastError());
    return 1;
    }

    printf("Initialised.\n");

    // Create a socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
    printf("Could not create socket : %d", WSAGetLastError());
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(4040);

    // Connect to remote server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
    puts("connect error");
    return 1;
    }

    puts("Connected");

    //! HERE I START SENDING
    //!----------------------------------------------------------------------
    file samples;
    samples = fopen("sample.txt", "r");  //* Open file
    fgets(buffer, BUFFER_SIZE, samples); //* Store everything in buffer
    fclose(samples);
    if (send(s, buffer, BUFFER_SIZE, 0) < 0) //! SENDING SAMPLE
    {
    puts("Send failed");
    return 1;
    }
    //!----------------------------------------------------------------------
    //! SENDING NUM OF SEQUENCES
    //!----------------------------------------------------------------------
    file sequences;
    sequences = fopen("sequences.txt", "r");
    int numSquences = 0;
    while (fgets(buffer, BUFFER_SIZE, sequences))
    {
    numSquences++;
    }
    sprintf(buffer, "%d", numSquences);
    printf("numSeq client: %d\n", numSquences);
    if (send(s, buffer, BUFFER_SIZE, 0) < 0) //! SENDING NUM OF SEQUENCES
    {
    puts("Send failed");
    return 1;
    }else{
        puts("numSeq sent");
    }
    fclose(sequences);
    sequences = fopen("sequences.txt", "r");
 //!----------------------------------------------------------------------
    int i = 0;
    while (fgets(buffer, BUFFER_SIZE, sequences))
    {
        printf("entered while @%d\n", i++);
        if (send(s, buffer, BUFFER_SIZE, 0) < 0) //! SENDING EACH SEQUENCE
        {
            puts("Send failed");
            return 1;
        }else{
            puts("Send succeeded");
        }
    }

    if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR)
    {
    puts("recv failed");
    }

    puts("Reply received\n");

    // Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);

    closesocket(s);
    WSACleanup();

    return 0;
}