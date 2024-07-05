#!/usr/bin/env bash
mkdir -p /run/dbus
dbus-daemon --system --fork
/usr/lib/bluetooth/bluetoothd &
sleep 5
hciattach /dev/ttyS0 any 115200 noflow
hciconfig hci0 up

tail -f /dev/null
