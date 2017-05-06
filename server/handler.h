#ifndef HANDLER_H
#define HANDLER_H

#include <boost/asio.hpp>

struct Package
{
    struct Header
    {
        uint64_t lenght;
    } m_header[1];
    std::vector<char> m_data;
};

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
    void DoProcess();
    void DoRead();
private:
    boost::asio::ip::tcp::socket m_socket;
    Package m_package;
};

#endif // HANDLER_H
