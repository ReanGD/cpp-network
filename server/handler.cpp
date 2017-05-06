#include "handler.h"
#include <boost/log/trivial.hpp>


using boost::asio::ip::tcp;

CHandler::CHandler(tcp::socket socket)
    : m_socket(std::move(socket))
{
}

void CHandler::Start()
{
    DoRead();
}

void CHandler::DoReadHeader()
{
    auto self(shared_from_this());

    m_socket.async_read_some(boost::asio::buffer(m_package.m_header),
                             [this, self](boost::system::error_code ec, std::size_t length)
    {
        if (ec)
        {
            BOOST_LOG_TRIVIAL(error) << "DoReadHeader: Error read header, error message \"" << ec << "\"";
        } else
        {
            if (length != sizeof(m_package.m_header))
            {
                BOOST_LOG_TRIVIAL(error) << "DoReadHeader: Error read header, error size";
            } else
            {
                DoReadData();
            }
        }
    });
}

void CHandler::DoReadData()
{
    auto self(shared_from_this());

    auto length = m_package.m_header[0].lenght;
    auto completion = [length](const boost::system::error_code &ec, size_t bytes) -> size_t
    {
        if(ec)
            return 0;

        if (bytes == length)
            return 0;

        if (bytes > length)
        {
            BOOST_LOG_TRIVIAL(error) << "DoReadData: Error read data, error size in completion";
            return 0;
        }

        return 1;
    };

    m_package.m_data.resize(m_package.m_header[0].lenght);
    boost::asio::async_read(m_socket, boost::asio::buffer(m_package.m_data), completion,
                            [this, self](boost::system::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            if(length != m_package.m_data.size())
                BOOST_LOG_TRIVIAL(error) << "DoReadData: Error read data, error size in async_read";
            else
            {
                DoProcess();
            }
        } else
        {
            BOOST_LOG_TRIVIAL(error) << "DoReadData: Error read data, error message \"" << ec << "\"";
        }
    });
}

void CHandler::DoProcess()
{
    std::string msg(m_package.m_data.begin(), m_package.m_data.end());
    BOOST_LOG_TRIVIAL(info) << "Message: \"" << msg << "\"";
    DoRead();
}

void CHandler::DoRead()
{
    DoReadHeader();
}
