# This minimal build image is intentionally not based on chip-build
FROM ubuntu:jammy

ARG CHIPTOOL_PATH=amd64
RUN set -x \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    ca-certificates pkg-config python3-venv net-tools iputils-ping iproute2 dbus bluez \
    && apt-get purge -y --auto-remove \
    && rm -rf /var/lib/apt/lists/* 

COPY ${CHIPTOOL_PATH}/chip-tool /sbin/
COPY entrypoint/entrypoint.sh /sbin/

# Set entrypoint
ENTRYPOINT ["/bin/bash", "-c", "/sbin/entrypoint.sh"]