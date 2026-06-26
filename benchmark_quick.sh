#!/bin/bash
# Quick 5-second TPS benchmark
cd build
timeout 6 ./b6_hydra 2>&1 | tail -10
