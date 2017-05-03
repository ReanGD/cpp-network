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
    catch ( boost::system::system_error& e)
    {
        std::cerr << "Boost exception: " << e.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
