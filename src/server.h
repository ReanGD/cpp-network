#pragma once

#include "connection.h"


class Server
        : public std::enable_shared_from_this<Server> {
public:
    using OnConnect = std::function<void (const boost::system::error_code&, ConnectionPtr)>;
private:
    Server() = delete;
    Server(boost::asio::io_service& io_service);
public:
    static std::shared_ptr<Server> create(boost::asio::io_service& io_service);

    void start(const uint16_t port);
    void connect(const std::string& host, const uint16_t port, const OnConnect& onConnect);
private:    
    void doAccept();
private:
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
};
