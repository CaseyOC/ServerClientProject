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
  if(sa->sa_family == AF_INET){
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char* argv[]){
  int status, listener;
  char* portno;
  struct addrinfo hints, *res;

  if (argc < 2){
    printf("No port provided");
    _exit(1);
  }
  if (atoi(argv[1]) != 0){
   portno = argv[1];
  } else{
    fprintf(stderr,"Invalid port number: %s\n", argv[1]);
    _exit(0);
  }

  //empty hints
  memset(& hints, 0, sizeof hints);
  //set the attribute for hint
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
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
  listener = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
  if(listener < 0 ){
    fprintf(stderr,"socket error: %s\n",gai_strerror(status));
  }
  //bind the socket to local machine and port number
  status = bind(listener, res->ai_addr, res->ai_addrlen);
  if(status < 0){
    fprintf(stderr,"bind: %s\n",gai_strerror(status));
  }
  status = listen(listener, 10);
  if(status < 0){
    fprintf(stderr,"listen: %s\n",gai_strerror(status));
  }
  printf("Now listening on host: %s port: %s\n", hostname, portno);
  //free the res linked list
  freeaddrinfo(res);

  int new_conn_fd, n;
  char buffer[256];
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  char s[INET6_ADDRSTRLEN];
  addr_size = sizeof client_addr;
  const char* ip_addr;

  while(1){
    //accept a new connection and return back the socket desciptor
    new_conn_fd = accept(listener, (struct sockaddr *) & client_addr, &addr_size);
    if(new_conn_fd < 0){
      fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
    }
    ip_addr = inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s);

    n = read(new_conn_fd, buffer, 256);

    printf("Message from client: %s \n",buffer);
    status = write(new_conn_fd,"World!\n", 6);
    if(status == -1){
      close(new_conn_fd);
      _exit(4);
    }
    printf("Responded to client at address: %s\n", ip_addr);
  }
  //close the socket
  close(new_conn_fd);
  return 0;
}
