# =========================
# BUILD STAGE
# =========================
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wget \
    git \
    build-essential \
    libssl-dev \
    ninja-build \
    zlib1g-dev \
    libpqxx-dev \
    libpq-dev \
    libmpc-dev \
    flex \
    bison \
    texinfo \
    libgmp-dev \
    libmpfr-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

# ----- GCC -----
RUN git clone https://gcc.gnu.org/git/gcc.git gcc-trunk
WORKDIR /tmp/gcc-trunk

RUN ./contrib/download_prerequisites

RUN mkdir build && cd build && \
    ../configure \
        --prefix=/opt/gcc-15.2 \
        --disable-multilib \
        --enable-languages=c,c++ \
        --program-suffix=-15.2 && \
    make -j$(nproc) && \
    make install-strip

# ----- CMAKE -----
WORKDIR /tmp

RUN wget https://github.com/Kitware/CMake/releases/download/v3.29.5/cmake-3.29.5.tar.gz && \
    tar -xf cmake-3.29.5.tar.gz

WORKDIR /tmp/cmake-3.29.5

RUN ./bootstrap && \
    make -j$(nproc) && \
    make install

# =========================
# RUNTIME STAGE
# =========================
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y \
    git \
    make \
    build-essential \
    ninja-build \
    libstdc++6 \
    libgcc-s1 \
    libgmp10 \
    libmpfr6 \
    libmpc3 \
    zlib1g \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# ---- USER ----
ARG USERNAME=test
ARG PASSWORD=test

RUN useradd -m -s /bin/bash ${USERNAME} \
    && echo "${USERNAME}:${PASSWORD}" | chpasswd \
    && usermod -aG sudo ${USERNAME}

# (optional) passwordless sudo
RUN echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Copy toolchains
COPY --from=builder /opt/gcc-15.2 /opt/gcc-15.2
COPY --from=builder /usr/local /usr/local

# Make gcc-15.2 default
RUN update-alternatives --install /usr/bin/gcc gcc /opt/gcc-15.2/bin/gcc-15.2 100 && \
    update-alternatives --install /usr/bin/g++ g++ /opt/gcc-15.2/bin/g++-15.2 100 && \
    update-alternatives --install /usr/bin/c++ c++ /opt/gcc-15.2/bin/c++-15.2 100 && \
    update-alternatives --install /usr/bin/cpp cpp /opt/gcc-15.2/bin/cpp-15.2 100

# Update LD_LIBRARY:
RUN echo "/opt/gcc-15.2/lib64" > /etc/ld.so.conf.d/gcc-15.2.conf
RUN ldconfig

# Update PATH:
ENV PATH="/opt/gcc-15.2/bin:/usr/local/bin:${PATH}"

# по умолчанию логинимся как test
USER ${USERNAME}
WORKDIR /home/${USERNAME}

CMD ["bash"]