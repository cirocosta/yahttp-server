#include <iostream>
#include "Server.hh"

int main(int argc, char *argv[])
{
  http::Server server(8080);

  server.start();

  return EXIT_SUCCESS;
}
