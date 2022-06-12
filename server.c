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
//#define NTHREADS 5

// typedef declaration
typedef char *string;
typedef FILE *file;

// Global variables declaration
char buffer[BUFFER_SIZE];

// struct declaration
struct _ThreadArgs
{
  int tid;
  //int sleepTime;
  int posSequence;
};

void *myFun(void *x)
{
  struct _ThreadArgs args;
  args = *((struct _ThreadArgs *) x);
  printf("entré a myFun no %d", args.tid);

  /*

    sample: string de la muestra
    sequence: string de la secuencia a buscar
    posSequence: 
  */

  //opens both files and saves address of start of sequence  
  file samp1_file = fopen("sample_1.txt", "r");
  file seq1_file = fopen("sequences_1.txt", "r");
  void* start_seq1_file = seq1_file;
  
  //saves sample in string
  char sample[BUFFER_SIZE];
  fgets(sample, BUFFER_SIZE, samp1_file);

  //moves cursor to sequence start
  fseek(seq1_file, args.posSequence, SEEK_SET);

  //saves sequence in string
  void* seq_found;
  char sequence[BUFFER_SIZE];
  fgets(sequence, BUFFER_SIZE, seq1_file);
  
  //searches substring in string 
  seq_found = strstr(sample, sequence);

  //was substring found?
  if (seq_found != NULL)
  {
    //if yes, say in which pos
    int pos_in_sequence = seq_found - start_seq1_file; 
    printf("Found sequence no %d at pos %d\n", args.tid, pos_in_sequence);
  }else
  {
    printf("didn't find sequence no %d\n", args.tid);
  }
  
  //printf("Woke up %d after seconds %d\n", tid, sleepTime);
  
  fclose(samp1_file);

  return NULL;
}

int main(int argc, char *argv[])
{
  WSADATA wsa;
  SOCKET s, new_socket;
  struct sockaddr_in server, client;
  int c, recv_size, len, nSequences, sizes[4], cont = 0;
  char *message, *posStr, server_reply[2000], *server_replyR, seq[20000], server_replySQ[19000], sequences[4][19000];

  posStr = (char*)malloc(sizeof(long));

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
    //open a file in server and save sample from client message in that file
    file samp1_file = fopen("sample_1.txt", "w");
    fprintf(samp1_file, "%s", buffer);
    fclose(samp1_file);
    //!----------------------------------------------------------------------
    //!----------------------------------------------------------------------
    if ((recv(new_socket, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) //! RECIVING NUM OF SEQUENCES
    {
      puts("recv failed");
    }
    int numSequences = atoi(buffer);
    printf("numSeq server: %d\n", numSequences);
    long posSequences[numSequences]; //array for pointers to the start of each sequence in file
    //! Receiving sequences
    //!----------------------------------------------------------------------
    file seq1_file = fopen("sequences_1.txt", "a");
    for (int i = 0; i < numSequences; i++)
    {
      printf("entered for @%d\n", i);
      //recieve sequence and save it in buffer
      if (recv(new_socket, buffer, BUFFER_SIZE, 0) == SOCKET_ERROR)
      {
        puts("recv failed");
      }
      //receive sequence's address(position) in file
      if (recv(new_socket, posStr, 8, 0) == SOCKET_ERROR)
      {
        puts("recv failed");
      }
      printf("posStr: %s\n", posStr);
      fputs(buffer, seq1_file);
      posSequences[i] = atol(posStr);
      printf("seq %d received\n", i);
      //printf("posSeq: %ld\n", posSequences[i]);

    }
    //!----------------------------------------------------------------------


    int NTHREADS = numSequences;
    pthread_t threads[NTHREADS];
    struct _ThreadArgs thread_args[NTHREADS];
    int rc, i;

    /* spawn the threads */
    for (i=0; i<NTHREADS; ++i)
      {
        //int sleepTime = rand() % 10;
        thread_args[i].tid = i;
        //thread_args[i].sleepTime = sleepTime;
        thread_args[i].posSequence = posSequences[i];
        printf("spawning thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, myFun, (void *) &thread_args[i]); //!check max num of threads
      }

    /* wait for threads to finish */
    for (i=0; i<NTHREADS; ++i) {
      rc = pthread_join(threads[i], NULL);
    }

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