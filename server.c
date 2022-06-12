/*
 Live Server on port 8888
*/
// Use linker flag  -lws2_32
#include <stdio.h>
#include <io.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
// Linux will use <sockets.h>
// #include<sys/socket.h>

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
  SOCKET s, new_socket;
  struct sockaddr_in server, client;
  int c, recv_size, len, nSequences, sizes[4], cont = 0;
  char *message, server_reply[2000], *server_replyR, seq[20000], server_replySQ[19000], sequences[4][19000];

  // Initializes the library
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("Failed. Error Code : %d", WSAGetLastError());
    return 1;
  }

  printf("Library initialized\n");

  // Create a socket
  //  Type stream tcp
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("Error creating socket : %d", WSAGetLastError());
  }

  printf("Socket created.\n");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(4040);

  // Bind
  if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
  {
    printf("Bind error : %d", WSAGetLastError());
    exit(EXIT_FAILURE);
  }

  puts("Bound");
  listen(s, 3);
  puts("Waiting for connections...");

  c = sizeof(struct sockaddr_in);

  //! HERE IS WHERE I RECIVING
  while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
  {
    puts("Connection accepted");
    if ((recv(new_socket, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) //! RECIVING SAMPLE
    {
      puts("recv failed");
    }
    file test = fopen("sample_1.txt", "w");
    fprintf(test, "%s", buffer);
    fclose(test);
    //!----------------------------------------------------------------------
    //!----------------------------------------------------------------------
    if ((recv(new_socket, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) //! RECIVING NUM OF SEQUENCES
    {
      puts("recv failed");
    }
    int numSquences = atoi(buffer);
    printf("numSeq server: %d\n", numSquences);
    //! Receiving sequences
    //!----------------------------------------------------------------------
    file test_1 = fopen("sequences_1.txt", "a");
    for (int i = 0; i < numSquences; i++)
    {
      printf("entered for @%d\n", i);
      if (recv(new_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR)
      {
        puts("recv failed");
      }
      fputs(buffer, test_1);
      printf("seq %d received\n", i);
    }
    //!----------------------------------------------------------------------
    message = "Hello socket\n";
    send(new_socket, message, strlen(message), 0);

    /* 	pthread_t threads[NTHREADS]; */
    /*   	struct _ThreadArgs thread_args[NTHREADS]; */
    int rc, i;

  }

  if (new_socket == INVALID_SOCKET)
  {
    printf("accept failed with error code : %d", WSAGetLastError());
    return 1;
  }

  closesocket(s);
  WSACleanup();

  return 0;
}