#include "server.h"

#include <boost/thread/thread.hpp>

#include "connection.h"
#include "utils/logger.h"

using boost::asio::ip::tcp;

static const uint8_t BIND_RETRY_COUNT = 5;
static const boost::posix_time::milliseconds BIND_RETRY_SLEEP_BASE(1000);
static const std::string CLASS("Server: ");

Server::Server(boost::asio::io_service &io_service, const uint16_t port)
    : m_acceptor(io_service)
    , m_socket(io_service) {

    INFO(CLASS << "Starting server on address 0.0.0.0:" << port);

    create(port);
    doAccept();
}

void Server::create(const uint16_t port) {
    using namespace boost::asio;

    boost::system::error_code ec;
    tcp::endpoint endpoint(tcp::v4(), port);

    m_acceptor.open(endpoint.protocol(), ec);
    detail::throw_error(ec, "open");

    m_acceptor.set_option(socket_base::reuse_address(true), ec);
    detail::throw_error(ec, "set_option");

    m_acceptor.bind(endpoint, ec);
    for(uint8_t i=1; (ec == error::basic_errors::address_in_use) && i!=(BIND_RETRY_COUNT + 1); ++i) {
        BOOST_LOG_TRIVIAL(warning) << "Address 0.0.0.0:" << port << " already in use, sleep " << (BIND_RETRY_SLEEP_BASE * i).total_milliseconds() << " ms" << std::endl;
        boost::this_thread::sleep(BIND_RETRY_SLEEP_BASE * i);
        m_acceptor.bind(endpoint, ec);
    }
    detail::throw_error(ec, "bind");

    m_acceptor.listen(socket_base::max_connections, ec);
    detail::throw_error(ec, "listen");
}

void Server::doAccept() {
    DEBUG(CLASS << "Wait accept");
    m_acceptor.async_accept(m_socket,
                            [this](boost::system::error_code ec) {
        if (ec) {
            ERROR(CLASS << "Error accept = " << BOOST_ERROR(ec));
        } else {
            auto remote = m_socket.remote_endpoint();
            INFO(CLASS << "Accepted client from " << remote.address().to_string() << ":" << remote.port());
            std::make_shared<Connection>(std::move(m_socket))->start();
        }

        doAccept();
    });
}
