#include "server.h"
#include "utils/logger.h"


void onConnect(ConnectionPtr connection) {
    for (uint32_t i=0; i!=10; ++i) {
        std::string msg("hello");
        msg += ('a' + i);
        DEBUG(i);
        PackageHeader header[1] = {PackageHeader{msg.size()}};
        boost::asio::write(connection->m_socket, boost::asio::buffer(header));
        boost::asio::write(connection->m_socket, boost::asio::buffer(msg));
    }
}

int main(int /*argc*/, char** /*argv[]*/) {
    try {
        initLogger();
        boost::asio::io_service ioService;
        Server::create(ioService)->connect("192.168.1.2", 5555, onConnect);
        ioService.run();
    } catch (boost::system::system_error& e) {
        FATAL("top level boost error " << e.what());
    } catch (std::exception& e) {
        FATAL("top level error " << e.what());
    }

    return 0;
}
