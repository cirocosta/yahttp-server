#include <iostream>
#include "yahttp/Server.hh"

int main(int argc, char *argv[])
{
  yahttp::Server server(8080);

  server.start();

  return EXIT_SUCCESS;
}
