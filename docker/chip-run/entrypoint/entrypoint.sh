#!/usr/bin/env bash

# redirect log
logfile=/tmp/entrypoint__.log
exec >$logfile 2>&1

mkdir -p /run/dbus
rm -f /run/dbus/pid

pid=$(dbus-daemon --system --fork --print-pid)
# wait for dbus-daemon PID
while true; do
    if [ -f /run/dbus/pid ]; then
        break
    fi
    sleep 1
    echo "wait for D-Bus daemon to be ready"
done
echo "D-Bus daemon PID: $pid"

# Start bluetoothd in the background and redirect stderr to a log file
/usr/lib/bluetooth/bluetoothd -n 2>/tmp/btlog.txt &
# Wait for "Bluetooth management interface 1.14 initialized" in the log file
while ! grep -q "Bluetooth management interface .* initialized" /tmp/btlog.txt; do
    echo "wait for bluetoothd to be ready.."
    sleep 1
done
echo "bluetoothd is ready"

hciattach /dev/ttyS0 any 115200 noflow
hciconfig hci0 up
hciconfig

tail -f /dev/null
