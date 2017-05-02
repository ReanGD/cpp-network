#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

class CServer
{
public:
    CServer() = delete;
    CServer(boost::asio::io_service& io_service, const uint16_t port);
private:
    void DoAccept();
private:
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
};

#endif // SERVER_H
