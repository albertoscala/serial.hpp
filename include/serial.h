#ifndef SERIAL_H
#define SERIAL_H

#include <stdlib.h>
#include <stdint.h>

#define BUF_SIZE 512

typedef enum 
{
    SERIAL_OK = 0,
    SERIAL_OPEN_FAIL,
    SERIAL_INVALID_TTY,
    SERIAL_READ_ERR,
    SERIAL_WRITE_ERR,
} serial_err_t;

serial_err_t serial_write(int fd, const void* buf, size_t n);
serial_err_t serial_read(int fd, void* buf, size_t n);

#endif /* SERIAL_H */