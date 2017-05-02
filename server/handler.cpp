#include "handler.h"


using boost::asio::ip::tcp;

CHandler::CHandler(tcp::socket socket)
    : m_socket(std::move(socket))
{
}

void CHandler::Start()
{
    DoRead();
}

void CHandler::DoRead()
{
    auto self(shared_from_this());
    m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            DoWrite(length);
          }
        });
}

void CHandler::DoWrite(const std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                DoRead();
            }
        });
}
