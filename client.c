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
    char* posSeq = (char*)malloc(sizeof(int));
    long cursorSeq = 0;
    int bytes_send=0;
    // read sequences file line by line
    // rewrite each line in buffer every time
    while (fgets(buffer, BUFFER_SIZE, sequences))
    {
        printf("entered while @%d\n", i++);
        if (send(s, buffer, BUFFER_SIZE, 0) < 0) //! SENDING EACH SEQUENCE
        {
            puts("Send failed");
            return 1;
        }else{
            puts("seq Send succeeded");
        }

        // save pos of cursor for end of previous line
        sprintf(posSeq, "%ld", cursorSeq);

        bytes_send = send(s, posSeq, 8, 0);
        if (bytes_send < 0) //! SENDING POS OF SEQUENCE IN FILE IN MEMORY
        {
            puts("Send failed");
            return 1;
        }else{
            printf("posSeq: %s\n", posSeq);
            puts("pos Send succeeded");
        }
        printf("bytes_send: %d\n", bytes_send);

        //save cursor at end of sequence to save pos of next line in next while iteration
        cursorSeq = ftell(sequences);
    }


    // Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);

    closesocket(s);
    WSACleanup();

    return 0;
}