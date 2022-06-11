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

/* #define NTHREADS 4 */

/* struct _ThreadArgs
{
  int tid;
  int sleepTime;
};

void *myFun(void *x)
{
  struct _ThreadArgs args;
  int tid, sleepTime;
  int sum = 0;
  args = *((struct _ThreadArgs *) x);
  tid = args.tid;
  sleepTime = args.sleepTime;
#ifdef _WIN32
Sleep(sleepTime * 1000);
#else
sleep(sleepTime);
#endif
  printf("Woke up %d after seconds %d\n", tid, sleepTime);
  //Row sum and print
  for(int i = 0; i < NTHREADS; i++){
  }
  printf("The sum of row: %d is: %d\n", tid, sum);
  return NULL;
} */

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
  /* 	pthread_t threads[NTHREADS]; */
  /*   	struct _ThreadArgs thread_args[NTHREADS]; */
  int rc, i;

  c = sizeof(struct sockaddr_in);
  //! HERE IS WHERE I RECIVE THE STRINGS
  while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
  {
    puts("Connection accepted");
    // if ((recv_size = recv(new_socket, server_reply, 2000, 0)) == SOCKET_ERROR)
    // {
    //   puts("recv failed");
    // }
    // server_reply[recv_size] = '\0';
    // len = atoi(server_reply);
    // server_replyR = (char *)malloc(len);
    if ((recv(new_socket, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR)
    {
      puts("recv failed");
    }
    file test = fopen("test.txt", "w");
    fprintf(test, buffer);
    fclose(test);
    // puts("hi");
    // char hola[20];

    // if ((recv(new_socket, hola, strlen(hola), 0)) == SOCKET_ERROR)
    // {
    //   puts("recv failed");
    // }

    // puts(hola);

    // if ((recv_size = recv(new_socket, server_reply, 2000, 0)) == SOCKET_ERROR)
    // {
    //  puts("recv failed");
    // }
    // server_reply[recv_size] = '\0';
    // nSequences = atoi(server_reply);
    // for (int i = 0; i < nSequences; i++)
    // {
    /* 			if((recv_size = recv(new_socket, server_replySQ, 2000, 0)) == SOCKET_ERROR){
        puts("recv failed");
       }
       server_replySQ[recv_size] = '\0';
       strcpy(sequences[cont], server_replySQ);
       if(i == 300){
        puts(sequences[i]);
       }
       if(cont == 3){
        cont = 0;
        memset(sequences[0], '\0', strlen(sequences[0]));
        memset(sequences[1], '\0', strlen(sequences[1]));
        memset(sequences[2], '\0', strlen(sequences[2]));
        memset(sequences[3], '\0', strlen(sequences[3]));
       }else{
        cont++;
       } */
    // }
    message = "Hello socket\n";
    send(new_socket, message, strlen(message), 0);
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