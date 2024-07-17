# This minimal build image is intentionally not based on chip-build
FROM ubuntu:jammy

ARG CHIPTOOL_PATH=amd64
ARG TFTP_SERVER=10.250.4.16
RUN set -x \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    ca-certificates pkg-config python3-venv net-tools iputils-ping iproute2 dbus bluez wget \
    && apt-get purge -y --auto-remove \
    && rm -rf /var/lib/apt/lists/* 

#COPY ${CHIPTOOL_PATH}/chip-tool /sbin/
RUN cd /sbin \
    && wget ${TFTP_SERVER}:/tftpboot/solution_part/chip-run/${CHIPTOOL_PATH}/chip-tool --ftp-user=image --ftp-password=image \
    && chmod a+x chip-tool

COPY entrypoint/entrypoint.sh /sbin/

# Set entrypoint
ENTRYPOINT ["/sbin/entrypoint.sh"]