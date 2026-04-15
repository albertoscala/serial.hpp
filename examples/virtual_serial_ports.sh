#!/bin/bash

SOCAT_PID_FILE=/tmp/socat_virtual_serial.pid

start() {
    if [ -f "$SOCAT_PID_FILE" ]; then
        echo "Virtual serials already running (PID $(cat $SOCAT_PID_FILE))"
        exit 1
    fi

    socat -d -d PTY,link=/dev/ttyV0,rawer,mode=666 PTY,link=/dev/ttyV1,rawer,mode=666 &
    echo $! > "$SOCAT_PID_FILE"
    echo "Virtual serials started (PID $!)"
}

stop() {
    if [ ! -f "$SOCAT_PID_FILE" ]; then
        echo "Virtual serials not running"
        exit 1
    fi

    kill $(cat "$SOCAT_PID_FILE")
    rm "$SOCAT_PID_FILE"
    echo "Virtual serials stopped"
}

status() {
    if [ -f "$SOCAT_PID_FILE" ] && kill -0 $(cat "$SOCAT_PID_FILE") 2>/dev/null; then
        echo "Running (PID $(cat $SOCAT_PID_FILE))"
    else
        echo "Not running"
    fi
}

case "$1" in
    start)  start ;;
    stop)   stop ;;
    restart) stop && start ;;
    status) status ;;
    *)      echo "Usage: $0 {start|stop|restart|status}" ;;
esac