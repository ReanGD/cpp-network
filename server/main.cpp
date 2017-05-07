#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "server.h"

void InitLog() {
    namespace expr = boost::log::expressions;
    auto fmt = expr::stream << "<" << boost::log::trivial::severity << "> " << expr::smessage;
    boost::log::add_console_log(std::clog, boost::log::keywords::format=fmt);
}


int main(int /*argc*/, char** /*argv[]*/) {
    try {
        InitLog();
        boost::asio::io_service io_service;
        CServer s(io_service, 5555);
        io_service.run();
    } catch (boost::system::system_error& e) {
        BOOST_LOG_TRIVIAL(fatal) << e.what();
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl;
    }

    return 0;
}
