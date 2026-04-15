#include <serial/serial.hpp>

#include <cstdlib>
#include <cstdint>
#include <string>
#include <string.h>
#include <iostream>

constexpr const size_t MAX_SIZE = 512;

struct Msg
{
    size_t len;
    std::uint8_t data[MAX_SIZE];
};

int main()
{
    Msg src{}, dst{};

    std::string data = "Hi from Serial!";

    src.len = data.size();
    ::memcpy(&src.data, data.data(), data.size());

    serial::write("/dev/ttyV0", &src, sizeof(Msg));

    serial::read("/dev/ttyV1", &dst, sizeof(Msg));

    std::cout << "Message: " << dst.data << " | Len: " << dst.len << std::endl;

    return 0;
}