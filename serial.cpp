#include "serial.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>

namespace serial
{
    ssize_t write(const char* port, const void* buf, size_t n)
    {
        termios tty{};
        ssize_t bytes;
        int fd = open(port, O_WRONLY);

        if (isatty(fd))
        {
            tcgetattr(fd, &tty);            // get serial attributes
            cfsetspeed(&tty, B9600);        // set serial speed

            tty.c_cflag &= ~PARENB;         // no parity
            tty.c_cflag &= ~CSTOPB;         // 1 stop bit
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |=  CS8;            // 8 data bits
            tty.c_cflag &= ~CRTSCTS;        // no hardware flow control
            tty.c_cflag |=  CLOCAL;         // ignore modem lines
            tty.c_oflag &= ~OPOST;          // raw output — important for write

            tcsetattr(fd, TCSANOW, &tty);   // set serial attributes
        
            bytes = ::write(fd, buf, n);
        }
        else
        {
            std::cerr << "[SERIAL] The port given is not a valid serial port..." << '\n';
        }

        close(fd);

        return bytes;
    }

    ssize_t read(const char* port, void* buf, size_t n)
    {
        termios tty{};
        ssize_t bytes = 0;
        int read;
        int fd = open(port, O_RDONLY | O_NOCTTY);

        if (isatty(fd))
        {
            tcgetattr(fd, &tty);
            cfsetspeed(&tty, B9600);            // must match the writer

            // framing - must match writer
            tty.c_cflag &= ~PARENB;             // no parity
            tty.c_cflag &= ~CSTOPB;             // 1 stop bit
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |=  CS8;                // 8 data bits
            tty.c_cflag &= ~CRTSCTS;            // no hardware flow control
            tty.c_cflag |=  CREAD | CLOCAL;     // CREAD is important - enables receiver

            // raw input - don't process incoming bytes
            tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
            tty.c_iflag &= ~(IXON | IXOFF | IXANY);
            tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

            // blocking behavior
            tty.c_cc[VMIN]  = 0;                // don't wait for minimum bytes
            tty.c_cc[VTIME] = 20;               // 2 second timeout

            tcsetattr(fd, TCSANOW, &tty);       // set serial attributes

            std::uint8_t* buf_ptr = reinterpret_cast<std::uint8_t*>(buf);
            while (bytes < n)
            {
                read = ::read(fd, buf_ptr + bytes, n - bytes);
                if (read < 0) break;
                if (read == 0) break;
                bytes += read;
            }
        }
        else
        {
            std::cerr << "[SERIAL] The port given is not a valid serial port..." << '\n';
        }

        close(fd);

        return bytes;
    }
}