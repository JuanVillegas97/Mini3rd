#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <conio.h>

typedef char *string;
typedef FILE *file;

#define BUFFER_SIZE 12000000
char buffer[BUFFER_SIZE];
int main(int argc, char *argv[])
{
 // SOCKET socket_client;

 file samples;
 samples = fopen("sample.txt", "r");  //* Open file
 fgets(buffer, BUFFER_SIZE, samples); //* Store everything in buffer
 fclose(samples);

 // if (send(socket_client, sample, strlen(sample), 0) < 0) //*Here I send the sample
 // {
 //  puts("Send failed");
 //  return 1;
 // }
}
