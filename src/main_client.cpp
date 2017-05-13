#include "server.h"
#include "utils/logger.h"


void onConnect(const boost::system::error_code& ec, ConnectionPtr connection) {
    if(ec) {
        return;
    }

    for (uint32_t i=0; i!=10; ++i) {
        std::string msg("hello");
        msg += ('a' + i);
        DEBUG(i);
        connection->write(std::make_shared<PackageBody>(msg));
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
