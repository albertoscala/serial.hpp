#define _GNU_SOURCE

#include "../include/serial.h"

#include <termios.h>
#include <stdio.h>

static struct termios set_shared_value(int fd)
{
    struct termios tty;
    
    tcgetattr(fd, &tty);            // get serial attributes
    cfsetspeed(&tty, B115200);        // set serial speed

    tty.c_cflag &= ~PARENB;         // no parity
    tty.c_cflag &= ~CSTOPB;         // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |=  CS8;            // 8 data bits
    tty.c_cflag &= ~CRTSCTS;        // no hardware flow control
    tty.c_cflag |=  CLOCAL;         // ignore modem lines
    tty.c_oflag &= ~OPOST;          // raw output — important for write

    tcsetattr(fd, TCSANOW, &tty);   // set serial attributes

    return tty;
}

serial_err_t serial_write(int fd, const void* buf, size_t n)
{
    size_t bytes_written = 0;
    ssize_t bytes = 0;

    if (isatty(fd))
    {
        (void)set_shared_value(fd);

        uint8_t* buf_ptr = (uint8_t*)buf;
        while (bytes_written < n)
        {
            size_t remaining = n - bytes_written;
            bytes = write(fd, buf_ptr + bytes_written, remaining < BUF_SIZE ? remaining : BUF_SIZE);            
            if (bytes < 0) return SERIAL_WRITE_ERR;
            if (bytes == 0) break;
            bytes_written += bytes;
        }
    }
    else
    {
        fprintf(stderr, "[SERIAL] The port given is not a valid serial port...\n");
        return SERIAL_INVALID_TTY;  // signal failure to caller
    }

    return SERIAL_OK;
}

serial_err_t serial_read(int fd, void* buf, size_t n)
{
    struct termios tty;
    size_t bytes_read = 0;
    ssize_t bytes = 0;

    if (isatty(fd))
    {
        tty = set_shared_value(fd);

        // raw input - don't process incoming bytes
        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

        // blocking behavior
        tty.c_cc[VMIN]  = 0;                // don't wait for minimum bytes
        tty.c_cc[VTIME] = 100;               // 2 second timeout

        tcsetattr(fd, TCSANOW, &tty);       // set serial attributes

        uint8_t* buf_ptr = (uint8_t*)buf;
        while (bytes_read < n)
        {
            size_t remaining = n - bytes_read;
            bytes = read(fd, buf_ptr + bytes_read, remaining < BUF_SIZE ? remaining : BUF_SIZE);            
            if (bytes < 0) return SERIAL_READ_ERR;
            if (bytes == 0) break;
            bytes_read += bytes;
        }
    }
    else
    {
        fprintf(stderr, "[SERIAL] The port given is not a valid serial port...\n");
        return SERIAL_INVALID_TTY;
    }

    return SERIAL_OK;
}
