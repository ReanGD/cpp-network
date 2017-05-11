#include "connection.h"
#include "utils/logger.h"


using boost::asio::ip::tcp;

Connection::Connection(tcp::socket socket)
    : m_socket(std::move(socket)) {
}

void Connection::start() {
    doRead();
}

void Connection::doRead() {
    auto self(shared_from_this());

    m_socket.async_read_some(boost::asio::buffer(m_buffer),
                             [this, self](boost::system::error_code ec, std::size_t length) {
        if (ec) {
            ERROR("DoRead: Error read \"" << ec << "\"");
        } else {
            std::shared_ptr<PackageBody> package;
            DEBUG("DoRead: read " << length << " bytes");
            while (package = m_parser.parse(m_buffer.begin(), length)) {
                std::string msg(package->m_data.begin(), package->m_data.end());
                INFO("Message: \"" << msg << "\"");
            }
            doRead();
        }
    });
}
