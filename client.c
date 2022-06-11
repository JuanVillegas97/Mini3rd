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

// typedef declaration
typedef char *string;
typedef FILE *file;

// Global variables declaration
char buffer[BUFFER_SIZE];

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

 file samples;
 samples = fopen("sample.txt", "r");  //* Open file
 fgets(buffer, BUFFER_SIZE, samples); //* Store everything in buffer
 fclose(samples);

 if (send(s, buffer, BUFFER_SIZE, 0) < 0) //*Here I send the sample
 {
  puts("Send failed");
  return 1;
 }

 // FILE reading sequences //!HERE I NEED TO SEND SEQUENCES
 // file sequences;
 // sequences = fopen("sequences.txt", "r");
 // int count;
 // while (fgets(buffer, BUFFER_SIZE, sequences))
 // {
 //  // string sequence = (char *)malloc(strlen(buffer) * sizeof(char *)); // Allocates memory for it
 //  // strcpy(sequence, buffer);
 //  count++;
 // }
 // char hola[20];
 // sprintf(hola, "%d", count);
 // if (send(s, hola, strlen(hola), 0) < 0) //*Here I send the sample
 // {
 //  puts("Send failed");
 //  return 1;
 // }

 //! NOT IMPORTANT FOR NOW
 // puts("Input instruction");
 // char Instruction[100];
 // scanf("%s", Instruction);
 // while (strcmp(Instruction, "Upload_reference") != 0)
 // {
 //  puts("Invalid instruction, try again.");
 //  scanf("%s", Instruction);
 // }

 //! HERE WHERE I START SENDING THINGS OF SAMPLE
 // Sends first sequence
 // sprintf(num, "%d", len1);
 // message = num;
 // if (send(s, message, strlen(message), 0) < 0) //
 // {
 //  puts("Send failed");
 //  return 1;
 // }
 // puts("Data Sent\n");
 // message = S1;
 // if (send(s, message, strlen(message), 0) < 0)
 // {
 //  puts("Send failed");
 //  return 1;
 // }
 // puts("Data Sent\n");

 //! HERE I STARTS ENDING SEQUENCES
 // Smaller sequences reading and sending
 // FILE *ptrSeqs;
 // char *S2, ch;
 // int seqCount = 0;
 // char *fName2 = "sequences.txt";
 // ptrSeqs = fopen(fName2, "r");
 // S2 = (char *)malloc(20000);
 // //*Storing sequences
 // while (fgets(S2, strlen(S2), fName2)) //*Storing Data
 // {
 // }
 //! HERE IT ENDS THE RECIVING THING
 // Receive a reply from the server
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