#include "server.h"
#include "handler.h"

using boost::asio::ip::tcp;

CServer::CServer(boost::asio::io_service &io_service, const uint16_t port)
    : m_acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
      m_socket(io_service)
{
    DoAccept();
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
