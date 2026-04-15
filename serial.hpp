#pragma once

#include <cstdlib>
#include <cstdint>

namespace serial
{
    ssize_t write(const char* port, const void* buf, size_t n);
    ssize_t read(const char* port, void* buf, size_t n);
}