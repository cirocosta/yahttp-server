#ifndef HTTP_SERVER_HH
#define HTTP_SERVER_HH

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>

#include <iostream>
#include <string>

namespace http {
  #define BACKLOG 10
  #define BUFSIZE 1024

  class Server
  {
    struct sockaddr_in m_server_addr;
    struct sockaddr_in m_client_addr;

    unsigned m_port;
    int m_server_socket;
    int m_client_socket;

  public:
    Server(const unsigned port);
    ~Server();

    int start();
    void end();
  private:
    int _init();
  };
};

#endif
