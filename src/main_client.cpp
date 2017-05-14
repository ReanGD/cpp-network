#include "server.h"
#include "utils/logger.h"

using TimerPtr = std::shared_ptr<boost::asio::deadline_timer>;


void write(const boost::system::error_code& ec, ConnectionPtr connection, TimerPtr timer, const uint32_t i) {
    if(ec) {
        ERROR("Error deadline_timer = " << BOOST_ERROR(ec));
        return;
    }

    std::string msg("hello");
    msg += ('a' + i);
    DEBUG(i);
    connection->write(std::make_shared<PackageBody>(msg));

    if (i == 9) {
        return;
    }

    timer->expires_from_now(boost::posix_time::seconds(2));
    timer->async_wait([connection, timer, i](const boost::system::error_code& ec){
        write(ec, connection, timer, i + 1);
    });
}

void onConnect(const boost::system::error_code& ec, ConnectionPtr connection) {
    if(ec) {
        return;
    }

    auto timer = std::make_shared<boost::asio::deadline_timer>(connection->getIoService());
    write(ec, connection, timer, 0);
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
