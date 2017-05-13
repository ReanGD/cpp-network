#include "connection.h"
#include "utils/logger.h"

using boost::asio::ip::tcp;

static const std::string CLASS("Connection: ");

Connection::Connection(tcp::socket socket)
    : m_socket(std::move(socket)) {
}

void Connection::read() {
    auto self(shared_from_this());

    m_socket.async_read_some(boost::asio::buffer(m_buffer),
                             [this, self](boost::system::error_code ec, std::size_t length) {
        if (ec) {
            ERROR(CLASS << "Error read = " << BOOST_ERROR(ec));
        } else {
            std::shared_ptr<PackageBody> package;
            DEBUG(CLASS << "read " << length << " bytes");
            while (package = m_parser.parse(m_buffer.begin(), length)) {
                std::string msg(package->m_data.begin(), package->m_data.end());
                INFO(CLASS << "Message: \"" << msg << "\"");
            }
            read();
        }
    });
}

void Connection::write(PackageBodyPtr package)
{
    auto self(shared_from_this());

    auto data = m_parser.serialize(package);
    m_socket.async_write_some(boost::asio::buffer(data),
                              [this, self](boost::system::error_code ec, std::size_t length) {
      if(ec) {
          ERROR(CLASS << "Error write = " << BOOST_ERROR(ec));
      } else {
          DEBUG(CLASS << "write " << length << " bytes");
      }
    });
}
