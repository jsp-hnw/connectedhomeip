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

# # ------------------------------------------------------------------------------
# # Install tizen
# ENV TIZEN_SDK_ROOT /opt/qorvo/

# # ------------------------------------------------------------------------------
# # Set environment
# ENV TIZEN_SDK_TOOLCHAIN $TIZEN_SDK_ROOT/toolchain/cross-pi-gcc-8.3.0-1
# ENV TIZEN_SDK_SYSROOT $TIZEN_SDK_ROOT/sysroot
# ENV PATH="$TIZEN_SDK_TOOLCHAIN/bin:$TIZEN_SDK_ROOT/tools/ide/bin:$TIZEN_SDK_ROOT/tools:$PATH"

ENV IMX_SDK_ROOT=/opt/qorvo/
#ENV PKG_CONFIG_LIBDIR=/opt/qorvo/sysroot/usr/lib
ENV PKG_CONFIG_PATH=/opt/qorvo/sysroot/usr/lib/arm-linux-gnueabihf/pkgconfig

### ENTRYPOINT ["/usr/local/bin/startup.sh"]

# ------------------------------------------------------------------------------
# Switch to the non-root user
#USER $USER_NAME
#WORKDIR $USER_HOME
