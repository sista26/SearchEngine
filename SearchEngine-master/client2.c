#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/socket.h>
int main(int argc, char *argv[]){
  char message[1000];
  int server, portNumber;
  char *pos;
  socklen_t len;
  int n1;
  struct sockaddr_in servAdd;
 if(argc != 3){
  printf("Call model:%s <IP> <Port#>\n",argv[0]);
  exit(0);
 }
 if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
  fprintf(stderr, "Cannot create socket\n");
  exit(1); 
 }
 servAdd.sin_family = AF_INET;
 sscanf(argv[2], "%d", &portNumber);
 servAdd.sin_port = htons((uint16_t)portNumber);
 if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
  fprintf(stderr, "inet_pton() has failed\n");
  exit(2);
 }
 if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }
 while(1){
  fprintf(stderr, "Enter the \"quit\" sign to quit or a message for the server\n");
  fgets(message, 254, stdin);
  if ((pos=strchr(message, '\n')) != NULL)
     *pos = '\0';
  if(strcmp(message, "quit") == 0){
    close(server);
    fprintf(stderr, "Closing the connection\n");
    exit(0);
    break;
  }
  write(server, message, strlen(message)+1);
  fprintf(stderr, "Server's reply:\n");
  if (read(server, message, 10000)<0){
        fprintf(stderr, "read() error\n");
        exit(3);
      }
      // while(read(server, message, 100)>0)
        printf("\nOutput:\n");
        fprintf(stderr, "%s\n", message);
        memset(&message[0], 0, sizeof(message));
  /*while(1){
  n1=read(server, message, 1);
  if(n1)
  {
   fprintf(stderr, "%s", message); 
   }
  if (n1 == -1)
  {
    fprintf(stderr, "reading error\n");
    exit(0);
  }
  memset(&message[0], 0, sizeof(message));
 }*/
 
 }
}
