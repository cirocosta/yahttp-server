#include "yahttp/Server.hh"

void sigchld_handler (int s)
{
  while (wait(NULL) > 0);
}

namespace yahttp {

  Server::Server(unsigned port)
    : m_port(port)
  {
    _init();
  }

  Server::~Server()
  { }

  int Server::_init()
  {
    int err = 0;
    const int yes = 1;
    struct sigaction sa;

    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!~m_server_socket) {
      std::cerr << "Socket: " << strerror(errno) << std::endl;
      return m_server_socket;
    }

    err = setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR,
                     &yes, sizeof(yes));
    if (!~err) {
      std::cerr << "setsockopt: " << strerror(errno) << std::endl;
      return err;
    }

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(m_port);
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(m_server_addr).sin_zero, '\0', 8);

    err = bind(m_server_socket, (struct sockaddr*)&m_server_addr,
               sizeof(struct sockaddr));
    if (!~err) {
      std::cerr << "bind: " << strerror(errno) << std::endl;
      return err;
    }

    sa.sa_handler = &sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (!~(err = sigaction(SIGCHLD, &sa, NULL))) {
      std::cerr << "sigaction: " << strerror(errno) << std::endl;
      return err;
    }

    return err;
  }

  int Server::start()
  {
    int err = 0;
    int pid;
    unsigned sin_size;
    char buffer[BUFSIZE];


    err = listen(m_server_socket, BACKLOG);
    if (!~err) {
      std::cerr << "listen: " << strerror(errno) << std::endl;
      return err;
    }

    sin_size = sizeof(struct sockaddr_in);

    while (1) {
      m_client_socket =
        accept(m_server_socket, (struct sockaddr*)&m_client_addr, &sin_size);
      if (!~m_client_socket) {
        std::cerr << "accept: " << strerror(errno) << std::endl;
        continue;
      }

      if (!(pid = fork())) {  // child
        close(m_server_socket);

        err = getnameinfo(
            (struct sockaddr*)&m_client_addr,
            sizeof(m_client_addr.sin_addr.s_addr),
            buffer,
            BUFSIZE, NULL, 0, NI_NUMERICHOST
        );

        err = recv(m_client_socket, buffer, BUFSIZE, 0);
        printf("%s\n", buffer);
        err = send(m_client_socket, "Hello, World!\n", 14, 0);
        err = close(m_client_socket);

        exit(EXIT_SUCCESS);
      } else if (pid > 0) {   // parent
        close(m_client_socket);
      } else {                // error
        std::cerr << "fork(): " << strerror(errno) << std::endl;
      }
    }

    return err;
  }

  void Server::end()
  {
    if (!~close(m_server_socket))
      std::cerr << "close: " << strerror(errno) << std::endl;
  }
};
