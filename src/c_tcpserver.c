#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(char *msg){
  perror(msg);
  exit(1);
}

void * get_in_addr(struct sockaddr* sa){
  if(sa->sa_family == AF_INET){
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[]){
  //initialize variables
  int tcpsockfd, newsockfd, portno, n;
  unsigned int clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  //try to create the tcp socket
  tcpsockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (tcpsockfd < 0) error("ERROR opening socket");

  //clear the address struct
  bzero((char*) &serv_addr, sizeof(serv_addr));
  if (atoi(argv[1]) != 0){
    if(atoi(argv[1]) < 2000 || atoi(argv[1]) > 65535){
      fprintf(stderr,"Invalid port number: %s\n", argv[1]);
      _exit(0);
    }
    else
      portno = atoi(argv[1]);
  }

  //setup host_addr struct
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  //convert int to port byte
  serv_addr.sin_port = htons(portno);
  char s[INET6_ADDRSTRLEN];
  const char* ip_addr;
  char hostname[256];
  hostname[255] = '\0';
  gethostname(hostname,255);
  int tcp, udp;
  tcp = 0;
  udp = 0;

  //bind the socket to the current IP on portno
  if (bind(tcpsockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    error("Error binding to socket");
  }

  //tell the tcp socket to listen for incoming connections
  listen(tcpsockfd, 5);
  printf("Listening on hostname: %s port: %d\n", hostname, portno);

  clilen = sizeof(cli_addr);
  while(1){
    newsockfd = accept(tcpsockfd, (struct sockaddr *) &cli_addr, &clilen);
    //accept ^ returns a new socket fd, tcpsockfd used to establish new connections,
    //and can communicate with newsockfd
    ip_addr = inet_ntop(AF_INET, get_in_addr((struct sockaddr *) &cli_addr),s ,sizeof s);
    if (newsockfd < 0)error("ERROR on accept");

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Message from client: %s\n",buffer);
    n = write(newsockfd,"World!",10);
    printf("Responded to client at address: %s\n", ip_addr);
    if (n < 0) error("ERROR writing to socket");

  }
  close(newsockfd);
  return 0;
}
