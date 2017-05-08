#ifndef HANDLER_H
#define HANDLER_H

#include <boost/asio.hpp>
#include "package_parser.h"


class CHandler
        : public std::enable_shared_from_this<CHandler> {
public:
    CHandler() = delete;
    CHandler(boost::asio::ip::tcp::socket socket);
public:
    void Start();
private:
    void DoRead();
private:
    boost::asio::ip::tcp::socket m_socket;
    std::array<uint8_t, 256 * 1024> m_buffer;
    PackageParser m_parser;
};

#endif // HANDLER_H
