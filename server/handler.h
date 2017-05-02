#ifndef HANDLER_H
#define HANDLER_H

#include <boost/asio.hpp>

class CHandler
        : public std::enable_shared_from_this<CHandler>
{
public:
    CHandler() = delete;
    CHandler(boost::asio::ip::tcp::socket socket);
public:
    void Start();
private:
    void DoRead();
    void DoWrite(const std::size_t length);
private:
    boost::asio::ip::tcp::socket m_socket;
    enum { max_length = 1024 };
    char m_data[max_length];
};

#endif // HANDLER_H
