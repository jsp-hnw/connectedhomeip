#!/usr/bin/env bash
# set -x
# redirect log
logfile=/tmp/entrypoint.log
exec >$logfile 2>&1

# Check if $1 is provided
if [ -n "$1" ]; then
    uart_dev="$1"
else
    uart_dev="/dev/ttyS0"
fi

# Output the uart_dev value for verification
echo "uart_dev is set to $uart_dev"

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

# Attempt to attach to the Bluetooth device
hciattach $uart_dev any 115200 noflow

# Check if hciattach was successful
if [ $? -eq 0 ]; then
    echo "hciattach successful."

    # Bring up hci0 interface
    while true; do
        hciconfig hci0 up
        if [ $? -eq 0 ]; then
            echo "##### hciconfig hci0 up successful."
            hciconfig
            break
        else
            #echo "hciconfig hci0 up failed. Wait for hci uart to be plugged in ..."
            sleep 3
        fi
    done

else
    echo "##### hciattach failed. maybe uart is not available!"
fi
tail -f /dev/null
