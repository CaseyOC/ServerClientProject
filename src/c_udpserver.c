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
  int udpsockfd, portno, n;
  unsigned int clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  struct sockaddr_storage serv_stor;
  socklen_t addr_size;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  //try to create the udp socket
  udpsockfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (udpsockfd < 0) error("ERROR opening socket");

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
  memset(serv_addr.sin_zero, '\0', sizeof serv_addr.sin_zero );
  char s[INET6_ADDRSTRLEN];
  const char* ip_addr;

  //bind the socket to the current IP on portno
  if (bind(udpsockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    error("Error binding to socket");
  }
  addr_size = sizeof serv_stor;
  while(1){
    bzero(buffer,256);
    n = recvfrom(udpsockfd, buffer, 255, 0,(struct sockaddr *) &serv_stor, &addr_size);
    ip_addr = inet_ntop(PF_INET, get_in_addr((struct sockaddr *) &serv_stor),s ,sizeof s);
    if (n < 0) error("ERROR reading from socket");
    printf("Message from client: %s\n",buffer);
    sendto(udpsockfd, "World!", 6, 0, (struct sockaddr *) &serv_stor, addr_size);
    printf("Responded to client at address: %s\n", ip_addr);
  }
  close(udpsockfd);
  return 0;
}
