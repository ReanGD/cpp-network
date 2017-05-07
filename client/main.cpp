#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

struct Header {
    uint64_t lenght;
};

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;

    tcp::socket s(io_service);
    tcp::resolver resolver(io_service);
    boost::asio::connect(s, resolver.resolve({"192.168.1.2", "5555"}));

    for (uint32_t i=0; i!=10; ++i) {
        std::string msg("hello");
        msg += ('a' + i);
        std::cout << i << std::endl;
        Header header[1] = {Header{msg.size()}};
        boost::asio::write(s, boost::asio::buffer(header));
        boost::asio::write(s, boost::asio::buffer(msg));
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
