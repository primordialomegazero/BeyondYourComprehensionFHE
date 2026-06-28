FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y g++ libc6-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /build
COPY src/pozd_fhe.h .
COPY src/fractal.h .
COPY src/antimatter.h .
COPY src/pqc.h .
COPY src/zkp.h .
COPY src/supply_chain.h .
COPY src/pozd_server.cpp .
RUN g++ -std=c++17 -O3 -march=native -pthread -static -o pozd_server pozd_server.cpp -lm

FROM ubuntu:22.04
WORKDIR /app
COPY --from=builder /build/pozd_server .
EXPOSE 8093
HEALTHCHECK --interval=10s --timeout=3s CMD /app/pozd_server --health-check || exit 1
ENTRYPOINT ["./pozd_server"]
