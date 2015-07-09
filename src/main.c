#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// NOTICE: all ports under 1024 are RESERVED
// we can go with any up to 65535 (if not used)
#define PORT 8080
#define BACKLOG 10

#define CHKERR(var, msg) \
  do { \
    if (!~var) { \
      fputs(msg, stderr); \
      fputs(strerror(errno), stderr); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

int inet_aton(const char *cp, struct in_addr *inp);


/**
   * inet_aton: converts the Internet host address cp 
   *            from the IPv4 num‐ bers-and-dots notation into binary 
   *            form (in network byte order) and stores it in the 
   *            structure that inp points to.
   *
   *
   * socket(): returns a socket descriptor that we can use in later
   *           syscalls of -1 on error. The global `errno` is set to
   *           the error value.
   *
   *           socket(int domain, int type, int protocol)
   *
   *
   * perror: produces a message on the stderr describing the last
   *         error encountered during a call to a sys or lib fun.
   *
   *         perror(const char *s);
   *         
   *
   * bind: having the socket we must associate the socket with a port
   *       on the local machine. The port number is used by the kernel
   *       to match an incoming packet to a certein process' 
   *       descriptor. 
   *
   *       bind(int sockfd, struct sockaddr *my_addr, int addrlen);
   *
   *       note that addrlen can  be directly set to
   *       sizeof(struct sockaddr)
   *
   * connect: initiates a connection on a socket. Doesn't require a 
   *          bind before connecting.
   *
   *          int connect(int sockfd, struct sockaddr *serv_addr,
   *                      int addrlen);
   *
   * listen: waits for incoming connections and handles them in
   *         some way. First listen() and then accept(). Requires a
   *         bind() before. This will mark the socket as a passive
   *         socket (a socket that will accept incomming connection
   *         requests).
   *
   *         int listen(int sockfd, int backlog);
   *
   *         backlog describes the number of connections allowed on
   *         the incoming queue (an acceptance queue).
   *
   *
   * accept: someone is trying to `connect()` to the machine on a port
   *         that we've passed to it and we're listening on. The 
   *         connection will be queue until we accept it. When we
   *         accept it then we'll tell it to get the pending connection
   *         and it will return us a brand new socket file descriptor
   *         to use for this single connection. With it we'll then be
   *         able to `send()` and `recv()`.
   *
   *         int accept(int sockfd, void *addr, int *addrlen)
   *
   *
   * send: sends a message on a socket. May be used only when the 
   *       socket is in a connected state. Notice that the message
   *       might be too long to pass atomically through the underlying
   *       protocol (resulting in EMSGSIZE error.
   *
   *       int send(int sockfd, const void *msg, int len, int flags);
   *
   *       returns the number of characters sent (or -1 on error)
   *
   *
   * recv: used to receive messages from a socket, whether or not it 
   *       is connection-oriented.
   *
   *       int recv(int sockfd, void *buf, size_t len, int flags);
   *       
   *       returns the number of bytes received (-1 on error or 0 when
   *       the remote side has closed the connection).
   *
   */

int main ()
{
  struct sockaddr_in my_addr;
  struct sockaddr_in their_addr;
  int sockfd, new_fd;
  int localerr;
  unsigned int sin_size;

  int bufsize = 1024;
  char* buffer = malloc(bufsize*sizeof(*buffer));

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  CHKERR(sockfd, "socket");

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);  // chooses at random
  inet_aton("10.12.110.57", &(my_addr.sin_addr));
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  memset(&(my_addr.sin_zero), '\0', 8);

  localerr = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
  CHKERR(localerr, "bind");

  /* localerr = connect(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)); */
  CHKERR(localerr, "connect");

  printf("Serving at: %s:%d\n", 
         inet_ntoa(my_addr.sin_addr),
         ntohs(my_addr.sin_port));

  while (1) {
    localerr = listen(sockfd, BACKLOG);
    CHKERR(localerr, "listen");

    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
    CHKERR(new_fd, "accept");

    recv(new_fd, buffer, bufsize, 0);
    printf("%s\n", buffer);
    send(new_fd, "hello world\n", 12, 0);
    send(new_fd, "HTTP/1.1 200 OK\n", 16, 0);
    send(new_fd, "Content-length: 46\n", 19, 0);
    send(new_fd, "Content-Type: text/html\n\n", 25, 0);
    send(new_fd, "<html><body><H1>Hello world</H1></body></html>",46, 0);
    close(new_fd);
  }

  close(sockfd);

  printf("terminating.\n");
  exit(EXIT_SUCCESS);
}
