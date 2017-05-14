#pragma once

#include <boost/asio.hpp>
#include "package_parser.h"


class Connection
        : public std::enable_shared_from_this<Connection> {
public:
    Connection() = delete;
    Connection(boost::asio::ip::tcp::socket socket);
public:
    boost::asio::io_service& getIoService() { return m_socket.get_io_service(); }
    void read();
    void write(PackageBodyPtr package);
private:
    boost::asio::ip::tcp::socket m_socket;
    std::array<uint8_t, 256 * 1024> m_buffer;
    PackageParser m_parser;
};

using ConnectionPtr = std::shared_ptr<Connection>;
