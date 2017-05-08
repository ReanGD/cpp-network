#include "handler.h"
#include <boost/log/trivial.hpp>


using boost::asio::ip::tcp;

CHandler::CHandler(tcp::socket socket)
    : m_socket(std::move(socket)) {
}

void CHandler::Start() {
    DoRead();
}

void CHandler::DoRead() {
    auto self(shared_from_this());

    m_socket.async_read_some(boost::asio::buffer(m_buffer),
                             [this, self](boost::system::error_code ec, std::size_t length) {
        if (ec) {
            BOOST_LOG_TRIVIAL(error) << "DoRead: Error read \"" << ec << "\"";
        } else {
            std::shared_ptr<PackageBody> package;
            BOOST_LOG_TRIVIAL(debug) << "DoRead: read " << length << " bytes";
            while (package = m_parser.parse(m_buffer.begin(), length)) {
                std::string msg(package->m_data.begin(), package->m_data.end());
                BOOST_LOG_TRIVIAL(info) << "Message: \"" << msg << "\"";
            }
            DoRead();
        }
    });
}
