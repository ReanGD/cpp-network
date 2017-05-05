#include "handler.h"
#include <boost/log/trivial.hpp>


using boost::asio::ip::tcp;

CHandler::CHandler(tcp::socket socket)
    : m_socket(std::move(socket))
{
}

void CHandler::Start()
{
    DoReadHeader();
}

void CHandler::DoReadHeader()
{
    auto self(shared_from_this());

    m_socket.async_read_some(boost::asio::buffer(m_header),
                             [this, self](boost::system::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            DoReadData();
        } else
        {
            BOOST_LOG_TRIVIAL(info) << "Error read header, error message \"" << ec << "\"";
        }
    });
}

void CHandler::DoReadData()
{
    auto self(shared_from_this());

    auto length = m_header[0].lenght;
    auto completion = [length](const boost::system::error_code &ec, size_t bytes) -> size_t
    {
        if(ec)
            return 0;

        if (bytes == length)
            return 0;

        if (bytes > length)
        {
            BOOST_LOG_TRIVIAL(info) << "Error read data, error size";
            return 0;
        }

        return 1;
    };

    m_data.resize(m_header[0].lenght);
    boost::asio::async_read(m_socket, boost::asio::buffer(m_data), completion,
                            [this, self](boost::system::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            if(length != m_data.size())
                BOOST_LOG_TRIVIAL(info) << "Error read data, error size";
            else
            {
                std::string homepath(getenv("HOME"));
                FILE *f = fopen((homepath + "/tmp/data.dat").c_str(), "ab");
                fwrite(&m_data[0], m_data.size(), sizeof(m_data[0]), f);
                fclose(f);
                DoReadHeader();
            }
        } else
        {
            BOOST_LOG_TRIVIAL(info) << "Error read data, error message \"" << ec << "\"";
        }
    });
}
