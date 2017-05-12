#include "server.h"

#include <boost/thread/thread.hpp>

#include "connection.h"
#include "utils/logger.h"

using boost::asio::ip::tcp;

static const uint8_t BIND_RETRY_COUNT = 5;
static const boost::posix_time::milliseconds BIND_RETRY_SLEEP_BASE(1000);
static const std::string CLASS("Server: ");

Server::Server(boost::asio::io_service &io_service)
    : m_acceptor(io_service)
    , m_socket(io_service) {
}

std::shared_ptr<Server> Server::create(boost::asio::io_service &io_service) {
    return std::shared_ptr<Server>(new Server(io_service));
}

void Server::start(const uint16_t port) {
    using namespace boost::asio;
    INFO(CLASS << "Starting server on address 0.0.0.0:" << port);

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

    doAccept();
}

void Server::connect(const std::string& host, const uint16_t port, const OnConnect& onConnect) {
    using namespace boost::asio;
    INFO(CLASS << "Start connecting to address " << host << ":" << port);

    auto self(shared_from_this());
    tcp::resolver resolver(m_socket.get_io_service());
    tcp::resolver::iterator it = resolver.resolve({host, std::to_string(port)});
    async_connect(m_socket, it,
                  [this, self, onConnect](const boost::system::error_code& ec, tcp::resolver::iterator) {
        if (ec) {
            // TODO
            ERROR(CLASS << "Error connect = " << BOOST_ERROR(ec));
        } else {
            onConnect(std::make_shared<Connection>(std::move(m_socket)));
        }
    });
}

void Server::doAccept() {
    DEBUG(CLASS << "Wait accept");

    auto self(shared_from_this());
    m_acceptor.async_accept(m_socket,
                            [this, self](boost::system::error_code ec) {
        if (ec) {
            ERROR(CLASS << "Error accept = " << BOOST_ERROR(ec));
        } else {
            auto remote = m_socket.remote_endpoint();
            INFO(CLASS << "Accepted client from " << remote.address().to_string() << ":" << remote.port());
            std::make_shared<Connection>(std::move(m_socket))->doRead();
        }

        doAccept();
    });
}
