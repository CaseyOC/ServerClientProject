#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>

class udp_client{
public:
  udp_client();
  bool send_data(std::string address, int port, std::string data);
  std::string receive(int);
private:
  int sock;
  std::string address;
  int port;
  struct sockaddr_in server;
};

udp_client::udp_client(){
  sock = -1;
  port = 0;
  address = "";
}

//Send data to the host
bool udp_client::send_data(std::string address , int port, std::string data){
  if(sock == -1){
    //Create socket
    sock = socket(PF_INET , SOCK_DGRAM , 0);
    if (sock == -1){
      perror("Could not create socket");
    }
  }else{}
  //setup address structure
  if(inet_addr(address.c_str()) == -1){
    struct hostent *he;
    struct in_addr **addr_list;

    //resolve the hostname, its not an ip address
    if ( (he = gethostbyname( address.c_str() ) ) == NULL){
      //gethostbyname failed
      herror("gethostbyname");
      std::cout<<"Failed to resolve hostname\n";
      return false;
    }
    //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
    addr_list = (struct in_addr **) he->h_addr_list;
    for(int i = 0; addr_list[i] != NULL; i++){
      //strcpy(ip , inet_ntoa(*addr_list[i]) );
      server.sin_addr = *addr_list[i];
      break;
    }
  }else{
    server.sin_addr.s_addr = inet_addr( address.c_str() );
  }
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  //Send some data
  int status = sendto(sock,data.c_str(),strlen(data.c_str()),0,(struct sockaddr *)&server, sizeof(server));
  if( status < 0){
    perror("Send failed : ");
    return false;
  }
  return true;
}
//Receive data from the connected host
std::string udp_client::receive(int size=512){
  char buffer[size];
  std::string reply;
  //Receive a reply from the server
  if( recv(sock , buffer , sizeof(buffer) , 0) < 0){
    puts("recv failed");
  }
  reply = buffer;
  return reply;
}

int main(int argc , char *argv[]){
  udp_client client;
  std::string host, message;
  int portno;

  if(argc < 3){
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    _exit(0);
  }
  host = argv[1];
  if (atoi(argv[2]) != 0){
    portno = atoi(argv[2]);
  }
  else{
    fprintf(stderr,"Invalid port number: %s\n", argv[2]);
    _exit(0);
  }
  //while(1){
  std::cout << "Message to server: ";
  std::cin >> message;
  //send some data
  client.send_data(host, portno, message);
  //receive and echo reply
  std::cout<<client.receive(1024)<<std::endl;
  //}
  //done
  return 0;
}
