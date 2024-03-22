
FROM ubuntu:jammy
LABEL org.opencontainers.image.source https://github.com/project-chip/connectedhomeip

RUN set -x \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    git gcc g++ pkg-config libssl-dev libdbus-1-dev \
    libglib2.0-dev libavahi-client-dev ninja-build python3-venv python3-dev \
    python3-pip unzip libgirepository1.0-dev libcairo2-dev libreadline-dev \
    generate-ninja

# RUN set -x && pip3 install click
# RUN set -x && pip3 install jinja2
# RUN set -x && pip3 install lark
# RUN set -x && pip3 install stringcase
# RUN set -x && pip3 install wheel

# RUN set -x && pip3 install --no-cache-dir attrs
# RUN set -x && pip3 install --no-cache-dir click
# RUN set -x && pip3 install --no-cache-dir coloredlogs
# RUN set -x && pip3 install --no-cache-dir cxxfilt
# RUN set -x && pip3 install --no-cache-dir flake8
# RUN set -x && pip3 install --no-cache-dir future
# RUN set -x && pip3 install --no-cache-dir ghapi 
# RUN set -x && pip3 install --no-cache-dir mobly
# RUN set -x && pip3 install numpy --config-settings=setup-args="-Dallow-noblas=true"  
# RUN set -x && pip3 install --no-cache-dir pandas 
# RUN set -x && pip3 install --no-cache-dir portpicker 
# RUN set -x && pip3 install --no-cache-dir pygit
# RUN set -x && curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y \
#     && . /root/.bashrc \
#     && pip3 install --no-cache-dir PyGithub
# RUN set -x && pip3 install --no-cache-dir tabulate