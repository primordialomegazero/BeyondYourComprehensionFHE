# Stage 1: Build liboqs (PQC)
FROM gcc:13-bookworm AS oqs
RUN apt-get update && apt-get install -y cmake ninja-build git && rm -rf /var/lib/apt/lists/*
RUN git clone --depth 1 --branch main https://github.com/open-quantum-safe/liboqs.git /tmp/liboqs \
    && cd /tmp/liboqs && mkdir build && cd build \
    && cmake -GNinja -DCMAKE_BUILD_TYPE=Release .. \
    && ninja && ninja install

# Stage 2: Build pozDF-FHE
FROM gcc:13-bookworm AS builder
COPY --from=oqs /usr/local/lib/liboqs.a /usr/local/lib/
COPY --from=oqs /usr/local/include/oqs /usr/local/include/oqs
WORKDIR /app
COPY src/ .
RUN g++ -std=c++17 -O3 -march=native -pthread -static \
    -o pozd_server pozd_server.cpp -lm \
    /usr/local/lib/liboqs.a -lssl -lcrypto

# Stage 3: Minimal runtime
FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y libssl3 && rm -rf /var/lib/apt/lists/*
COPY --from=builder /app/pozd_server .
EXPOSE 8093
CMD ["./pozd_server"]
