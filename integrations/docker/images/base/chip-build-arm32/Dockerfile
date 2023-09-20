
# start with Ubuntu 20.04LTS
FROM ubuntu:jammy
LABEL org.opencontainers.image.source https://github.com/project-chip/connectedhomeip

VOLUME "/var/source"

ARG TARGETPLATFORM=linux/arm32

# Ensure TARGETPLATFORM is set
RUN case ${TARGETPLATFORM} in \
    "linux/arm32") \
    echo "Building for linux/arm32" \
    ;; \
    *) \
    if [ -z "$TARGETPLATFORM" ] ;\
    then \
    echo "TARGETPLATFORM not defined! Please run from buildkit (buildx)." \
    && return 1 ;\
    else \
    echo "Unsupported platform ${TARGETPLATFORM}." \
    && return 1 ;\
    fi \
    ;; \
    esac


# base build and check tools and libraries layer
RUN set -x \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -fy --fix-missing \
    autoconf \
    automake \
    bison \
    bridge-utils \
    ccache \
    clang \
    clang-format \
    clang-tidy \
    curl \
    flex \
    g++-10 \
    git \
    git-lfs \
    gperf \
    iproute2 \
    jq \
    lcov \
    libavahi-client-dev \
    libavahi-common-dev \
    libcairo-dev \
    libcairo2-dev \
    libdbus-1-dev \
    libdbus-glib-1-dev \
    libdmalloc-dev \
    libgif-dev \
    libglib2.0-dev \
    libical-dev \
    libjpeg-dev \
    libmbedtls-dev \
    libncurses5-dev \
    libncursesw5-dev \
    libnl-3-dev \
    libnl-route-3-dev \
    libnspr4-dev \
    libpango1.0-dev \
    libpixman-1-dev \
    libreadline-dev \
    libsdl-pango-dev \
    libsdl2-dev \
    libssl-dev \
    libtool \
    libudev-dev \
    libusb-1.0-0 \
    libusb-dev \
    libxml2-dev \
    make \
    meson \
    net-tools \
    ninja-build \
    openjdk-8-jdk \
    pkg-config \
    python-is-python3 \
    python3.9 \
    #python3.9-dev \
    #python3.9-venv \
    rsync \
    shellcheck \
    strace \
    systemd \
    udev \
    unzip \
    wget \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/ \
    && git lfs install \
    && : # last line

# Set gcc 10 as a default compiler to work with TSAN
RUN set -x \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10 \
    && update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30 \
    && update-alternatives --set cc /usr/bin/gcc \
    && update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30 \
    && update-alternatives --set c++ /usr/bin/g++ \
    && : # last line

# Cmake v3.23.1
RUN set -x \
    && cd /tmp \
    && wget --progress=dot:giga https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1.tar.gz \
    && tar xvfz cmake-3.23.1.tar.gz \
    && cd cmake-3.23.1 \
    && ./bootstrap \
    && make -j12 \
    && make install \
    && : # last line

RUN set -x \    
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -fy --fix-missing software-properties-common \
    && add-apt-repository -y ppa:deadsnakes/ppa \
    && apt update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -fy --fix-missing \
    python3.9 python3.9-dev python3.9-venv

# Python 3.9 and PIP
RUN set -x \
    && DEBIAN_FRONTEND=noninteractive  apt-get update \
    && DEBIAN_FRONTEND=noninteractive  apt-get install -y libgirepository1.0-dev \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y software-properties-common \
    && add-apt-repository universe \
    && curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py \
    && python3.9 get-pip.py \
    && update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.9 1 \
    && rm -rf /var/lib/apt/lists/ \
    && : # last line

RUN set -x && pip3 install --no-cache-dir attrs
RUN set -x && pip3 install --no-cache-dir click
RUN set -x && pip3 install --no-cache-dir coloredlogs
RUN set -x && pip3 install --no-cache-dir cxxfilt
RUN set -x && pip3 install --no-cache-dir flake8
RUN set -x && pip3 install --no-cache-dir future
RUN set -x && pip3 install --no-cache-dir ghapi 
RUN set -x && pip3 install --no-cache-dir mobly
RUN set -x && pip3 install numpy --config-settings=setup-args="-Dallow-noblas=true"  
RUN set -x && pip3 install --no-cache-dir pandas 
RUN set -x && pip3 install --no-cache-dir portpicker 
RUN set -x && pip3 install --no-cache-dir pygit
RUN set -x && curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y \
    && . /root/.bashrc \
    && pip3 install --no-cache-dir PyGithub
RUN set -x && pip3 install --no-cache-dir tabulate

