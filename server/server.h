#pragma once

#include <boost/asio.hpp>

class Server {
public:
    Server() = delete;
    Server(boost::asio::io_service& io_service, const uint16_t port);
private:
    void create(const uint16_t port);
    void doAccept();
private:
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
};
