#ifndef HANDLER_H
#define HANDLER_H

#include <boost/asio.hpp>

class CHandler
        : public std::enable_shared_from_this<CHandler>
{
    struct Header
    {
        uint64_t lenght;
    };

public:
    CHandler() = delete;
    CHandler(boost::asio::ip::tcp::socket socket);
public:
    void Start();
private:
    void DoReadHeader();
    void DoReadData();
private:
    boost::asio::ip::tcp::socket m_socket;
    Header m_header[1];
    std::vector<char> m_data;
};

#endif // HANDLER_H
