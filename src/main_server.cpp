#include "server.h"
#include "utils/logger.h"


int main(int /*argc*/, char** /*argv[]*/) {
    try {
        initLogger();
        boost::asio::io_service ioService;
        Server s(ioService, 5555);
        ioService.run();
    } catch (boost::system::system_error& e) {
        FATAL("top level boost error " << e.what());
    } catch (std::exception& e) {
        FATAL("top level error " << e.what());
    }

    return 0;
}
