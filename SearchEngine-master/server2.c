#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
void serviceClient(int sd){
  char message[255];
  char *pos;
  int pid;
  int status;
  dup2(sd, STDOUT_FILENO);
  while(1){
   
   fprintf(stderr, "new command\n");
    if(!read(sd, message, 255)){
    close(sd);
    fprintf(stderr, "Bye, client dead, wait for a new client\n");
    exit(0); 
   }
    
     if ((pos=strchr(message, '\n')) != NULL)
     *pos = '\0';
     fprintf(stderr, "client requested for %s command\n", message);
    // system(message);
     //fflush(stdout);
     
     pid = fork();
     if ( pid == -1 ) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
      system(message);
      memset(&message[0], 0, sizeof(message));
      exit(EXIT_FAILURE);
    }
     else{
       if(waitpid(pid, &status, 0)!= -1){
          if ( WIFEXITED(status) ) {
                int returned = WEXITSTATUS(status);
                //fprintf(stderr, "Exited normally with status %d\n", returned);
                
                fflush(stdout);
               //exit(0);
            }
            else if ( WIFSIGNALED(status) ) {
                int signum = WTERMSIG(status);
                fprintf(stderr, "Exited due to receiving signal %d\n", signum);
            }
            else if ( WIFSTOPPED(status) ) {
                int signum = WSTOPSIG(status);
                printf("Stopped due to receiving signal %d\n", signum);
            }
            else {
                printf("Something strange just happened.\n");
            }
       }
	     else {
            perror("waitpid() failed");
            exit(EXIT_FAILURE);
        }
     }

   }
  // fprintf(stderr, "nextline\n");

  
  close(sd);
 }

int main(int argc, char *argv[]){
 int sd, client, portNumber;
 socklen_t len;
 struct sockaddr_in servAdd;
 if(argc != 2){
    printf("Call model: %s <Port #>\n", argv[0]);
    exit(0);
 }
 if ((sd=socket(AF_INET,SOCK_STREAM,0))<0){
   fprintf(stderr, "Cannot create socket\n");
 exit(1); 
 }
 servAdd.sin_family = AF_INET;
 servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
 sscanf(argv[1], "%d", &portNumber);
 servAdd.sin_port = htons((uint16_t)portNumber);
 bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
 listen(sd, 5);
 while(1){
    fprintf(stderr, "Waiting for clients\n");
    client=accept(sd,(struct sockaddr*)NULL,NULL);
    printf("Got a client\n");
    if(!fork())
     serviceClient(client);
    close(client);

 } 
}
