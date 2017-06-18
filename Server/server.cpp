//
// A simple Internet server application.
// It listens to the port written in command line (default 1234),
// accepts a connection, and sends the "Hello!\r\n" message
// to a client. Then it receives the answer from a client and terminates.
//
// Usage:
//      server [port_to_listen]
// Default is the port 1234.
//
#include<iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<iostream>

using namespace std;
#define def_port 5060
static void usage();

int main(int argc, char *argv[])
  {
    if (argc > 1 && *(argv[1]) == '-') {
        usage(); exit(1);
    }

    int listenPort = def_port;
    if (argc > 1)
        listenPort = atoi(argv[1]);

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr <<("Cannot create a socket");
         exit(1);
    }

    // Fill in the address structure containing self address
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(listenPort);        // Port to listen
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind a socket to the address
    int res = bind(sock, (struct sockaddr*) &myaddr, sizeof(myaddr));
    std::cout << "res="<<res << '\n';
    if (res < 0)
    {
        cerr <<("Cannot bind a socket");
        exit(1);
    }

    // Set the "LINGER" timeout to zero, to close the listen socket
    // immediately at program termination.
    struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
    setsockopt(sock, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    // Now, listen for a connection
    res = listen(sock, 1);    // "1" is the maximal length of the queue
    if (res < 0)
     {
        cerr <<("Cannot listen");
         exit(1);
    }

  cout << "waiting for conections..." << '\n';
    // Accept a connection (the "accept" command waits for a connection with
    // no timeout limit...)
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len;
    int accepter = accept(sock, (struct sockaddr*) &peeraddr, &peeraddr_len);
    if (accepter < 0)
    {
          cerr <<("Cannot accept\n");
           exit(1);
    }

    // A connection is accepted. The new socket "s1" is created
    // for data input/output. The peeraddr structure is filled in with
    // the address of connected entity, print it.
    int first=((ntohl(peeraddr.sin_addr.s_addr)>>24)& 0xff);
    int sec=((ntohl(peeraddr.sin_addr.s_addr)>>16)& 0xff);
    int third=((ntohl(peeraddr.sin_addr.s_addr)>>8)& 0xff);
    int four=(ntohl(peeraddr.sin_addr.s_addr) & 0xff);
    cout << "conection from Ip "<<first<<"."<<sec<<"."<<third<<"."<<four << '\n';

    res = close(sock);    // Close the listen socket

    write(accepter, "Hello!\n", 8);

    char buffer[1024];
    res = read(accepter, buffer, 1023);
    if (res < 0) {
        perror("Read error"); exit(1);
    }
    buffer[res] = 0;
    printf("Received %d bytes:\n%s", res, buffer);

  close(accepter);          // Close the data socket
  //std::cout << "here" << '\n';
    return 0;
}

static void usage() {
    printf(
        "A simple Internet server application.\n"
        "It listens to the port written in command line (default 1234),\n"
        "accepts a connection, and sends the \"Hello!\" message to a client.\n"
        "Then it receives the answer from a client and terminates.\n\n"
        "Usage:\n"
        "     server [port_to_listen]\n"
        "Default is the port 1234.\n"
    );
}
