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

class tcp_client{
public:
    tcp_client();
    bool conn(std::string, int);
    bool send_data(std::string data);
    std::string receive(int);
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
};

tcp_client::tcp_client(){
    sock = -1;
    port = 0;
    address = "";
}

/**
    Connect to a host on a certain port number
*/
bool tcp_client::conn(std::string address , int port){
    //create socket if it is not already created
    if(sock == -1){
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1){
            perror("Could not create socket");
        }
    }
    else    {   /* OK , nothing */  }

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

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            //strcpy(ip , inet_ntoa(*addr_list[i]) );
            server.sin_addr = *addr_list[i];
            break;
        }
    }

    //plain ip address
    else{
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }

    std::cout<<"Connected\n";
    return true;
}

/**
    Send data to the connected host
*/
bool tcp_client::send_data(std::string data){
    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0){
        perror("Send failed : ");
        return false;
    }
    return true;
}

/**
    Receive data from the connected host
*/
std::string tcp_client::receive(int size=512){
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
    tcp_client client;
    std::string host, message;
    int portno;

    if(argc < 3){
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      _exit(0);
    }
    host = argv[1];
    portno = atoi(argv[2]);

    //connect to host
    client.conn(host , portno);
    std::cout << "Message to server: ";
    std::cin >> message;
    //send some data
    client.send_data(message);

    //receive and echo reply
    std::cout<<client.receive(1024)<<std::endl;

    //done
    return 0;
}
