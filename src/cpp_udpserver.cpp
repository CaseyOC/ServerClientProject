#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>

void * get_in_addr(struct sockaddr* sa){
  if(sa->sa_family== AF_INET){
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char* argv[]){
  int status, udpsockfd;
  char* portno;
  struct addrinfo hints, *res;

  if (argc < 2){
    printf("No port provided");
    _exit(1);
  }
  if (atoi(argv[1]) != 0){
    if(atoi(argv[1]) < 2000 || atoi(argv[1]) > 65535){
      fprintf(stderr,"Invalid port number: %s\n", argv[1]);
      _exit(0);
    }
    else
      portno = argv[1];
  }

  //empty hints
  memset(& hints, 0, sizeof hints);
  //set the attribute for hint
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  char hostname[256];
  hostname[255] = '\0';
  gethostname(hostname,255);

  //fill the res structure
  status = getaddrinfo(NULL, portno , &hints, &res);
  if(status != 0){
    fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
  }
  //create Socket
  udpsockfd = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
  if(udpsockfd < 0 ){
    fprintf(stderr,"socket error: %s\n",gai_strerror(status));
  }
  //bind the socket to local machine and port number
  status = bind(udpsockfd, res->ai_addr, res->ai_addrlen);
  if(status < 0){
    fprintf(stderr,"bind: %s\n",gai_strerror(status));
  }
  //free the res linked list
  freeaddrinfo(res);

  int n;
  char buffer[256];
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  char s[INET6_ADDRSTRLEN];
  addr_size = sizeof client_addr;
  const char* ip_addr;

  while(1){
    n = recvfrom(udpsockfd, buffer, 255, 0,(struct sockaddr *) &client_addr, &addr_size);
    ip_addr = inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s);
    std::cout<<"Message from client: " << buffer << std::endl;
    sendto(udpsockfd, "World!", 6, 0, (struct sockaddr *) &client_addr, addr_size);
    std::cout<<"Responded to client at address: " << ip_addr << std::endl;
  }
  //close the socket
  close(udpsockfd);
  return 0;
}
