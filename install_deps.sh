#!/bin/bash
echo "📦 INSTALLING DEPENDENCIES..."
echo "================================"

# Update
sudo apt-get update

# Core build tools
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    jq \
    python3 \
    python3-pip

# Crypto libraries
sudo apt-get install -y \
    libssl-dev \
    libntl-dev \
    libgmp-dev \
    libboost-all-dev

# Golang (for Lattigo)
sudo apt-get install -y golang-go

# OpenFHE dependencies
sudo apt-get install -y \
    libtool \
    autoconf \
    automake \
    make \
    g++ \
    libfftw3-dev

echo "✅ Dependencies installed!"
