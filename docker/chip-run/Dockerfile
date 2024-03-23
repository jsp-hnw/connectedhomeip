# This minimal build image is intentionally not based on chip-build
FROM ubuntu:jammy
RUN set -x \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    ca-certificates pkg-config python3-venv net-tools iputils-ping iproute2 dbus bluez \
    && apt-get purge -y --auto-remove \
    && rm -rf /var/lib/apt/lists/* 

# COPY rootnode_dimmablelight_bCwGYSDpoe /sbin/
# COPY chip-tool /sbin/