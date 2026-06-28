FROM gcc:13-bookworm AS builder
WORKDIR /app
COPY src/ .
RUN g++ -std=c++17 -O3 -march=native -pthread -static -o byc_server byc_server.cpp

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /app/byc_server .
EXPOSE 8093
CMD ["./byc_server"]
