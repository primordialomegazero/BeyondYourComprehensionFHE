FROM ubuntu:22.04

LABEL org.opencontainers.image.source="https://github.com/primordialomegazero/BeyondYourComprehensionFHE"
LABEL org.opencontainers.image.description="B6 HYDRA v6.0 — 6-Engine FHE System"
LABEL org.opencontainers.image.version="6.0.0"

# Install core dependencies
RUN apt-get update && apt-get install -y \
    build-essential cmake git g++ make \
    libssl-dev libntl-dev libgmp-dev \
    python3 python3-pip wget curl \
    && rm -rf /var/lib/apt/lists/*

# Install OpenFHE
WORKDIR /opt
RUN git clone https://github.com/openfheorg/openfhe-development.git openfhe --depth 1
WORKDIR /opt/openfhe
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/openfhe -DBUILD_SHARED=ON -DWITH_NTL=OFF -DWITH_OPENMP=OFF && \
    make -j$(nproc) && make install && ldconfig

# Install schupy
RUN pip install schupy

# Copy and build B6 HYDRA
WORKDIR /app
COPY . .
ENV LD_LIBRARY_PATH=/usr/local/openfhe/lib:/usr/local/lib:$LD_LIBRARY_PATH

RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc) b6_hydra

# Default: run B6 HYDRA
CMD ["./build/b6_hydra"]
