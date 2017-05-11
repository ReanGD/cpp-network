#include "utils/logger.h"

#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>


void initLogger() {
    namespace expr = boost::log::expressions;
    auto fmt = expr::stream << "<" << boost::log::trivial::severity << "> " << expr::smessage;
    boost::log::add_console_log(std::clog, boost::log::keywords::format=fmt);
}
