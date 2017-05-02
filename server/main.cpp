#include <iostream>
#include "server.h"

int main(int /*argc*/, char** /*argv[]*/)
{
  try
  {
    boost::asio::io_service io_service;
    CServer s(io_service, 5555);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
