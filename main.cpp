#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "version.h"

using namespace boost::asio;

class Message {
public:
    Message() = default;

    std::string to_string() const {
        std::stringstream ss;
        ss << v1 << "," << v2 << "," << v0 << "," <<
            v3 << "," << v4 << "," << v5 << "," <<
            v6 << "," << v7 << "," <<
            v8 << "," << v9;
        return ss.str();
    }

    int64_t v0 = 0;
    int32_t v1 = 0;
    int32_t v2 = 0;
    int32_t v3 = 0;
    int32_t v4 = 0;
    int32_t v5 = 0;
    int32_t v6 = 0;
    int32_t v7 = 0;
    int32_t v8 = 0;
    int32_t v9 = 0;
};


struct UdpServer {
    explicit UdpServer(ip::udp::socket socket)
        : socket_(std::move(socket)) {
        read();
    }
private:
    void read() {
        socket_.async_receive_from(buffer(data_, 1500),
            remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t length) {

                if (ec) return;

                data_[length] = '\0';

                int32_t size = 0;
                Message a;

                std::string magic = std::string(data_, 4);
                memcpy(&size, data_+4, 4);
                memcpy(&a.v0, data_ + 8, 8);
                memcpy(&a.v1, data_ + 16, 4);
                memcpy(&a.v2, data_ + 20, 4);
                memcpy(&a.v3, data_ + 24, 4);
                memcpy(&a.v4, data_ + 28, 4);
                memcpy(&a.v5, data_ + 32, 4);
                memcpy(&a.v6, data_ + 36, 4);
                memcpy(&a.v7, data_ + 40, 4);
                memcpy(&a.v8, data_ + 44, 4);
                memcpy(&a.v9, data_ + 48, 4);

                std::cout << "length: " << length << "\n";
                std::cout << "magic: "  << magic  << "\n";
                std::cout << "size: " << size << "\n";

                std::cout << "message: " << a.to_string() << "\n";

                if (strcmp(data_, "\n") == 0) return;
            }
        );
    }

    ip::udp::socket socket_;
    ip::udp::endpoint remote_endpoint_;
    char data_[1500 + 1]; // +1 for we can always null terminate safely
};


int main(int argc, char** argv) {

    std::string commandLineStr;
    for (int i = 0; i < argc; i++) commandLineStr += std::string(argv[i]) + " ";
    std::cout << commandLineStr << "\n";
    std::cout << "Version: " << PROJECT_VERSION << " | " << BUILD_DATE << "\n";

    try {
        io_context io_context;

        ip::udp::endpoint ep(ip::udp::v4(), 5002);
        ip::udp::socket sock(io_context, ep);
        UdpServer server(std::move(sock));
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}