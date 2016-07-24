#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(char *msg){
  perror(msg);
  exit(0);
}

int hostname_to_ip(char* hostname , char* ip){
  struct hostent* he;
  struct in_addr** addr_list;
  int i;

  if ( (he = gethostbyname( hostname ) ) == NULL) {
    // get the host info
    herror("Failed to resolve hostname\n");
    return 1;
  }

  addr_list = (struct in_addr **) he->h_addr_list;

  for(i = 0; addr_list[i] != NULL; i++) {
    //Return the first one;
    strcpy(ip , inet_ntoa(*addr_list[i]) );
    return 0;
  }
  return 1;
}

int main(int argc, char *argv[]){
  int tcpsockfd, udpsockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent* he;
  struct in_addr** addr_list;
  char ip[50];
  char buffer[255];

  if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }
  if (atoi(argv[2]) != 0){
    portno = atoi(argv[2]);
  }
  else{
    fprintf(stderr,"Invalid port number: %s\n", argv[2]);
    _exit(0);
  }

  //try to create the tcp socket
  tcpsockfd = socket(AF_INET, SOCK_STREAM, 0);
  //try to create the udp socket
  udpsockfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (udpsockfd < 0 && tcpsockfd < 0) error("ERROR opening socket");


  if(inet_addr(argv[1]) == -1){
    if ((he = gethostbyname(argv[1])) == NULL){
      herror("gethostbyname");
      printf("Failed to resolve hostname\n");
      _exit(0);
    }
    addr_list = (struct in_addr**) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++){
      serv_addr.sin_addr = *addr_list[i];
    }
  }
  else{
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)he->h_addr, (char *)&serv_addr.sin_addr.s_addr, he->h_length);
  serv_addr.sin_port = htons(portno);
  if (tcpsockfd != -1){
    if (connect(tcpsockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
      error("ERROR connecting");
      _exit(0);
    }
    //Connect to remote server
    printf("Connected\n");
  }

  //while(1){
    printf("Message to server: ");
    bzero(buffer,255);
    fgets(buffer,254,stdin);
    if (tcpsockfd != -1){
      n = write(tcpsockfd,buffer,strlen(buffer));
      if (n < 0) error("ERROR writing to socket");
      bzero(buffer,255);
      n = read(tcpsockfd,buffer,255);
      buffer[(n > 0) ? n : 0] = '\0';
      if (n < 0) error("ERROR reading from socket");
      printf("%s\n",buffer);

    }
    else if( udpsockfd != -1){
      sendto(udpsockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
      bzero(buffer,255);
      n = recvfrom(udpsockfd, buffer, 255, 0, NULL, NULL);
      buffer[(n > 0) ? n : 0] = '\0';
      printf("%s\n",buffer);
    }
    else{
      printf("Connection error");
      _exit(0);
    }
  //}

  return 0;
}
