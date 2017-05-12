#pragma once
#include <boost/log/trivial.hpp>

void initLogger();

#define TRACE(msg) BOOST_LOG_TRIVIAL(trace) << msg
#define DEBUG(msg) BOOST_LOG_TRIVIAL(debug) << msg
#define INFO(msg) BOOST_LOG_TRIVIAL(info) << msg
#define WARNING(msg) BOOST_LOG_TRIVIAL(warning) << msg
#define ERROR(msg) BOOST_LOG_TRIVIAL(error) << msg
#define FATAL(msg) BOOST_LOG_TRIVIAL(fatal) << msg

#define BOOST_ERROR(ec) (ec).message()

