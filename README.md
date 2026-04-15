# serial.hpp

> A lightweight C++ library for serial port communication on Linux.

Started as an internal utility to simplify serial port communication in day-to-day tooling at work. Used it once, liked the concept, and decided it was worth building out properly: a structured build system, and room to grow into something more reusable and robust.

It wraps the Linux termios API to provide simple `read`/`write` functions over serial ports, without requiring the caller to manually configure baud rate, parity, stop bits, or flow control every time.

---

## Features

- Simple one-call write and read interface
- Automatic port configuration (baud rate, parity, stop bits, flow control)
- Raw I/O mode - no byte processing or line-buffering by the OS
- 2-second read timeout with incremental buffered reads
- Error reporting via stderr for invalid or non-serial ports

---

## Requirements

- Linux (uses POSIX termios API)
- C++17 or later
- CMake 3.23+

---

## Installation

### Clone and build

```bash
git clone https://github.com/yourname/serial.git
cd serial
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

### Using as a subdirectory in your project

```cmake
add_subdirectory(serial)
target_link_libraries(your_target PRIVATE serial)
```

---

## Usage

### Writing to a port

```cpp
#include <serial/serial.hpp>

int main() {
    const char* msg = "hello";
    serial::write("/dev/ttyUSB0", msg, 5);
}
```

### Reading from a port

```cpp
#include <serial/serial.hpp>

int main() {
    char buf[64] = {};
    ssize_t n = serial::read("/dev/ttyUSB0", buf, sizeof(buf));
    // buf now contains up to n bytes
}
```

> **Note:** both functions open and close the port on every call. This is intentional for simplicity in the current version. A persistent connection API is planned for a future release.

---

## Default Serial Configuration

Both functions apply the following settings automatically:

| Parameter      | Value                          |
|----------------|--------------------------------|
| Baud rate      | 9600                           |
| Data bits      | 8                              |
| Stop bits      | 1                              |
| Parity         | None                           |
| Flow control   | None (no RTS/CTS)              |
| Read timeout   | 2 seconds (VTIME = 20)         |
| Read mode      | Non-blocking minimum (VMIN = 0)|

---

## Testing with Virtual Serial Ports

To test without physical hardware, use `socat` to create a linked virtual serial pair:

```bash
sudo socat -d -d \
  PTY,link=/dev/ttyV0,rawer,mode=666 \
  PTY,link=/dev/ttyV1,rawer,mode=666 &
```

Then write to one end and read from the other:

```cpp
serial::write("/dev/ttyV0", "ping", 4);

char buf[16] = {};
serial::read("/dev/ttyV1", buf, sizeof(buf));
```

---

## Roadmap

The library is functional but still early. The following improvements are planned:

- **`SerialConfig` struct** — define baud rate, parity, stop bits, and timeout once and pass it to both `read` and `write`, instead of hardcoding defaults inside each function
- **Error codes** — return typed error codes instead of relying solely on stderr output
- **Non-blocking mode** — optional async read support
- **Configurable timeout** — expose `VMIN` and `VTIME` through the config struct

---

## License

MIT License. See [LICENSE](LICENSE) for details.
