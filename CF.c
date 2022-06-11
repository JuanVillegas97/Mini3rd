/*
 Create a TCP socket
*/
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

typedef char *string;
#define BUFFER_SIZE 4096
static char buffer[BUFFER_SIZE]; // Creating a buffer to store strings

int main(int argc, char *argv[])
{
 WSADATA wsa;
 SOCKET s;
 struct sockaddr_in server;
 char *message, server_reply[2000], num[50], num2[50];
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

 if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
 {
		puts("connect error");
		return 1;
 }

 puts("Connected");

 //! FILE reading sample
 FILE *ptrSample;
 string sample;
 int sLenght;

 ptrSample = fopen("sample.txt", "r"); //*Opens de file
 fseek(ptrSample, 0, SEEK_END);								//*Sets the pointer at the end of the file
 sLenght = ftell(ptrSample);											//*Gets the lenght of the file string
 sample = (char *)malloc(sLenght);					//*Allocate memory for the message
 fclose(ptrSample);

 ptrSample = fopen("sample.txt", "r"); //*Opens de file
 fgets(sample, sLenght, ptrSample);				//*Reads all the file and store the message
 fclose(ptrSample);																				//*Close the file

 //! Connect to remote server THIS IS ONE IT IS THE INSTRUCTION DO NO CHANGE IT'S LIKE FILTER
 // puts("Input instruction");
 // char Instruction[100];
 // scanf("%s", Instruction);
 // while (strcmp(Instruction, "Upload_reference") != 0)
 // {
 // 	puts("Invalid instruction, try again.");
 // 	scanf("%s", Instruction);
 // }

 //! Sends first sample
 sprintf(buffer, "%d", sLenght); //*Converts the size of the file in a string
 if (send(s, buffer, sLenght, 0) < 0)
 {
		puts("Send failed");
		return 1;
 }

 // puts("Data Sent\n");
 // message = S1;
 // if (send(s, message, strlen(message), 0) < 0)
 // {
 // 	puts("Send failed");
 // 	return 1;
 // }
 // puts("Data Sent\n");

 // // Smaller sequences reading and sending
 // FILE *ptrSeqs;
 // char *S2, ch;
 // int seqCount = 0;
 // char *fName2 = "s_cerevisia_2022_01.seq";
 // ptrSeqs = fopen(fName2, "r");
 // S2 = (char *)malloc(20000);
 // while (!feof(ptrSeqs))
 // {
 // 	fgets(S2, 20000, ptrSeqs);
 // 	seqCount++;
 // }
 // fclose(ptrSeqs);
 // sprintf(num2, "%d", seqCount);
 // message = num2;
 // if (send(s, message, strlen(message), 0) < 0)
 // {
 // 	puts("Send failed");
 // 	return 1;
 // }
 // ptrSeqs = fopen(fName2, "r");
 // for (int i = 0; i < seqCount; i++)
 // {
 // 	fgets(S2, 20000, ptrSeqs);
 // 	message = S2;
 // 	if (i == 0 || i == 10 || i == 257)
 // 	{
 // 		puts(message);
 // 	}
 // 	if (send(s, message, strlen(message), 0) < 0)
 // 	{
 // 		puts("Send failed");
 // 		return 1;
 // 	}
 // 	memset(S2, '\0', strlen(S2));
 // }

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