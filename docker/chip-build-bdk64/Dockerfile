ARG VERSION=1
#FROM ghcr.io/project-chip/chip-build:${VERSION}
#LABEL org.opencontainers.image.source https://github.com/project-chip/connectedhomeip
FROM ubuntu:jammy

# ------------------------------------------------------------------------------
# Add group/user for tizen
ARG USER_NAME=$USER
ARG USER_UID=1000
ARG USER_GID=$USER_UID
ENV USER_HOME /home/$USER_NAME

RUN set -x \
    && groupadd -g $USER_GID $USER_NAME \
    && useradd -m $USER_NAME -s /bin/bash -u $USER_UID -g $USER_GID -G sudo -l \
    && : # last line

# ------------------------------------------------------------------------------
# Install dependencies
RUN set -x \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    git gcc g++ pkg-config libssl-dev libdbus-1-dev \
    libglib2.0-dev libavahi-client-dev ninja-build python3-venv python3-dev \
    python3-pip unzip libgirepository1.0-dev libcairo2-dev libreadline-dev \
    generate-ninja

COPY imx.py /usr/local
COPY startup.sh /usr/local/bin

ENV TOOLCHAIN_ROOT=/opt/toolchains/crosstools-aarch64-gcc-10.3-linux-4.19-glibc-2.32-binutils-2.36.1-chip-build/
ENV TOOLCHAIN_SYSROOT=${TOOLCHAIN_ROOT}/aarch64-buildroot-linux-gnu/sysroot/
ENV HUMAX_SYSROOT=${TOOLCHAIN_ROOT}/aarch64-buildroot-linux-gnu/sysroot-humax/
ENV PKG_CONFIG_PATH=${TOOLCHAIN_SYSROOT}/usr/lib/pkgconfig
