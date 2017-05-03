#include "server.h"

#include <iostream>
#include <boost/thread/thread.hpp>

#include "handler.h"

using boost::asio::ip::tcp;

static const uint8_t BIND_RETRY_COUNT = 5;
static const boost::posix_time::milliseconds BIND_RETRY_SLEEP_BASE(1000);

CServer::CServer(boost::asio::io_service &io_service, const uint16_t port)
    : m_acceptor(io_service),
      m_socket(io_service)
{
    Create(port);
    DoAccept();
}

void CServer::Create(const uint16_t port)
{
    using namespace boost::asio;

    boost::system::error_code ec;
    tcp::endpoint endpoint(tcp::v4(), port);

    m_acceptor.open(endpoint.protocol(), ec);
    detail::throw_error(ec, "open");

    m_acceptor.set_option(socket_base::reuse_address(true), ec);
    detail::throw_error(ec, "set_option");

    m_acceptor.bind(endpoint, ec);
    for(uint8_t i=1; (ec == error::basic_errors::address_in_use) && i!=(BIND_RETRY_COUNT + 1); ++i)
    {
        std::cerr << "Port " << port << " already in use, sleep " << (BIND_RETRY_SLEEP_BASE * i).total_milliseconds() << " ms" << std::endl;
        boost::this_thread::sleep(BIND_RETRY_SLEEP_BASE * i);
        m_acceptor.bind(endpoint, ec);
    }
    detail::throw_error(ec, "bind");

    m_acceptor.listen(socket_base::max_connections, ec);
    detail::throw_error(ec, "listen");
}

void CServer::DoAccept()
{
    m_acceptor.async_accept(m_socket,
                            [this](boost::system::error_code ec)
    {
        if (!ec)
        {
            std::make_shared<CHandler>(std::move(m_socket))->Start();
        }

        DoAccept();
    });
}
