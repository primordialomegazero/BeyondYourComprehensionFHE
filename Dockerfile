FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y g++ && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY src/ src/
COPY include/ include/
COPY tests/ tests/

RUN g++ -std=c++17 -O3 -march=native -Wall -Wextra \
    -I src/core -I src/chaos -I include \
    -o demon_test tests/test_1m_demons.cpp -lm

FROM ubuntu:22.04
WORKDIR /app
COPY --from=builder /build/demon_test .
CMD ["./demon_test"]