# build and install gn
RUN set -x \
    && git clone https://gn.googlesource.com/gn \
    && cd gn \
    && python3 build/gen.py \
    && ninja -C out \
    && cp out/gn /usr/local/bin \
    && cd .. \
    && rm -rf gn \
    && : # last line

# Install bloat comparison tools
RUN set -x \
    && git clone https://github.com/google/bloaty.git \
    && mkdir -p bloaty/build \
    && cd bloaty/build \
    && cmake ../ \
    && make -j12 \
    && make install \
    && cd ../.. \
    && rm -rf bloaty \
    && : # last line

# Need newer version of include-what-you-use
RUN set -x \
    && apt-get update \
    # Install build and runtime requirements for IWYU
    && DEBIAN_FRONTEND=noninteractive apt-get install -fy --fix-missing clang-12 libclang-12-dev \
    # Build and install IWYU
    && git clone --depth=1 --branch=clang_12 https://github.com/include-what-you-use/include-what-you-use.git \
    && mkdir -p include-what-you-use/build \
    && cd include-what-you-use/build \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/usr/lib/llvm-12 -DIWYU_LINK_CLANG_DYLIB=OFF .. \
    && make -j12 \
    && make install \
    # Save clang-12 files, so we can restore them after build dependencies cleanup
    && tar -cf clang-12-files.tar $(dpkg -L libclang-common-12-dev |grep /include) /usr/lib/llvm-12/lib/libLLVM-12.so.1 \
    # Cleanup build dependencies
    && apt autopurge -fy clang-12 libclang-12-dev \
    && rm -rf /var/lib/apt/lists/ \
    # Restore clang-12 files
    && tar -xf clang-12-files.tar -C / \
    # Cleanup
    && cd ../.. \
    && rm -rf include-what-you-use \
    && : # last line

# Build glib-2.0 from source with enabled thread sanitizer. This is needed for
# running CHIP tests with TSAN enabled. When running applications with TSAN
# all shared libraries should be built with TSAN enabled, otherwise TSAN might
# report false positives. This case is most prominent with glib-2.0, which has
# a lot of threads-related APIs.
ENV LD_LIBRARY_PATH_TSAN=/usr/lib/x86_64-linux-gnu-tsan
RUN case ${TARGETPLATFORM} in \
    "linux/amd64") \
    set -x \
    && mkdir -p $LD_LIBRARY_PATH_TSAN \
    && export CCACHE_DISABLE=1 PYTHONDONTWRITEBYTECODE=1 \
    && GLIB_VERSION=$(pkg-config --modversion glib-2.0) \
    && git clone --depth=1 --branch=$GLIB_VERSION https://github.com/GNOME/glib.git \
    && CFLAGS="-O2 -g -fsanitize=thread" meson glib/build glib -Dtests=false \
    && DESTDIR=../build-image ninja -C glib/build install \
    && mv glib/build-image/usr/local/lib/x86_64-linux-gnu/lib* $LD_LIBRARY_PATH_TSAN \
    && rm -rf glib \
    ;; \
    "linux/arm32") \
    echo "ARM32 unsupported with TSAN" \
    ;; \
    *) \
    echo "Unsupported platform ${TARGETPLATFORM}" \
    && return 1 ;\
    ;; \
    esac

# NodeJS: install a newer version than what apt-get would read
# This installs the latest LTS version of nodejs
#
# NodeJS is required by github actions, we use Wandalen/wretry.action@v1.3.0
# and that seems to use the built-in node installation in the image
#
# This is not a CHIP dependency directly, but used by CI
ENV CHIP_NODE_VERSION=v16.13.2
ENV NODE_PLATFORM_VERSION=
RUN case ${TARGETPLATFORM} in \
    "linux/arm32") NODE_PLATFORM_VERSION=armv7l;; \
    *) \
    test -n "$TARGETPLATFORM" \
    echo "Unsupported platform ${TARGETPLATFORM}" \
    && return 1 ;\
    ;; \
    esac \
    && set -x \
    && mkdir node_js \
    && cd node_js \
    && wget https://nodejs.org/dist/$CHIP_NODE_VERSION/node-$CHIP_NODE_VERSION-linux-$NODE_PLATFORM_VERSION.tar.xz \
    && tar xfvJ node-$CHIP_NODE_VERSION-linux-$NODE_PLATFORM_VERSION.tar.xz \
    && mv node-$CHIP_NODE_VERSION-linux-$NODE_PLATFORM_VERSION /opt/ \
    && ln -s /opt/node-$CHIP_NODE_VERSION-linux-$NODE_PLATFORM_VERSION /opt/node \
    && ln -s /opt/node/bin/* /usr/bin \
    && cd .. \
    && rm -rf node_js \
    && : # last line