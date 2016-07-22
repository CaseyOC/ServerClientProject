#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]){
  //initialize variables
  int sockfd, newsockfd, portno;
  unsigned int clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  //create the socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  //clear the address struct
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);

  //setup host_addr struct
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  //convert int to port byte
  serv_addr.sin_port = htons(portno);

  //bind the socket to the current IP on portno
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  //tell the socket to listen to incoming connections and set the max size for
  //backlog queue to 5
  listen(sockfd, 5);

  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  //accept ^ returns a new socket fd, sockfd used to establish new connections,
  //and can communicate with newsockfd
  if (newsockfd < 0)
    error("ERROR on accept");
  bzero(buffer,256);
  n = read(newsockfd,buffer,255);
  if (n < 0) error("ERROR reading from socket");
  printf("Message from client: %s\n",buffer);
  n = write(newsockfd,"World!",10);
  printf("Responded to client\n");
  if (n < 0) error("ERROR writing to socket");

  return 0;
}
